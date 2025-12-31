#pragma once
#include "common.h"

unsigned long syscall(unsigned long n, ...);
void* curthread();
uint64_t Readmsr(int Reg);
void cpu_enable_wp();
void cpu_disable_wp();