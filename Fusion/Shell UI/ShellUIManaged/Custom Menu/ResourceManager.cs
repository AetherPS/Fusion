using Fusion.Internal;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace Fusion
{
    public static class ResourceManager
    {
        private static readonly Dictionary<string, byte[]> _resources = new Dictionary<string, byte[]>(StringComparer.OrdinalIgnoreCase);

        public static void Initialize()
        {
            LoadFromFile("/user/data/Fusion/Resources/settings_root.xml", "settings_root.xml");
            LoadFromFile("/user/data/Fusion/Resources/fusion_menu.xml", "fusion_menu.xml");
            Console.WriteLine("[ResourceManager] Initialized");
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
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[ResourceManager] Failed to load {path}: {ex.Message}");
            }
        }

        [MethodOverride(typeof(Assembly))]
        public static Stream GetManifestResourceStream(Assembly instance, string name)
        {
            if (!string.IsNullOrEmpty(name))
            {
                if (_resources.TryGetValue(name, out var data))
                {
                    Console.WriteLine($"[ResourceManager] Override (exact): {name}");
                    return new MemoryStream(data);
                }

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

            return MethodOverrideManager.InvokeOriginal<Stream>(instance, new object[] { name });
        }

        public static void Clear()
        {
            _resources.Clear();
            Console.WriteLine("[ResourceManager] Cleared");
        }
    }
}