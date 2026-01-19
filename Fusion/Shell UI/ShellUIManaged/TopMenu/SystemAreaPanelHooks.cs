using Fusion.Internal;
using Sce.PlayStation.PUI.UI2;
using Sce.Vsh.ShellUI.TopMenu;
using System;
using System.Collections.Generic;

namespace Fusion.TopMenu
{
    internal class SystemAreaPanelHooks
    {
        private unsafe delegate void SysItemInitDelegate(IntPtr instance);
        private static SysItemInitDelegate _SysItemInit_stub;

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
        public static unsafe void SysItemInit(SystemAreaPanel instance)
        {
            AddFusionMenu(instance);
            _SysItemInit_stub(*(IntPtr*)&instance);
        }
    }
}