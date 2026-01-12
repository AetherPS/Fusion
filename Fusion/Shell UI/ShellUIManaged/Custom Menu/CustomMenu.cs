using Sce.PlayStation.PUI;
using Sce.Vsh.ShellUI.Settings.Core;
using System;
using System.Collections.Generic;
using System.Reflection;

namespace Fusion
{
    public static class CustomMenu
    {
        private static readonly Dictionary<string, Func<AssetManager, SettingsPlugin>> _customPlugins
            = new Dictionary<string, Func<AssetManager, SettingsPlugin>>();

        // Cache for stock plugin types - resolved once at init
        private static readonly Dictionary<string, Type> _stockPluginTypes = new Dictionary<string, Type>();

        private static AssetManager _assetManager;

        public static void Initialize()
        {
            // Pre-resolve all stock plugin types
            ResolveStockPlugins();

            DetourManager.Install(
                key: "GetPlugin",
                assemblyName: "app",
                namespaceName: "Sce.Vsh.ShellUI",
                className: "SettingsApplication",
                methodName: "GetPlugin",
                parameterTypes: new Type[] { typeof(string) },
                hook: typeof(CustomMenu).GetMethod(nameof(GetPlugin_Hook),
                    BindingFlags.Static | BindingFlags.Public)
            );
        }

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

        public static SettingsPlugin GetPlugin_Hook(object instance, string pluginName)
        {
            Console.WriteLine($"[CustomMenu] GetPlugin: {pluginName}");

            // Get AssetManager from instance if we don't have it
            if (_assetManager == null)
            {
                _assetManager = GetAssetManagerFrom(instance);
            }

            // Check custom plugins first
            if (_customPlugins.TryGetValue(pluginName, out var factory))
            {
                Console.WriteLine($"[CustomMenu] Returning custom: {pluginName}");
                return factory(_assetManager);
            }

            // Fall back to stock plugins
            if (_stockPluginTypes.TryGetValue(pluginName, out var pluginType))
            {
                Console.WriteLine($"[CustomMenu] Creating stock: {pluginName}");
                try
                {
                    return (SettingsPlugin)Activator.CreateInstance(pluginType, _assetManager);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[CustomMenu] Failed to create {pluginName}: {ex.Message}");
                }
            }

            Console.WriteLine($"[CustomMenu] Unknown plugin: {pluginName}");
            return null;
        }

        private static AssetManager GetAssetManagerFrom(object settingsApp)
        {
            try
            {
                var field = settingsApp.GetType().GetField("appAssetManager",
                    BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);

                return field?.GetValue(settingsApp) as AssetManager;
            }
            catch
            {
                return null;
            }
        }

        private static void ResolveStockPlugins()
        {
            var mappings = new Dictionary<string, string>
            {
                { "core_plugin", "Sce.Vsh.ShellUI.Settings.Core.SettingsCorePlugin" },
                { "datetime_plugin", "Sce.Vsh.ShellUI.Settings.DateTimeSettings.Plugin" },
                { "audio_device_settings_plugin", "Sce.Vsh.ShellUI.Settings.Peripherals.AudioDeviceSettings.Plugin" },
                { "storage_data_plugin", "Sce.Vsh.ShellUI.Settings.StorageData.Plugin" },
                { "peripherals_plugin", "Sce.Vsh.ShellUI.Settings.Peripherals.Plugin" },
                { "psn_plugin", "Sce.Vsh.ShellUI.Settings.Psn.Plugin" },
                { "update_plugin", "Sce.Vsh.ShellUI.Settings.Update.Plugin" },
                { "accessibility_plugin", "Sce.Vsh.ShellUI.Settings.Accessibility.Plugin" },
                { "addcontent_plugin", "Sce.Vsh.ShellUI.Settings.Addcontent.Plugin" },
                { "console_settings_plugin", "Sce.Vsh.ShellUI.Settings.ConsoleSettings.Plugin" },
                { "video_settings_plugin", "Sce.Vsh.ShellUI.Settings.VideoSettings.Plugin" },
                { "controller_plugin", "Sce.Vsh.ShellUI.Settings.Peripherals.Controller.Plugin" },
                { "language_plugin", "Sce.Vsh.ShellUI.Settings.Language.Plugin" },
                { "remote_play_plugin", "Sce.Vsh.ShellUI.Settings.RemotePlay.Plugin" },
                { "parental_control_plugin", "Sce.Vsh.ShellUI.Settings.ParentalControl.Plugin" },
                { "bluetooth_device_plugin", "Sce.Vsh.ShellUI.Settings.Peripherals.BluetoothDevice.Plugin" },
                { "login_plugin", "Sce.Vsh.ShellUI.Settings.Login.Plugin" },
                { "keyboard_settings_plugin", "Sce.Vsh.ShellUI.Settings.Peripherals.KeyboardSettings.Plugin" },
                { "pkginstaller_plugin", "Sce.Vsh.ShellUI.Settings.PkgInstaller.Plugin" },
                { "usb_storage_device_plugin", "Sce.Vsh.ShellUI.Settings.Peripherals.UsbStorageDevice.Plugin" },
                { "power_management_plugin", "Sce.Vsh.ShellUI.Settings.PowerManagement.Plugin" },
                { "morpheus_update_plugin", "Sce.Vsh.ShellUI.Settings.Peripherals.MorpheusUpdate.Plugin" },
                { "morpheus_settings_plugin", "Sce.Vsh.ShellUI.Settings.Peripherals.MorpheusSettings.Plugin" },
                { "users_guide_plugin", "Sce.Vsh.ShellUI.Settings.UsersGuide.Plugin" },
                { "theme_plugin", "Sce.Vsh.ShellUI.Settings.Theme.Plugin" },
                { "sound_screen_plugin", "Sce.Vsh.ShellUI.Settings.SoundScreen.Plugin" },
                { "settings_root_plugin", "Sce.Vsh.ShellUI.Settings.SettingsRoot.Plugin" },
                { "format_plugin", "Sce.Vsh.ShellUI.Settings.Format.Plugin" },
                { "savedata_plugin", "Sce.Vsh.ShellUI.Settings.SaveData.Plugin" },
                { "spotify_plugin", "Sce.Vsh.ShellUI.Settings.Spotify.Plugin" },
                { "notification_settings_plugin", "Sce.Vsh.ShellUI.Settings.NotificationSettings.Plugin" },
                { "companion_app_plugin", "Sce.Vsh.ShellUI.Settings.CompanionApp.Plugin" },
                { "initial_setup_plugin", "Sce.Vsh.ShellUI.Settings.InitialSetup.Plugin" },
                { "network_plugin", "Sce.Vsh.ShellUI.Settings.Network.Plugin" },
                { "debug_settings_plugin", "Sce.Vsh.ShellUI.Settings.DebugSettings.Plugin" },
            };

            foreach (var kvp in mappings)
            {
                var type = Type.GetType($"{kvp.Value}, app");
                if (type != null)
                {
                    _stockPluginTypes[kvp.Key] = type;
                }
                else
                {
                    Console.WriteLine($"[CustomMenu] Warning: Could not resolve {kvp.Key}");
                }
            }

            Console.WriteLine($"[CustomMenu] Resolved {_stockPluginTypes.Count} stock plugins");
        }
    }
}
