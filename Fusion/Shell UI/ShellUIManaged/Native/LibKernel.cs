using System;
using System.Runtime.InteropServices;

namespace Fusion.Native
{
    public static class LibKernel
    {
        [DllImport("libkernel_sys.sprx", SetLastError = true)]
        public static extern int sysctlbyname(string name, IntPtr oldp, ref IntPtr oldlenp, IntPtr newp, long newlen);
    }
}
