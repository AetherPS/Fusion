.intel_syntax noprefix

.global curthread
.global Readmsr
.global cpu_enable_wp
.global cpu_disable_wp

# Get current thread pointer from GS segment
curthread:
    mov rax, gs:0
    ret

# Read Model Specific Register
# edi = MSR register number
Readmsr:
    mov ecx, edi
    rdmsr
    shl rdx, 32
    or rax, rdx
    ret

# Enable Write Protection bit in CR0
cpu_enable_wp:
    mov rax, cr0
    or rax, 0x10000
    mov cr0, rax
    ret

# Disable Write Protection bit in CR0
cpu_disable_wp:
    mov rax, cr0
    and rax, ~0x10000
    mov cr0, rax
    ret