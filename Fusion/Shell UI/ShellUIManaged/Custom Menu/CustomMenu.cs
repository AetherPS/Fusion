using Fusion.Internal;
using Sce.PlayStation.PUI;
using Sce.Vsh.ShellUI;
using Sce.Vsh.ShellUI.Settings.Core;
using System;
using System.Collections.Generic;

namespace Fusion
{
    public static class CustomMenu
    {
        private static readonly Dictionary<string, Func<AssetManager, SettingsPlugin>> _customPlugins
            = new Dictionary<string, Func<AssetManager, SettingsPlugin>>();

        private static AssetManager _assetManager;

        public static void RegisterPlugin<T>(string name) where T : SettingsPlugin
        {
            _customPlugins[name] = (assetManager) =>
                (SettingsPlugin)Activator.CreateInstance(typeof(T), assetManager);
            Console.WriteLine($"[CustomMenu] Registered custom plugin: {name}");
        }

        public static void RegisterPlugin(string name, Func<AssetManager, SettingsPlugin> factory)
        {
            _customPlugins[name] = factory;
            Console.WriteLine($"[CustomMenu] Registered custom plugin: {name}");
        }

        [MethodOverride(typeof(SettingsApplication))]
        public static SettingsPlugin GetPlugin(SettingsApplication instance, string pluginName)
        {
            if (_assetManager == null)
                _assetManager = Reflect.Get<AssetManager>(instance, "appAssetManager");

            if (_customPlugins.TryGetValue(pluginName, out var factory))
            {
                Console.WriteLine($"[CustomMenu] Returning custom: {pluginName}");
                return factory(_assetManager);
            }

            return MethodOverrideManager.InvokeOriginal<SettingsPlugin>(instance, new object[] { pluginName });
        }
    }
}