using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Fusion.Internal
{
    public static class MethodOverrideManager
    {
        [DllImport("ShellUI.sprx", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void AddDetour(string name, IntPtr originalMonoMethod, IntPtr detourMonoMethod);

        [DllImport("ShellUI.sprx", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern IntPtr CallOriginal(string name, IntPtr instance, IntPtr args);

        private static readonly HashSet<string> _installedHooks = new HashSet<string>();
        private static readonly Dictionary<string, string> _hookKeyLookup = new Dictionary<string, string>();

        public static void Initialize()
        {
            Initialize(Assembly.GetCallingAssembly());
        }

        public static void Initialize(Assembly assembly)
        {
            foreach (var type in assembly.GetTypes())
            {
                foreach (var method in type.GetMethods(BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic))
                {
                    var attr = method.GetCustomAttribute<MethodOverrideAttribute>();
                    if (attr != null)
                        TryInstallHook(method, attr);
                }
            }
        }

        private static bool TryInstallHook(MethodInfo hookMethod, MethodOverrideAttribute attr)
        {
            if (attr.TargetType == null)
            {
                Console.WriteLine($"[MethodOverride] Target type is null for {hookMethod.Name}");
                return false;
            }

            var targetName = attr.TargetMethodName ?? hookMethod.Name;
            var hookParams = hookMethod.GetParameters();

            var isInstanceHook = hookParams.Length > 0 &&
                (hookParams[0].ParameterType == attr.TargetType ||
                 hookParams[0].ParameterType.IsAssignableFrom(attr.TargetType) ||
                 attr.TargetType.IsAssignableFrom(hookParams[0].ParameterType) ||
                 hookParams[0].ParameterType == typeof(object) ||
                 hookParams[0].ParameterType == typeof(IntPtr));

            var searchParams = isInstanceHook
                ? hookParams.Skip(1).Select(p => p.ParameterType).ToArray()
                : hookParams.Select(p => p.ParameterType).ToArray();

            var searchFlags = BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic;
            MethodBase targetMethod = null;

            // Check if this is a constructor
            if (targetName == ".ctor" || targetName == ".cctor")
            {
                if (targetName == ".cctor")
                {
                    // Static constructor
                    targetMethod = attr.TargetType.TypeInitializer;
                }
                else
                {
                    // Instance constructor - find by parameters
                    var constructors = attr.TargetType.GetConstructors(searchFlags);
                    targetMethod = constructors.FirstOrDefault(c =>
                    {
                        var ctorParams = c.GetParameters();
                        if (ctorParams.Length != searchParams.Length)
                            return false;

                        for (int i = 0; i < ctorParams.Length; i++)
                        {
                            if (ctorParams[i].ParameterType != searchParams[i])
                                return false;
                        }
                        return true;
                    });
                }

                if (targetMethod == null)
                {
                    Console.WriteLine($"[MethodOverride] Constructor not found: {attr.TargetType.FullName}.{targetName}");
                    return false;
                }
            }
            else
            {
                // Regular method
                var method = attr.TargetType.GetMethod(targetName, searchFlags, null, searchParams, null);

                if (method == null)
                {
                    var expectedCount = isInstanceHook ? hookParams.Length - 1 : hookParams.Length;
                    method = attr.TargetType
                        .GetMethods(searchFlags)
                        .FirstOrDefault(m => m.Name == targetName && m.GetParameters().Length == expectedCount);
                }

                if (method == null)
                {
                    Console.WriteLine($"[MethodOverride] Method not found: {attr.TargetType.FullName}.{targetName}");
                    return false;
                }

                targetMethod = method;
            }

            RuntimeHelpers.RunClassConstructor(attr.TargetType.TypeHandle);
            RuntimeHelpers.RunClassConstructor(hookMethod.DeclaringType.TypeHandle);
            RuntimeHelpers.PrepareMethod(targetMethod.MethodHandle);
            RuntimeHelpers.PrepareMethod(hookMethod.MethodHandle);

            var hookKey = $"{attr.TargetType.FullName}::{targetName}";

            // Install the detour
            AddDetour(hookKey, targetMethod.MethodHandle.Value, hookMethod.MethodHandle.Value);
            _installedHooks.Add(hookKey);

            // Store reverse lookup
            var callerKey = $"{hookMethod.DeclaringType.FullName}.{hookMethod.Name}";
            _hookKeyLookup[callerKey] = hookKey;

            Console.WriteLine($"[MethodOverride] Installed: {hookKey}");
            return true;
        }

        public static object InvokeOriginal(object instance, object[] args, [CallerMemberName] string memberName = "")
        {
            var hookKey = ResolveHookKey(memberName);
            if (hookKey == null)
            {
                Console.WriteLine($"[MethodOverride] InvokeOriginal: Could not resolve hook key for {memberName}");
                return null;
            }

            try
            {
                // Convert instance to GCHandle
                IntPtr instancePtr = IntPtr.Zero;
                GCHandle instanceHandle = default(GCHandle);
                if (instance != null)
                {
                    instanceHandle = GCHandle.Alloc(instance);
                    instancePtr = GCHandle.ToIntPtr(instanceHandle);
                }

                // Convert args array to GCHandle
                IntPtr argsPtr = IntPtr.Zero;
                GCHandle argsHandle = default(GCHandle);
                if (args != null && args.Length > 0)
                {
                    argsHandle = GCHandle.Alloc(args);
                    argsPtr = GCHandle.ToIntPtr(argsHandle);
                }

                try
                {
                    // Call native function
                    IntPtr resultPtr = CallOriginal(hookKey, instancePtr, argsPtr);

                    // Convert result back from GCHandle
                    if (resultPtr != IntPtr.Zero)
                    {
                        GCHandle resultHandle = GCHandle.FromIntPtr(resultPtr);
                        object result = resultHandle.Target;
                        resultHandle.Free();
                        return result;
                    }
                }
                finally
                {
                    // Clean up handles
                    if (instanceHandle.IsAllocated)
                        instanceHandle.Free();
                    if (argsHandle.IsAllocated)
                        argsHandle.Free();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[MethodOverride] InvokeOriginal failed for {hookKey}: {ex.Message}");
            }

            return null;
        }

        public static TResult InvokeOriginal<TResult>(object instance, object[] args, [CallerMemberName] string memberName = "")
        {
            var result = InvokeOriginal(instance, args, memberName);
            if (result == null)
                return default(TResult);
            return (TResult)result;
        }

        private static string ResolveHookKey(string memberName)
        {
            foreach (var kvp in _hookKeyLookup)
            {
                if (kvp.Key.EndsWith("." + memberName))
                    return kvp.Value;
            }

            Console.WriteLine($"[MethodOverride] ResolveHookKey: No hook found for {memberName}");
            return null;
        }
    }
}