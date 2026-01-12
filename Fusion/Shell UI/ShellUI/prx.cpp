#include "stdafx.h"

extern uint8_t _binary_ShellUIManaged_dll_start[];
extern uint8_t _binary_ShellUIManaged_dll_end[];

static PatchManager* Manager = nullptr;

void InstallMethodPatch(const char* assemblyName, const char* nameSpace,
    const char* klass, const char* methodName,
    int parameterCount, size_t offset,
    const void* data, size_t length)
{
    auto assembly = mono_domain_assembly_open(mono_get_root_domain(), assemblyName);
    if (!assembly)
    {
        Logger::Error("InstallMethodPatch: Assembly \"%s\" is not loaded!", assemblyName);
        return;
    }

    MonoImage* assemblyImage = mono_assembly_get_image(assembly);
    if (!assemblyImage)
    {
        Logger::Error("InstallMethodPatch: Failed to get image from assembly");
        return;
    }

    MonoClass* monoClass = mono_class_from_name(assemblyImage, nameSpace, klass);
    if (!monoClass)
    {
        Logger::Error("InstallMethodPatch: Failed to find class %s::%s", nameSpace, klass);
        return;
    }

    MonoMethod* method = mono_class_get_method_from_name(monoClass, methodName, parameterCount);
    if (!method)
    {
        Logger::Error("InstallMethodPatch: Failed to find method %s", methodName);
        return;
    }

    void* methodAddress = mono_compile_method(method);
    if (!methodAddress)
    {
        Logger::Error("InstallMethodPatch: Failed to compile method!");
        return;
    }

    uint64_t patchAddress = (uint64_t)methodAddress + offset;

    Logger::Info("Installing patch at: 0x%llx (size: %zu bytes)", patchAddress, length);
    Manager->AddPatch(methodName, patchAddress, data, length);
}

extern "C"
{
    __declspec(dllexport)
        void* AllocateExecutableMemory(int size)
    {
        if (size <= 0) return nullptr;

        void* ptr = nullptr;
        sceKernelMmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0, &ptr);

        return (ptr == MAP_FAILED) ? nullptr : ptr;
    }

    __declspec(dllexport)
        void FreeExecutableMemory(void* ptr, int size)
    {
        if (ptr)
            sceKernelMunmap(ptr, size);
    }

    int __cdecl module_start(size_t argc, const void* args)
    {
        ScePthread thr;
        scePthreadCreate(&thr, 0, [](void* arg) -> void*
        {
            Logger::Init(true, Logger::LogLevelAll);

            Manager = new PatchManager();

            MonoDomain* rootDomain = mono_get_root_domain();
            mono_thread_attach(rootDomain);

            InstallMethodPatch("Sce.PlayStation.Core.dll",
                "Sce.PlayStation.Core.Runtime", "Diagnostics",
                "CheckRunningOnMainThread", 0, 0, "\xC3", 1);

            MonoImageOpenStatus status;
            MonoImage* image = mono_image_open_from_data_full(
                (char*)_binary_ShellUIManaged_dll_start,
                (uint64_t)&_binary_ShellUIManaged_dll_end - (uint64_t)&_binary_ShellUIManaged_dll_start,
                1, &status, 0
            );

            if (status != MONO_IMAGE_OK || !image)
            {
                Logger::Error("Failed to open image: %d", status);
                scePthreadExit((void*)-1);
                return (void*)-1;
            }

            MonoAssembly* assembly = mono_assembly_load_from_full(image, "ShellUIManaged.dll", &status, 0);

            if (status != MONO_IMAGE_OK || !assembly)
            {
                Logger::Error("Failed to load assembly: %d", status);
                scePthreadExit((void*)-1);
                return (void*)-1;
            }

            MonoImage* asmImage = mono_assembly_get_image(assembly);
            MonoClass* klass = mono_class_from_name(asmImage, "Fusion", "ModuleMain");

            if (klass)
            {
                MonoMethod* onLoad = mono_class_get_method_from_name(klass, "OnLoad", 0);
                if (onLoad)
                {
                    mono_runtime_invoke(onLoad, nullptr, nullptr, nullptr);
                }
            }

            scePthreadExit(0);
            return 0;
        }, 0, "ModInit");

        scePthreadJoin(thr, nullptr);
        return 0;
    }

    int __cdecl module_stop(size_t argc, const void* args)
    {
        if (Manager)
        {
            delete Manager;
            Manager = nullptr;
        }

        Logger::Info("Module stopped");
        return 0;
    }
}