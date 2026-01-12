using Sce.Vsh.ShellUI.Settings.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Fusion
{
    /// <summary>
    /// Handler for fusion menu page
    /// </summary>
    public class FusionMenuHandler : SettingsHandler
    {
        private readonly SettingsPlugin _plugin;

        public FusionMenuHandler(SettingsPlugin plugin)
        {
            _plugin = plugin;
        }

        public override void OnPageActivating(SettingPage page, PageTransitionEventArgs e) { }

        public override void OnPageActivated(SettingPage page, PageTransitionEventArgs e)
        {
            Console.WriteLine("[FusionMenu] Activated");
        }

        public override void OnPreCreate(SettingElement element, ElementCreatingEventArgs e) { }

        public override void OnPress(SettingElement element, ElementPressedEventArgs e)
        {
            Console.WriteLine($"[FusionMenu] Press: {element?.Id}");
        }

        public override void OnBackButtonPress(SettingPage page, ButtonPressedEventArgs e) { }
    }
}
