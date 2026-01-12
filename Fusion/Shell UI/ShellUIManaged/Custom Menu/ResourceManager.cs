using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace Fusion
{
    public static class ResourceManager
    {
        private static readonly Dictionary<string, byte[]> _resources = new Dictionary<string, byte[]>(StringComparer.OrdinalIgnoreCase);
        private static Detour _detour;

        public static void Install()
        {
            var hook = typeof(ResourceManager).GetMethod(nameof(GetManifestResourceStream_Hook),
                BindingFlags.Static | BindingFlags.Public);

            _detour = Detour.Create(
                typeof(Assembly),
                "GetManifestResourceStream",
                new[] { typeof(string) },
                hook
            );
            
            // Can probably store these as a resource.
            LoadFromFile("/user/data/Fusion/Resources/settings_root.xml", "settings_root.xml");
            LoadFromFile("/user/data/Fusion/Resources/fusion_menu.xml", "fusion_menu.xml");

            Console.WriteLine("[ResourceManager] Installed");
        }

        public static void Register(string name, string xml)
        {
            _resources[name] = System.Text.Encoding.UTF8.GetBytes(xml);
            Console.WriteLine($"[ResourceManager] Registered: {name}");
        }

        public static void Register(string name, byte[] data)
        {
            _resources[name] = data;
            Console.WriteLine($"[ResourceManager] Registered: {name}");
        }

        private static void LoadFromFile(string path, string resourceName)
        {
            try
            {
                if (File.Exists(path))
                {
                    _resources[resourceName] = File.ReadAllBytes(path);
                    Console.WriteLine($"[ResourceManager] Loaded: {resourceName}");
                }
                else
                {
                    Console.WriteLine($"[ResourceManager] File not found: {path}");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[ResourceManager] Failed to load {path}: {ex.Message}");
            }
        }

        public static Stream GetManifestResourceStream_Hook(Assembly instance, string name)
        {
            Console.WriteLine($"[ResourceManager] Request: '{name}'");

            if (!string.IsNullOrEmpty(name))
            {
                // Direct match
                if (_resources.TryGetValue(name, out var data))
                {
                    Console.WriteLine($"[ResourceManager] Override (exact): {name}");
                    return new MemoryStream(data);
                }

                // Suffix match
                foreach (var kvp in _resources)
                {
                    if (name.EndsWith(kvp.Key, StringComparison.OrdinalIgnoreCase) ||
                        name.EndsWith("." + kvp.Key, StringComparison.OrdinalIgnoreCase))
                    {
                        Console.WriteLine($"[ResourceManager] Override (suffix): {name} -> {kvp.Key}");
                        return new MemoryStream(kvp.Value);
                    }
                }
            }

            return _detour.CallOriginal(() => instance.GetManifestResourceStream(name));
        }

        public static void Uninstall()
        {
            _detour?.Dispose();
            _detour = null;
            _resources.Clear();

            Console.WriteLine("[ResourceManager] Uninstalled");
        }
    }
}