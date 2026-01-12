using Sce.PlayStation.PUI;
using Sce.Vsh.ShellUI.Settings.Core;
using System;
using System.Collections.Generic;

namespace Fusion
{
    /// <summary>
    /// Your custom plugin
    /// </summary>
    public class FusionPlugin : SettingsPlugin
    {
        public const string PluginName = "fusion_plugin";

        private readonly Dictionary<string, SettingsHandler> _handlers = new Dictionary<string, SettingsHandler>();

        public FusionPlugin(AssetManager assetManager)
            : base(PluginName, assetManager)
        {
        }

        // Makes it so you can do custom icon paths not in an RCO.
        public override string GetImageUri(string imgId)
        {
            if (imgId.Contains(":"))
            {
                return imgId;
            }
            return base.GetImageUri(imgId);
        }

        public override void Init()
        {
            Console.WriteLine("[FusionPlugin] Init");
        }

        public override void Start()
        {
            Console.WriteLine("[FusionPlugin] Start");

            _handlers["id_fusion_menu"] = new FusionMenuHandler(this);

            EnableAccessibility = false;
        }

        public override void Stop()
        {
            _handlers.Clear();
        }

        public override void Exit()
        {
        }

        public override SettingsHandler GetHandler(string pageName)
        {
            _handlers.TryGetValue(pageName, out var handler);
            return handler;
        }
    }
}
