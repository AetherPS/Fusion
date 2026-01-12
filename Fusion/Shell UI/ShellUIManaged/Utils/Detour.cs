using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Fusion
{
    public class Detour : IDisposable
    {
        private readonly MethodBase _target;
        private readonly MethodInfo _hook;
        private readonly IntPtr _targetPtr;
        private readonly byte[] _originalBytes;
        private readonly object _lock = new object();
        private bool _isApplied;
        private bool _disposed;

        private const int PatchSize = 12;

        public bool IsApplied => _isApplied;
        public MethodBase TargetMethod => _target;
        public MethodInfo HookMethod => _hook;

        private Detour(MethodBase target, MethodInfo hook)
        {
            _target = target ?? throw new ArgumentNullException(nameof(target));
            _hook = hook ?? throw new ArgumentNullException(nameof(hook));

            RuntimeHelpers.PrepareMethod(target.MethodHandle);
            RuntimeHelpers.PrepareMethod(hook.MethodHandle);

            _targetPtr = target.MethodHandle.GetFunctionPointer();

            _originalBytes = new byte[PatchSize];
            Marshal.Copy(_targetPtr, _originalBytes, 0, PatchSize);
        }

        public TResult CallOriginal<TResult>(Func<TResult> originalCall)
        {
            lock (_lock)
            {
                bool wasApplied = _isApplied;
                try
                {
                    if (wasApplied)
                        RemoveInternal();

                    return originalCall();
                }
                finally
                {
                    if (wasApplied)
                        ApplyInternal();
                }
            }
        }

        public void CallOriginal(Action originalCall)
        {
            lock (_lock)
            {
                bool wasApplied = _isApplied;
                try
                {
                    if (wasApplied)
                        RemoveInternal();

                    originalCall();
                }
                finally
                {
                    if (wasApplied)
                        ApplyInternal();
                }
            }
        }

        public void Apply()
        {
            lock (_lock)
            {
                ApplyInternal();
            }
        }

        public void Remove()
        {
            lock (_lock)
            {
                RemoveInternal();
            }
        }

        private void ApplyInternal()
        {
            if (_isApplied || _disposed) return;

            var hookPtr = _hook.MethodHandle.GetFunctionPointer();

            unsafe
            {
                byte* ptr = (byte*)_targetPtr;

                ptr[0] = 0x48;
                ptr[1] = 0xB8;
                *(long*)(ptr + 2) = (long)hookPtr;

                ptr[10] = 0xFF;
                ptr[11] = 0xE0;
            }

            _isApplied = true;
        }

        private void RemoveInternal()
        {
            if (!_isApplied || _disposed) return;
            Marshal.Copy(_originalBytes, 0, _targetPtr, PatchSize);
            _isApplied = false;
        }

        public void Dispose()
        {
            if (_disposed) return;

            lock (_lock)
            {
                RemoveInternal();
                _disposed = true;
            }
        }

        #region Static Factory Methods

        public static Detour Create(MethodBase target, MethodInfo hook)
        {
            var detour = new Detour(target, hook);
            detour.Apply();
            return detour;
        }

        public static Detour Create(Type targetType, string methodName, MethodInfo hook)
        {
            return Create(targetType, methodName, null, hook);
        }

        public static Detour Create(Type targetType, string methodName, Type[] parameterTypes, MethodInfo hook)
        {
            var flags = BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic;

            MethodInfo target = parameterTypes != null
                ? targetType.GetMethod(methodName, flags, null, parameterTypes, null)
                : targetType.GetMethod(methodName, flags);

            if (target == null)
                throw new ArgumentException($"Method not found: {targetType.FullName}.{methodName}");

            return Create(target, hook);
        }

        public static Detour CreateForConstructor(Type targetType, Type[] parameterTypes, MethodInfo hook)
        {
            var flags = BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic;

            ConstructorInfo ctor = parameterTypes != null
                ? targetType.GetConstructor(flags, null, parameterTypes, null)
                : targetType.GetConstructors(flags)[0];

            if (ctor == null)
                throw new ArgumentException($"Constructor not found: {targetType.FullName}");

            return Create(ctor, hook);
        }

        public static Detour Create(string assemblyName, string namespaceName, string className, string methodName, MethodInfo hook)
        {
            return Create(assemblyName, namespaceName, className, methodName, null, hook);
        }

        public static Detour Create(string assemblyName, string namespaceName, string className, string methodName, Type[] parameterTypes, MethodInfo hook)
        {
            var target = FindMethod(assemblyName, namespaceName, className, methodName, parameterTypes);

            if (target == null)
                throw new ArgumentException($"Method not found: {namespaceName}.{className}.{methodName}");

            return Create(target, hook);
        }

        private static MethodInfo FindMethod(string assemblyName, string namespaceName, string className, string methodName, Type[] parameterTypes)
        {
            Assembly assembly = null;
            string searchName = assemblyName.Replace(".exe", "").Replace(".dll", "");

            foreach (var asm in AppDomain.CurrentDomain.GetAssemblies())
            {
                var name = asm.GetName().Name;
                if (name.Equals(assemblyName, StringComparison.OrdinalIgnoreCase) ||
                    name.Equals(searchName, StringComparison.OrdinalIgnoreCase))
                {
                    assembly = asm;
                    break;
                }
            }

            if (assembly == null)
            {
                Console.WriteLine($"[Detour] Assembly not found: {assemblyName}");
                return null;
            }

            var fullTypeName = string.IsNullOrEmpty(namespaceName) ? className : $"{namespaceName}.{className}";
            var type = assembly.GetType(fullTypeName) ?? assembly.GetType(fullTypeName.Replace(".", "+"));

            if (type == null)
            {
                Console.WriteLine($"[Detour] Type not found: {fullTypeName}");
                return null;
            }

            var flags = BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic;

            if (parameterTypes != null)
                return type.GetMethod(methodName, flags, null, parameterTypes, null);

            try
            {
                return type.GetMethod(methodName, flags);
            }
            catch (AmbiguousMatchException)
            {
                Console.WriteLine($"[Detour] Ambiguous method: {methodName}. Specify parameter types.");
                return null;
            }
        }

        #endregion
    }

    public static class DetourManager
    {
        private static readonly Dictionary<string, Detour> _detours = new Dictionary<string, Detour>();

        public static Detour Install(string key, MethodInfo target, MethodInfo hook)
        {
            if (_detours.ContainsKey(key))
                throw new InvalidOperationException($"Detour '{key}' already exists");

            var detour = Detour.Create(target, hook);
            _detours[key] = detour;
            Console.WriteLine($"[DetourManager] Installed: {key}");
            return detour;
        }

        public static Detour Install(string key, Type targetType, string methodName, MethodInfo hook)
        {
            return Install(key, targetType, methodName, null, hook);
        }

        public static Detour Install(string key, Type targetType, string methodName, Type[] parameterTypes, MethodInfo hook)
        {
            if (_detours.ContainsKey(key))
                throw new InvalidOperationException($"Detour '{key}' already exists");

            var detour = Detour.Create(targetType, methodName, parameterTypes, hook);
            _detours[key] = detour;
            Console.WriteLine($"[DetourManager] Installed: {key}");
            return detour;
        }

        public static Detour Install(string key, string assemblyName, string namespaceName, string className, string methodName, MethodInfo hook)
        {
            return Install(key, assemblyName, namespaceName, className, methodName, null, hook);
        }

        public static Detour Install(string key, string assemblyName, string namespaceName, string className, string methodName, Type[] parameterTypes, MethodInfo hook)
        {
            if (_detours.ContainsKey(key))
                throw new InvalidOperationException($"Detour '{key}' already exists");

            var detour = Detour.Create(assemblyName, namespaceName, className, methodName, parameterTypes, hook);
            _detours[key] = detour;
            Console.WriteLine($"[DetourManager] Installed: {key}");
            return detour;
        }

        public static Detour Get(string key)
        {
            _detours.TryGetValue(key, out var detour);
            return detour;
        }

        public static void Remove(string key)
        {
            if (_detours.TryGetValue(key, out var detour))
            {
                detour.Dispose();
                _detours.Remove(key);
                Console.WriteLine($"[DetourManager] Removed: {key}");
            }
        }

        public static void RemoveAll()
        {
            foreach (var kvp in _detours)
            {
                kvp.Value.Dispose();
                Console.WriteLine($"[DetourManager] Removed: {kvp.Key}");
            }
            _detours.Clear();
        }
    }
}