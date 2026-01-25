BITS 64
DEFAULT REL
entry: dq shellcode
sceKernelLoadStartModule: dq 0
original: dq 0
ModulePath: db '/data/Fusion/Plugins/ShellUI.sprx', 0
RebootFlag: dd 1  ; Flag to indicate reboot

; Main shellcode function
shellcode:
    ; Save the first 3 arguments (rdi, rsi, rdx)
    push rdi
    push rsi
    push rdx
    
    ; Load the module with reboot flag
    ; sceKernelLoadStartModule(moduleFileName, args, argp, flags, pOpt, pRes)
    ; rdi = moduleFileName, rsi = args, rdx = argp, rcx = flags, r8 = pOpt, r9 = pRes
    xor r9, r9          ; pRes = NULL
    xor r8, r8          ; pOpt = NULL
    xor rcx, rcx        ; flags = 0
    lea rdx, [RebootFlag] ; argp = &RebootFlag
    mov rsi, 4          ; args = 4 (sizeof(uint32_t))
    lea rdi, [ModulePath]
    mov rax, qword [sceKernelLoadStartModule]
    call rax
    
    ; Restore the first 3 arguments
    pop rdx
    pop rsi
    pop rdi
    
    ; Tail call to original with preserved arguments
    mov rax, qword [original]
    jmp rax