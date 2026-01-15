.intel_syntax noprefix
.text

.global _start
.global CurrentThread
.global Readmsr
.global cpu_enable_wp
.global cpu_disable_wp

_start:
  jmp _main

CurrentThread:
  mov rax, gs:0
  ret

Readmsr:
  mov ecx, edi
  rdmsr
  shl rdx, 32
  or rax, rdx
  ret

cpu_enable_wp:
  mov rax, cr0
  or rax, 0x10000
  mov cr0, rax
  ret

cpu_disable_wp:
  mov rax, cr0
  and rax, ~0x10000
  mov cr0, rax
  ret

# Mark stack as non-executable
.section .note.GNU-stack,"",@progbits