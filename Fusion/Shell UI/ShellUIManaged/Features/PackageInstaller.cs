using Fusion.Internal;
using Sce.Vsh.ShellUI.Settings.PkgInstaller;
using System;

namespace Fusion
{
    public static class PackageInstaller
    {
        private static readonly string[] _searchPaths =
        {
            "/user/data/pkg",
            "/user/data/Fusion/pkg",
        };

        [MethodOverride(typeof(SearchJob))]
        public static void SearchDisc(SearchJob instance)
        {
            Console.WriteLine("[PackageInstaller] SearchDisc -> custom paths");

            foreach (var path in _searchPaths)
            {
                try
                {
                    Console.WriteLine($"[PackageInstaller] Searching: {path}");
                    Reflect.Call(instance, "SearchDir", path, null);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[PackageInstaller] Error: {ex.Message}");
                }
            }
        }
    }
}