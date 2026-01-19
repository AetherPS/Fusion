#include "stdafx.h"
#include "Exports.h"

void* get_compiled_method(MonoMethod* method)
{
    if (!method) return nullptr;

    // Try AOT first, fall back to JIT
    void* aot_code = mono_aot_get_method(mono_domain_get(), method);
    if (aot_code)
        return aot_code;

    return mono_compile_method(method);
}

uint64_t GetAddressOfMethod(const char* assemblyName, const char* nameSpace, const char* className, const char* methodName, int ParamaterCount)
{
    auto assembly = mono_domain_assembly_open(mono_get_root_domain(), assemblyName);
    if (assembly == nullptr)
    {
        Logger::Error("GetAddressOfMethod: Failed to open \"%s\" assembly.", assemblyName);
        return 0;
    }

    auto assemblyImage = mono_assembly_get_image(assembly);
    if (assemblyImage == nullptr)
    {
        Logger::Error("GetAddressOfMethod: Failed to open \"%s\" Image.", assemblyName);
        return 0;
    }

    MonoClass* klass = mono_class_from_name(assemblyImage, nameSpace, className);

    if (klass == nullptr)
    {
        Logger::Error("GetAddressOfMethod: Failed to open \"%s\" class from \"%s\" Namespace.", className, nameSpace);
        return 0;
    }

    if (!klass)
    {
        Logger::Error("GetAddressOfMethod: failed to open class \"%s\" in namespace \"%s\"", className, nameSpace);
        return 0;
    }

    MonoMethod* Method = mono_class_get_method_from_name(klass, methodName, ParamaterCount);
    if (!Method)
    {
        Logger::Error("GetAddressOfMethod: failed to find method \"%s\" in class \"%s\"", methodName, className);
        return 0;
    }

    return (uint64_t)get_compiled_method(Method);
}

extern "C"
{
    __declspec(dllexport) void AddMethodDetour(const char* assemblyName, const char* nameSpace, const char* klass, const char* methodName, int parameterCount, MonoMethod* detour_mono_method, const char* hookKey)
    {
        uint64_t originalAddress = GetAddressOfMethod(assemblyName, nameSpace, klass, methodName, parameterCount);
        if (!originalAddress)
        {
            Logger::Error("[Detour] Failed to get address of original method: %s", methodName);
            return;
        }

        void* compiled_detour = get_compiled_method(detour_mono_method);
        if (!compiled_detour)
        {
            Logger::Error("[Detour] Failed to compile detour method: %s", methodName);
            return;
        }

        Manager->AddDetour<Detour64>(hookKey, originalAddress, compiled_detour);
        Logger::Success("[Detour] Installed: %s (key: %s)", methodName, hookKey);
    }

    __declspec(dllexport) void* GetStubAddress(const char* hookKey)
    {
        void* stub = Manager->GetStub(hookKey);
        if (!stub)
        {
            Logger::Error("[GetStubAddress] Stub not found: %s", hookKey);
        }
        return stub;
    }
}