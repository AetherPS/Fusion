#pragma once

extern "C"
{
    extern thread* CurrentThread();
    extern uint64_t Readmsr(int Reg);
    extern void cpu_enable_wp();
    extern void cpu_disable_wp();
}
