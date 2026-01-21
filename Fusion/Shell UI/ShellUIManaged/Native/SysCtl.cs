using System;
using System.Runtime.InteropServices;

namespace Fusion.Native
{
    public static class SysCtl
    {
        /// <summary>
        /// Get a sysctl value as a string
        /// </summary>
        public static string GetString(string name)
        {
            IntPtr length = IntPtr.Zero;

            // First call to get the size
            if (LibKernel.sysctlbyname(name, IntPtr.Zero, ref length, IntPtr.Zero, 0) != 0)
            {
                throw new InvalidOperationException($"Failed to get size for sysctl '{name}'. Error: {Marshal.GetLastWin32Error()}");
            }

            // Allocate buffer
            IntPtr buffer = Marshal.AllocHGlobal(length);
            try
            {
                // Second call to get the actual data
                if (LibKernel.sysctlbyname(name, buffer, ref length, IntPtr.Zero, 0) != 0)
                {
                    throw new InvalidOperationException($"Failed to get value for sysctl '{name}'. Error: {Marshal.GetLastWin32Error()}");
                }

                return Marshal.PtrToStringAnsi(buffer);
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }

        /// <summary>
        /// Get a sysctl value as an integer (32-bit)
        /// </summary>
        public static int GetInt32(string name)
        {
            int value = 0;
            IntPtr length = new IntPtr(sizeof(int));
            IntPtr buffer = Marshal.AllocHGlobal(sizeof(int));

            try
            {
                if (LibKernel.sysctlbyname(name, buffer, ref length, IntPtr.Zero, 0) != 0)
                {
                    throw new InvalidOperationException($"Failed to get value for sysctl '{name}'. Error: {Marshal.GetLastWin32Error()}");
                }

                value = Marshal.ReadInt32(buffer);
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }

            return value;
        }

        /// <summary>
        /// Get a sysctl value as a long (64-bit)
        /// </summary>
        public static long GetInt64(string name)
        {
            long value = 0;
            IntPtr length = new IntPtr(sizeof(long));
            IntPtr buffer = Marshal.AllocHGlobal(sizeof(long));

            try
            {
                if (LibKernel.sysctlbyname(name, buffer, ref length, IntPtr.Zero, 0) != 0)
                {
                    throw new InvalidOperationException($"Failed to get value for sysctl '{name}'. Error: {Marshal.GetLastWin32Error()}");
                }

                value = Marshal.ReadInt64(buffer);
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }

            return value;
        }

        /// <summary>
        /// Get raw bytes from sysctl
        /// </summary>
        public static byte[] GetBytes(string name, long size)
        {
            IntPtr length = new IntPtr(size);
            byte[] buffer = new byte[size];
            IntPtr ptr = Marshal.AllocHGlobal((int)size);

            try
            {
                if (LibKernel.sysctlbyname(name, ptr, ref length, IntPtr.Zero, 0) != 0)
                {
                    throw new InvalidOperationException($"Failed to get value for sysctl '{name}'. Error: {Marshal.GetLastWin32Error()}");
                }

                Marshal.Copy(ptr, buffer, 0, (int)size);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }

            return buffer;
        }

        /// <summary>
        /// Set a sysctl value from an integer
        /// </summary>
        public static void SetInt32(string name, int value)
        {
            IntPtr buffer = Marshal.AllocHGlobal(sizeof(int));
            try
            {
                Marshal.WriteInt32(buffer, value);
                IntPtr oldlen = IntPtr.Zero;

                if (LibKernel.sysctlbyname(name, IntPtr.Zero, ref oldlen, buffer, sizeof(int)) != 0)
                {
                    throw new InvalidOperationException($"Failed to set value for sysctl '{name}'. Error: {Marshal.GetLastWin32Error()}");
                }
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }
    }
}
