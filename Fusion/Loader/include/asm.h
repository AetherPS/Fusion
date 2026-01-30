#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void* curthread();
uint64_t Readmsr(int Reg);
void cpu_enable_wp();
void cpu_disable_wp();