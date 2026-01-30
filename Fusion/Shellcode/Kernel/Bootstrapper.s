BITS 64
DEFAULT REL

entry: dq shellcode
sceKernelLoadStartModule: dq 0
sceKernelStopUnloadModule: dq 0
sceSysmodulePreloadModuleForLibkernel: dq 0
ModulePath: db '/data/Fusion/Plugins/Bootstrapper.sprx', 0

; Main shellcode function
shellcode:
    ; Tail call to original sceSysmodulePreloadModuleForLibkernel
    mov rax, qword [sceSysmodulePreloadModuleForLibkernel]
    call rax

    ; Load the module
    xor r9, r9
    xor r8, r8
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    lea rdi, [ModulePath]
    mov rax, qword [sceKernelLoadStartModule]
    call rax

    xor r9, r9
    xor r8, r8
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    mov rdi, rax
    mov rax, qword [sceKernelStopUnloadModule]
    call rax

    ret