using Fusion.Internal;
using Sce.Vsh.ShellUI.TopMenu;
using Sce.PlayStation.PUI.UI2;
using System.Collections.Generic;

namespace Fusion.TopMenu
{
    internal class SystemAreaPanelHooks
    {
        public static void AddFusionMenu(SystemAreaPanel instance)
        {
            var m_baseWidget = Reflect.Get<Widget>(instance, "m_baseWidget");
            var m_systemAreaIconList = Reflect.Get<List<SystemAreaIconBase>>(instance, "m_systemAreaIconList");

            // Create the panel
            var fusionPanel = new Panel()
            {
                Name = "FusionPanel",
                Width = 128,
                Height = 128,
                X = 56,
                Y = 38,
            };

            m_baseWidget.AppendChild(fusionPanel);

            // Create the icon
            var fusionIcon = new SystemAreaIconFusion(fusionPanel);

            // Insert at the beginning (leftmost position)
            m_systemAreaIconList.Insert(0, fusionIcon);

            Reflect.Call(instance, "createVoiceGuide", new object[] { "Fusion" });
        }

        [MethodOverride(typeof(SystemAreaPanel))]
        public static void SysItemInit(SystemAreaPanel instance)
        {
            AddFusionMenu(instance);

            MethodOverrideManager.InvokeOriginal(instance, new object[] { });
        }
    }
}