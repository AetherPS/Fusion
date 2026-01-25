BITS 64
DEFAULT REL

thr_initial: dq 0
sceKernelLoadStartModule: dq 0
Path times 4096 db 0
entry: dq shellcode

shellcode:
	; Load thread-local storage base into FS
    mov rdi, qword [thr_initial]
    mov rsi, qword [rdi]
    mov rdi, qword [rsi + 0x1E0]
    call amd64_set_fsbase

    xor r9, r9
    xor r8, r8
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    lea rdi, [Path]
    mov r12, qword [sceKernelLoadStartModule]
	call r12

    ; Exit main thread
    xor rdi, rdi
    call sys_thr_exit
    
    ret

;-----------------------------------------------------------------------------
; Syscall wrappers
;-----------------------------------------------------------------------------
sys_thr_exit:
    mov rax, 431
    mov r10, rcx
    syscall
    ret

sys_sysarch:
    mov rax, 165
    mov r10, rcx
    syscall
    ret

amd64_set_fsbase:
    push rbp
    mov rbp, rsp
    push rbx
    sub rsp, 0x18
    mov qword [rbp - 0x18], rdi
    lea rsi, [rbp - 0x18]
    mov edi, 129
    call sys_sysarch
    add rsp, 0x18
    pop rbx
    pop rbp
    ret