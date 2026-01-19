using System;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Fusion.Internal
{
    public static class MethodOverrideManager
    {
        [DllImport("ShellUI.sprx", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void AddMethodDetour(
            string assemblyName,
            string nameSpace,
            string klass,
            string methodName,
            int parameterCount,
            IntPtr detourMonoMethod,
            string hookKey);

        [DllImport("ShellUI.sprx", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern IntPtr GetStubAddress(string hookKey);

        public static void Initialize()
        {
            Initialize(Assembly.GetCallingAssembly());
        }

        public static void Initialize(Assembly assembly)
        {
            var hookMethods = assembly.GetTypes()
                .SelectMany(t => t.GetMethods(BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic))
                .Select(m => new { Method = m, Attribute = m.GetCustomAttribute<MethodOverrideAttribute>() })
                .Where(x => x.Attribute != null);

            foreach (var hook in hookMethods)
            {
                TryInstallHook(hook.Method, hook.Attribute);
            }
        }

        private static bool TryInstallHook(MethodInfo hookMethod, MethodOverrideAttribute attr)
        {
            try
            {
                if (attr.TargetType == null)
                {
                    Console.WriteLine($"[MethodOverride] Target type is null for {hookMethod.Name}");
                    return false;
                }

                var targetName = attr.TargetMethodName ?? hookMethod.Name;
                var hookParams = hookMethod.GetParameters();

                // Determine if this is an instance hook (first param is the instance)
                var isInstanceHook = hookParams.Length > 0 &&
                    (hookParams[0].ParameterType == attr.TargetType ||
                     hookParams[0].ParameterType.IsAssignableFrom(attr.TargetType) ||
                     attr.TargetType.IsAssignableFrom(hookParams[0].ParameterType) ||
                     hookParams[0].ParameterType == typeof(object) ||
                     hookParams[0].ParameterType == typeof(IntPtr));

                // Get method parameters (excluding instance param if present)
                var methodParams = (isInstanceHook
                    ? hookParams.Skip(1)
                    : hookParams)
                    .Select(p => p.ParameterType)
                    .ToArray();

                // Find the target method
                MethodBase targetMethod = (targetName == ".ctor" || targetName == ".cctor")
                    ? FindConstructor(attr.TargetType, targetName, methodParams)
                    : FindMethod(attr.TargetType, targetName, methodParams, isInstanceHook);

                if (targetMethod == null)
                {
                    Console.WriteLine($"[MethodOverride] Target not found: {attr.TargetType.FullName}.{targetName}");
                    return false;
                }

                int paramCount = methodParams.Length;
                string assemblyName = attr.TargetType.Assembly.GetName().Name;

                // Handle nested classes properly with Mono's / separator
                string nameSpace;
                string className;
                GetTypeNameParts(attr.TargetType, out nameSpace, out className);

                // Create hook key for stub lookup
                var hookKey = $"{targetMethod.DeclaringType.FullName}::{targetMethod.Name}";

                // Install the detour
                AddMethodDetour(
                    assemblyName,
                    nameSpace,
                    className,
                    targetName,
                    paramCount,
                    hookMethod.MethodHandle.Value,
                    hookKey);

                // Get the stub address
                IntPtr stubAddress = GetStubAddress(hookKey);
                if (stubAddress == IntPtr.Zero)
                {
                    Console.WriteLine($"[MethodOverride] Warning: No stub returned for: {hookKey}");
                }
                else
                {
                    Console.WriteLine($"[MethodOverride]   -> Stub: 0x{stubAddress.ToInt64():X}");

                    // Try to populate the stub field in the hook class
                    string stubFieldName = attr.StubFieldName ?? $"_{hookMethod.Name}_stub";

                    var stubField = hookMethod.DeclaringType.GetField(
                        stubFieldName,
                        BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic);

                    if (stubField != null)
                    {
                        // Check if it's a delegate type
                        if (stubField.FieldType.IsSubclassOf(typeof(Delegate)))
                        {
                            try
                            {
                                // Create delegate from stub address
                                var delegateInstance = Marshal.GetDelegateForFunctionPointer(stubAddress, stubField.FieldType);
                                stubField.SetValue(null, delegateInstance);
                                Console.WriteLine($"[MethodOverride]   -> Populated delegate field: {stubFieldName}");
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine($"[MethodOverride]   -> Failed to create delegate: {ex.Message}");
                            }
                        }
                        else if (stubField.FieldType == typeof(IntPtr))
                        {
                            // Just store the raw stub address
                            stubField.SetValue(null, stubAddress);
                            Console.WriteLine($"[MethodOverride]   -> Populated IntPtr field: {stubFieldName}");
                        }
                        else
                        {
                            Console.WriteLine($"[MethodOverride]   -> Field '{stubFieldName}' is not a Delegate or IntPtr type");
                        }
                    }
                    else
                    {
                        Console.WriteLine($"[MethodOverride]   -> No field '{stubFieldName}' found (optional)");
                    }
                }

                Console.WriteLine($"[MethodOverride] Hooked: {attr.TargetType.FullName}.{targetName}");
                Console.WriteLine($"[MethodOverride]   -> Implementation: {targetMethod.DeclaringType.FullName}.{targetMethod.Name}");
                Console.WriteLine($"[MethodOverride]   -> Namespace: {nameSpace}");
                Console.WriteLine($"[MethodOverride]   -> ClassName: {className}");

                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[MethodOverride] Exception in TryInstallHook: {ex}");
            }

            return false;
        }

        /// <summary>
        /// Splits a type into namespace and class name, handling nested classes with Mono's / separator.
        /// For nested classes, the class name becomes "OuterClass/InnerClass" or "Outer/Middle/Inner"
        /// </summary>
        private static void GetTypeNameParts(Type type, out string nameSpace, out string className)
        {
            nameSpace = type.Namespace ?? string.Empty;

            if (type.IsNested)
            {
                // Build the nested class path with Mono's / separator: OuterClass/InnerClass
                var parts = new System.Collections.Generic.List<string>();
                var currentType = type;

                while (currentType != null)
                {
                    parts.Insert(0, currentType.Name);
                    currentType = currentType.DeclaringType;
                }

                className = string.Join("/", parts);
            }
            else
            {
                className = type.Name;
            }
        }

        private static MethodBase FindConstructor(Type targetType, string ctorName, Type[] paramTypes)
        {
            if (ctorName == ".cctor")
                return targetType.TypeInitializer;

            return targetType.GetConstructors(BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic)
                .FirstOrDefault(c => ParametersMatch(c.GetParameters(), paramTypes));
        }

        private static MethodInfo FindMethod(Type targetType, string methodName, Type[] paramTypes, bool isInstance)
        {
            var flags = BindingFlags.Public | BindingFlags.NonPublic;
            flags |= isInstance ? BindingFlags.Instance : BindingFlags.Static;

            var declaredMethods = targetType.GetMethods(flags | BindingFlags.DeclaredOnly);

            var declaredMatch = declaredMethods
                .FirstOrDefault(m => m.Name == methodName && ParametersMatch(m.GetParameters(), paramTypes));

            if (declaredMatch != null)
                return declaredMatch;

            var inheritedMatch = targetType.GetMethods(flags)
                .FirstOrDefault(m => m.Name == methodName && ParametersMatch(m.GetParameters(), paramTypes));

            if (inheritedMatch != null)
                return inheritedMatch;

            Console.WriteLine($"[MethodOverride] *** METHOD NOT FOUND ***");
            Console.WriteLine($"[MethodOverride] Searched for: {methodName} with {paramTypes.Length} parameters");
            Console.WriteLine($"[MethodOverride] Instance method: {isInstance}");
            Console.WriteLine($"[MethodOverride] Available methods in {targetType.FullName}:");
            foreach (var method in targetType.GetMethods(flags).Where(m => m.Name == methodName))
            {
                Console.WriteLine($"[MethodOverride]   - {method.Name}({string.Join(", ", method.GetParameters().Select(p => p.ParameterType.Name))}) - IsStatic: {method.IsStatic}");
            }

            return null;
        }

        private static bool ParametersMatch(ParameterInfo[] actual, Type[] expected)
        {
            return actual.Length == expected.Length &&
                   actual.Zip(expected, (a, e) => a.ParameterType == e).All(match => match);
        }
    }
}