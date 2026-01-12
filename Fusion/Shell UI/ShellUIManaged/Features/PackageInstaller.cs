using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Fusion
{
    public static class PackageInstaller
    {
        private static readonly string[] _searchPaths =
        {
            "/user/data/pkg",
            "/user/data/Fusion/pkg",
        };

        private static Detour _detour;
        private static MethodInfo _searchDirMethod;

        public static void Install()
        {
            var searchJobType = Type.GetType("Sce.Vsh.ShellUI.Settings.PkgInstaller.SearchJob, app");

            if (searchJobType == null)
            {
                Console.WriteLine("[PackageInstaller] SearchJob type not found");
                return;
            }

            _searchDirMethod = searchJobType.GetMethod("SearchDir", BindingFlags.Instance | BindingFlags.NonPublic);

            if (_searchDirMethod == null)
            {
                Console.WriteLine("[PackageInstaller] SearchDir method not found");
                return;
            }

            var hook = typeof(PackageInstaller).GetMethod(nameof(SearchDisc_Hook), BindingFlags.Static | BindingFlags.Public);

            _detour = Detour.Create(searchJobType, "SearchDisc", Type.EmptyTypes, hook);

            Console.WriteLine("[PackageInstaller] Installed");
        }

        public static void Uninstall()
        {
            _detour?.Dispose();
            _detour = null;
            _searchDirMethod = null;

            Console.WriteLine("[PackageInstaller] Uninstalled");
        }

        public static void SearchDisc_Hook(object instance)
        {
            Console.WriteLine("[PackageInstaller] SearchDisc -> custom paths");

            foreach (var path in _searchPaths)
            {
                try
                {
                    Console.WriteLine($"[PackageInstaller] Searching: {path}");
                    _searchDirMethod.Invoke(instance, new object[] { path, null });
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[PackageInstaller] Error searching {path}: {ex.Message}");
                }
            }
        }
    }
}
