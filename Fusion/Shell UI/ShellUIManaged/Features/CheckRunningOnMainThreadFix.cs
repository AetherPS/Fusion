using Fusion.Internal;
using Sce.PlayStation.Core.Runtime;

namespace Fusion.Features
{
    internal class CheckRunningOnMainThreadFix
    {
        [MethodOverride(typeof(Diagnostics))]
        public static void CheckRunningOnMainThread()
        {
            return;
        }
    }
}
