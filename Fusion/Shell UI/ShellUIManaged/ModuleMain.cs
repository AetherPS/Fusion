using System;

namespace Fusion
{
    public static class ModuleMain
    {
        [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.NoInlining)]
        public static void OnLoad()
        {
            try
            {
                Console.WriteLine("=== Fusion UI Loading ===");

                // Initialize managers
                ResourceManager.Install();
                PackageInstaller.Install();
                CustomMenu.Initialize();

                DebugTitleIdLabel.Install();
                DebugTitleIdLabel.ShowLabels = true;

                // Register custom plugin
                CustomMenu.RegisterPlugin<FusionPlugin>(FusionPlugin.PluginName);

                Console.WriteLine("=== Fusion UI Loaded ===");
            }
            catch (System.Exception ex)
            {
                // Log exception (if you have logging)
                System.Console.WriteLine($"OnLoad failed: {ex.Message}");
                throw;
            }
        }

        public static void OnUnload()
        {
            try
            {

            }
            catch (System.Exception ex)
            {
                System.Console.WriteLine($"OnUnload failed: {ex.Message}");
            }
        }
    }
}