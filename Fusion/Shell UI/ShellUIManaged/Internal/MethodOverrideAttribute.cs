using System;

namespace Fusion.Internal
{
    [AttributeUsage(AttributeTargets.Method)]
    public class MethodOverrideAttribute : Attribute
    {
        public Type TargetType { get; }
        public string TargetMethodName { get; }

        public MethodOverrideAttribute(Type targetType, string targetMethodName = null)
        {
            TargetType = targetType;
            TargetMethodName = targetMethodName;
        }
    }
}