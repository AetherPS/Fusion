using Fusion.Features;
using Fusion.Features.Devkit;
using Fusion.Internal;
using Fusion.TopMenu;
using Sce.PlayStation.Core.Runtime;
using Sce.PlayStation.PUI;
using Sce.Vsh.ShellUI.TopMenu;
using System;

namespace Fusion
{
    public static class ModuleMain
    {
        [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.NoInlining)]
        public static void OnLoad()
        {
            if (!Diagnostics.IsMainThread)
            {
                FrameTask.CallOnce(OnLoad);
                return;
            }

            try
            {
                Console.WriteLine("=== Fusion UI Loading ===");

                ManifestFileUriScheme.Initialize();
                MethodOverrideManager.Initialize();
                DevkitTitleIdLabel.ShowLabels = true;
                DevkitPanel.ShowPanel = true;
                DevkitContent.DebugSettingsShortcut = true;
                DevkitContent.AppHome = true;

                // Register custom plugin
                SettingsApplicationHooks.RegisterPlugin<FusionPlugin>(FusionPlugin.PluginName);

                var m_systemAreaPanel = Reflect.Get<SystemAreaPanel>(SystemAreaManager.Instance, "m_systemAreaPanel");
                SystemAreaPanelHooks.AddFusionMenu(m_systemAreaPanel);

                Console.WriteLine("=== Fusion UI Loaded ===");

                WelcomeMessage.DoWelcome();
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