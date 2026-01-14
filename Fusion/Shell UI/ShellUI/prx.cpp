#include "stdafx.h"

extern uint8_t _binary_ShellUIManaged_dll_start[];
extern uint8_t _binary_ShellUIManaged_dll_end[];

DetourManager* Manager;

void LoadManagedDll()
{
    MonoDomain* rootDomain = mono_get_root_domain();
    mono_thread_attach(rootDomain);

    MonoImageOpenStatus status;
    MonoImage* image = mono_image_open_from_data_full(
        (char*)_binary_ShellUIManaged_dll_start,
        (uint64_t)&_binary_ShellUIManaged_dll_end - (uint64_t)&_binary_ShellUIManaged_dll_start,
        1, &status, 0
    );

    if (status != MONO_IMAGE_OK || !image)
    {
        Logger::Error("Failed to open image: %d", status);
        return;
    }

    MonoAssembly* assembly = mono_assembly_load_from_full(image, "ShellUIManaged.dll", &status, 0);

    if (status != MONO_IMAGE_OK || !assembly)
    {
        Logger::Error("Failed to load assembly: %d", status);
        return;
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
}

extern "C"
{
    int __cdecl module_start(size_t argc, const void* args)
    {
        ScePthread thr;
        scePthreadCreate(&thr, 0, [](void* arg) -> void*
        {
            Logger::Init(true, Logger::LogLevelAll);

            Manager = new DetourManager();
            LoadManagedDll();

            scePthreadExit(0);
            return 0;
        }, 0, "ModInit");

        scePthreadJoin(thr, nullptr);
        return 0;
    }

    int __cdecl module_stop(size_t argc, const void* args)
    {
        delete Manager;
        return 0;
    }
}