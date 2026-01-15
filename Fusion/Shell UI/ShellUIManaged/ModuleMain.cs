using Fusion.Internal;
using Fusion.TopMenu;
using Sce.Vsh.ShellUI.TopMenu;
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

                ManifestFileUriScheme.Initialize();
                MethodOverrideManager.Initialize();
                DebugTitleIdLabel.ShowLabels = true;
                
                // Register custom plugin
                SettingsApplicationHooks.RegisterPlugin<FusionPlugin>(FusionPlugin.PluginName);

                var m_systemAreaPanel = Reflect.Get<SystemAreaPanel>(SystemAreaManager.Instance, "m_systemAreaPanel");
                SystemAreaPanelHooks.AddFusionMenu(m_systemAreaPanel);

                Console.WriteLine("=== Fusion UI Loaded ===");
            }
            catch (Exception ex)
            {
                // Log exception (if you have logging)
                Console.WriteLine($"OnLoad failed: {ex.Message}");
                throw;
            }
        }

        public static void OnUnload()
        {
            try
            {

            }
            catch (Exception ex)
            {
                Console.WriteLine($"OnUnload failed: {ex.Message}");
            }
        }
    }
}