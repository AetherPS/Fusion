BITS 64
DEFAULT REL
entry: dq shellcode
sceKernelLoadStartModule: dq 0
sceKernelStopUnloadModule: dq 0
sceSysmodulePreloadModuleForLibkernel: dq 0
ModulePath: db '/data/Fusion/Plugins/Bootstrapper.sprx', 0

; Main shellcode function
shellcode:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15
    sub rsp, 8              ; Align stack to 16 bytes
    
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
    
    ; Unload the module (rax contains module handle from load)
    xor r9, r9
    xor r8, r8
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    mov rdi, rax
    mov rax, qword [sceKernelStopUnloadModule]
    call rax
    
    ; Restore stack and return
    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret