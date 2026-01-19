using System;
using System.Collections.Generic;
using System.Reflection;

namespace Fusion.Internal
{
    // Examples:
    //
    // // Static private method
    // Reflect.Call(typeof(SomeClass), "PrivateStaticMethod", arg1, arg2);
    // 
    // // Static with return value
    // var result = Reflect.Call<int>(typeof(SomeClass), "CalculateSomething", 42);
    // 
    // // Instance private method
    // Reflect.Call(myObject, "DoPrivateThing", "hello", 123);
    // 
    // // Instance with return
    // var data = Reflect.Call<string>(myObject, "GetPrivateData");
    // 
    // // Private fields
    // var fieldValue = Reflect.Get<int>(instance, "_privateCounter");
    // Reflect.Set(instance, "_privateCounter", 100);
    // 
    // // Static private field
    // var staticField = Reflect.Get<string>(typeof(MyClass), "_secretKey");
    // 
    // // Private properties
    // var prop = Reflect.GetProp<bool>(instance, "IsInitialized");
    // Reflect.SetProp(instance, "IsInitialized", true);

    public static class Reflect
    {
        private static readonly Dictionary<string, MethodInfo> _methodCache = new Dictionary<string, MethodInfo>();
        private static readonly Dictionary<string, Type> _typeCache = new Dictionary<string, Type>();
        private const BindingFlags All = BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic;

        /// <summary>
        /// Call a static method: Reflect.Call(typeof(MyClass), "PrivateMethod", arg1, arg2)
        /// </summary>
        public static void Call(Type type, string method, params object[] args)
        {
            GetMethod(type, method, args.Length).Invoke(null, args);
        }

        /// <summary>
        /// Call a static method with return: Reflect.Call&lt;int&gt;(typeof(MyClass), "PrivateMethod", arg1)
        /// </summary>
        public static T Call<T>(Type type, string method, params object[] args)
        {
            return (T)GetMethod(type, method, args.Length).Invoke(null, args);
        }

        /// <summary>
        /// Call an instance method: Reflect.Call(instance, "PrivateMethod", arg1, arg2)
        /// </summary>
        public static void Call(object instance, string method, params object[] args)
        {
            GetMethod(instance.GetType(), method, args.Length).Invoke(instance, args);
        }

        /// <summary>
        /// Call an instance method with return: Reflect.Call&lt;string&gt;(instance, "PrivateMethod", arg1)
        /// </summary>
        public static T Call<T>(object instance, string method, params object[] args)
        {
            return (T)GetMethod(instance.GetType(), method, args.Length).Invoke(instance, args);
        }

        /// <summary>
        /// Get a private field value: Reflect.Get&lt;int&gt;(instance, "_privateField")
        /// </summary>
        public static T Get<T>(object instance, string field)
        {
            if (instance == null)
                return default(T);

            Type type = instance.GetType();
            FieldInfo f = null;

            // Search up the type hierarchy
            while (type != null && f == null)
            {
                f = type.GetField(field, All);
                if (f != null)
                    break;
                type = type.BaseType;
            }

            return f != null ? (T)f.GetValue(instance) : default(T);
        }

        /// <summary>
        /// Get a static field value: Reflect.Get&lt;int&gt;(typeof(MyClass), "_privateField")
        /// </summary>
        public static T Get<T>(Type type, string field)
        {
            var f = type.GetField(field, All);
            return (T)f?.GetValue(null);
        }

        /// <summary>
        /// Set a private field value: Reflect.Set(instance, "_privateField", value)
        /// </summary>
        public static void Set(object instance, string field, object value)
        {
            if (instance == null)
                return;

            Type type = instance.GetType();
            FieldInfo f = null;

            // Search up the type hierarchy
            while (type != null && f == null)
            {
                f = type.GetField(field, All);
                if (f != null)
                    break;
                type = type.BaseType;
            }

            f?.SetValue(instance, value);
        }

        /// <summary>
        /// Set a static field value: Reflect.Set(typeof(MyClass), "_privateField", value)
        /// </summary>
        public static void Set(Type type, string field, object value)
        {
            var f = type.GetField(field, All);
            f?.SetValue(null, value);
        }

        /// <summary>
        /// Get a private property value: Reflect.GetProp&lt;int&gt;(instance, "PrivateProp")
        /// </summary>
        public static T GetProp<T>(object instance, string property)
        {
            if (instance == null)
                return default(T);

            Type type = instance.GetType();
            PropertyInfo p = null;

            // Search up the type hierarchy
            while (type != null && p == null)
            {
                p = type.GetProperty(property, All);
                if (p != null)
                    break;
                type = type.BaseType;
            }

            return p != null ? (T)p.GetValue(instance) : default(T);
        }

        /// <summary>
        /// Set a private property value: Reflect.SetProp(instance, "PrivateProp", value)
        /// </summary>
        public static void SetProp(object instance, string property, object value)
        {
            if (instance == null)
                return;

            Type type = instance.GetType();
            PropertyInfo p = null;

            // Search up the type hierarchy
            while (type != null && p == null)
            {
                p = type.GetProperty(property, All);
                if (p != null)
                    break;
                type = type.BaseType;
            }

            p?.SetValue(instance, value);
        }

        /// <summary>
        /// Find a type by name (supports internal types): Reflect.FindType("MyNamespace.MyInternalClass")
        /// </summary>
        public static Type FindType(string fullName, Assembly assembly = null)
        {
            if (_typeCache.TryGetValue(fullName, out var cached))
                return cached;

            Type type = null;

            if (assembly != null)
            {
                type = assembly.GetType(fullName, false);
            }
            else
            {
                // Search all loaded assemblies
                foreach (var asm in AppDomain.CurrentDomain.GetAssemblies())
                {
                    type = asm.GetType(fullName, false);
                    if (type != null)
                        break;
                }
            }

            if (type == null)
                throw new TypeLoadException($"Type not found: {fullName}");

            _typeCache[fullName] = type;
            return type;
        }

        /// <summary>
        /// Create an instance of an internal type: Reflect.Create("MyNamespace.MyClass", arg1, arg2)
        /// </summary>
        public static object Create(string typeName, params object[] args)
        {
            var type = FindType(typeName);
            return Activator.CreateInstance(type, All, null, args, null);
        }

        /// <summary>
        /// Create an instance with return type: Reflect.Create&lt;T&gt;("MyNamespace.MyClass", arg1)
        /// </summary>
        public static T Create<T>(string typeName, params object[] args)
        {
            return (T)Create(typeName, args);
        }

        /// <summary>
        /// Call a method on an internal type by name: Reflect.CallByType("MyNamespace.MyClass", instance, "Method", args)
        /// </summary>
        public static void CallByType(string typeName, object instance, string method, params object[] args)
        {
            var type = FindType(typeName);
            GetMethod(type, method, args.Length).Invoke(instance, args);
        }

        /// <summary>
        /// Call a method with return on an internal type: Reflect.CallByType&lt;T&gt;("MyNamespace.MyClass", instance, "Method", args)
        /// </summary>
        public static T CallByType<T>(string typeName, object instance, string method, params object[] args)
        {
            var type = FindType(typeName);
            return (T)GetMethod(type, method, args.Length).Invoke(instance, args);
        }

        private static MethodInfo GetMethod(Type type, string name, int paramCount)
        {
            var key = $"{type.FullName}.{name}#{paramCount}";

            if (_methodCache.TryGetValue(key, out var cached))
                return cached;

            var method = type.GetMethods(All)
                .FirstOrDefault(m => m.Name == name && m.GetParameters().Length == paramCount);

            if (method == null)
                throw new MissingMethodException($"Method not found: {type.FullName}.{name} with {paramCount} params");

            _methodCache[key] = method;
            return method;
        }

        // Needed for FirstOrDefault without full LINQ
        private static MethodInfo FirstOrDefault(this MethodInfo[] methods, Func<MethodInfo, bool> predicate)
        {
            foreach (var m in methods)
                if (predicate(m))
                    return m;
            return null;
        }
    }
}