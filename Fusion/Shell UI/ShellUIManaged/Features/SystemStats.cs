using Fusion.Native;
using System;

namespace Fusion.Features
{
    internal static class SystemStats
    {
        private static long[] _previousTicks = null;

        public static float GetCpuUsage()
        {
            try
            {
                byte[] cpTime = SysCtl.GetBytes("kern.cp_time", sizeof(long) * 5);
                if (cpTime.Length != sizeof(long) * 5)
                {
                    return 0;
                }

                long[] currentTicks = new long[5];
                Buffer.BlockCopy(cpTime, 0, currentTicks, 0, sizeof(long) * 5);

                // First call - just store the baseline
                if (_previousTicks == null)
                {
                    _previousTicks = currentTicks;
                    return 0;
                }

                // Calculate deltas since last measurement
                long totalDelta = 0;
                long idleDelta = currentTicks[4] - _previousTicks[4];

                for (int i = 0; i < 5; i++)
                {
                    totalDelta += currentTicks[i] - _previousTicks[i];
                }

                // Store current for next time
                _previousTicks = currentTicks;

                // Avoid division by zero
                if (totalDelta == 0)
                    return 0;

                // CPU usage = (total - idle) / total
                float usage = ((totalDelta - idleDelta) / (float)totalDelta) * 100.0f;
                return usage;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
            return 0;
        }
    }
}
