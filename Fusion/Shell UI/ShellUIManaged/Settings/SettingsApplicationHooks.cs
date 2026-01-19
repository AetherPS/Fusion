using Fusion.Internal;
using Sce.PlayStation.PUI;
using Sce.Vsh.ShellUI;
using Sce.Vsh.ShellUI.Settings.Core;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace Fusion
{
    public static class SettingsApplicationHooks
    {
        private unsafe delegate IntPtr GetPluginDelegate(IntPtr instance, IntPtr pluginName);
        private unsafe delegate void StartSettingsPageDelegate(IntPtr instance);

        private static GetPluginDelegate _GetPlugin_stub;
        private static StartSettingsPageDelegate _StartSettingsPage_stub;

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
        public static unsafe SettingsPlugin GetPlugin(SettingsApplication instance, string pluginName)
        {
            if (_assetManager == null)
            {
                _assetManager = Reflect.Get<AssetManager>(instance, "appAssetManager");
            }

            if (_customPlugins.TryGetValue(pluginName, out var factory))
            {
                Console.WriteLine($"[CustomMenu] Returning custom: {pluginName}");
                return factory(_assetManager);
            }

            var result = _GetPlugin_stub(*(IntPtr*)&instance, *(IntPtr*)&pluginName);
            return *(SettingsPlugin*)&result;
        }

        [MethodOverride(typeof(SettingsApplication))]
        public static Stream ReadFromAssembly(SettingsApplication instance, string fileName)
        {
            var sanitizedFileName = fileName.Replace("/", ".");

            var execuringAssembly = Assembly.GetExecutingAssembly();
            var callingAssembly = Assembly.GetCallingAssembly();

            return execuringAssembly.GetManifestResourceStream("Fusion.Settings." + sanitizedFileName) ?? callingAssembly.GetManifestResourceStream("Sce.Vsh.ShellUI.src.Sce.Vsh.ShellUI.Settings.Plugins." + sanitizedFileName);
        }

        [MethodOverride(typeof(SettingsApplication))]
        public static unsafe void StartSettingsPage(SettingsApplication instance)
        {
            var function = instance["function"];
            if (function == "fusion_menu")
            {
                var uiManager = Reflect.Get<UIManager>(instance, "uiManager");
                uiManager.Push(
                    "FusionSettings/data/fusion_menu.xml",
                    "id_fusion_menu",
                    TransitionAnimationType.Fade
                );
                return;
            }

            _StartSettingsPage_stub(*(IntPtr*)&instance);
        }
    }
}