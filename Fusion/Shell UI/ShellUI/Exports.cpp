#include "stdafx.h"
#include "Exports.h"

std::map<std::string, MonoMethod*> OriginalMethods;

void* get_compiled_method(MonoMethod* method)
{
    if (!method) return nullptr;

    // Try AOT first, fall back to JIT
    void* aot_code = mono_aot_get_method(mono_domain_get(), method);
    if (aot_code)
        return aot_code;

    return mono_compile_method(method);
}

extern "C"
{
    __declspec(dllexport) void AddDetour(const char* name, void* original_mono_method, void* detour_mono_method)
    {
        MonoMethod* original = (MonoMethod*)original_mono_method;
        MonoMethod* detour = (MonoMethod*)detour_mono_method;

        OriginalMethods[name] = original;

        void* compiled_original = get_compiled_method(original);
        void* compiled_detour = get_compiled_method(detour);

        if (!compiled_original || !compiled_detour)
        {
            Logger::Error("[Detour] Failed to compile methods: %s", name);
            return;
        }

        Manager->AddDetour<Detour64>(name, (uint64_t)compiled_original, compiled_detour);
        Logger::Success("[Detour] Installed: %s", name);
    }

    static void* CallStubWithArgs(void* stub, void** args, int argCount)
    {
        switch (argCount)
        {
        case 0: return ((void* (*)())stub)();
        case 1: return ((void* (*)(void*))stub)(args[0]);
        case 2: return ((void* (*)(void*, void*))stub)(args[0], args[1]);
        case 3: return ((void* (*)(void*, void*, void*))stub)(args[0], args[1], args[2]);
        case 4: return ((void* (*)(void*, void*, void*, void*))stub)(args[0], args[1], args[2], args[3]);
        case 5: return ((void* (*)(void*, void*, void*, void*, void*))stub)(args[0], args[1], args[2], args[3], args[4]);
        case 6: return ((void* (*)(void*, void*, void*, void*, void*, void*))stub)(args[0], args[1], args[2], args[3], args[4], args[5]);
        case 7: return ((void* (*)(void*, void*, void*, void*, void*, void*, void*))stub)(args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
        case 8: return ((void* (*)(void*, void*, void*, void*, void*, void*, void*, void*))stub)(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
        case 9: return ((void* (*)(void*, void*, void*, void*, void*, void*, void*, void*, void*))stub)(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]);
        case 10: return ((void* (*)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*))stub)(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
        default: return nullptr;
        }
    }

    __declspec(dllexport) void* CallOriginal(const char* name, void* instanceHandle, void* argsHandle)
    {
        void* stub = Manager->GetStub(name);
        if (!stub)
        {
            Logger::Error("[CallOriginal] No stub found for: %s", name);
            return nullptr;
        }

        auto it = OriginalMethods.find(name);
        if (it == OriginalMethods.end())
        {
            Logger::Error("[CallOriginal] Method not found: %s", name);
            return nullptr;
        }

        MonoMethod* method = it->second;

        // Convert GCHandles
        MonoObject* instance = nullptr;
        if (instanceHandle != nullptr)
        {
            uint32_t handle = (uint32_t)(uintptr_t)instanceHandle;
            instance = mono_gchandle_get_target(handle);
        }

        MonoArray* argsArray = nullptr;
        if (argsHandle != nullptr)
        {
            uint32_t handle = (uint32_t)(uintptr_t)argsHandle;
            argsArray = (MonoArray*)mono_gchandle_get_target(handle);
        }

        int argCount = argsArray ? mono_array_length(argsArray) : 0;
        bool isInstance = !(mono_method_get_flags(method, nullptr) & 0x0010);

        int totalArgs = isInstance ? (argCount + 1) : argCount;
        void** args = (void**)alloca(totalArgs * sizeof(void*));

        int idx = 0;
        if (isInstance)
            args[idx++] = instance;

        for (int i = 0; i < argCount; i++)
            args[idx++] = mono_array_get(argsArray, MonoObject*, i);

        // Call helper function
        void* result = CallStubWithArgs(stub, args, totalArgs);

        if (totalArgs > 10)
        {
            Logger::Error("[CallOriginal] Too many arguments: %d", totalArgs);
            return nullptr;
        }

        if (result != nullptr)
        {
            uint32_t resultHandle = mono_gchandle_new((MonoObject*)result, false);
            return (void*)(uintptr_t)resultHandle;
        }

        return nullptr;
    }
}