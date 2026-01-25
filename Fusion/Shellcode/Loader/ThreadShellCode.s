BITS 64
DEFAULT REL

magic: db 'SHEL'
entry: dq shellcode

; External function pointers (filled by loader)
thr_initial: dq 0
ascePthreadCreate: dq 0
ascePthreadJoin: dq 0
ascePthreadExit: dq 0
aShellCode: dq 0

; State
ShellCodeComplete: db 0

; Local vars
hThread: dq 0
str_threadName: db 'ShellCode Thread', 0

;-----------------------------------------------------------------------------
; thread_wrapper - Trampoline that calls user shellcode then exits cleanly
;
; This is the actual thread entry point passed to scePthreadCreate.
; It calls the user's shellcode, then calls scePthreadExit automatically.
;
; Input: rdi = arg passed from pthread_create (unused, or could pass context)
;-----------------------------------------------------------------------------
thread_wrapper:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15
    sub rsp, 8              ; Align stack to 16 bytes
    
    ; Save the thread arg if needed later
    mov r12, rdi
    
    ; Call the user's shellcode
    mov rax, qword [aShellCode]
    test rax, rax
    jz .skip_call           ; Safety check: do not call null pointer
    
    ; Pass the original arg to user shellcode (optional)
    mov rdi, r12
    call rax
    
    ; Store return value if you want to capture it
    mov r13, rax
    
.skip_call:
    ; Clean exit via scePthreadExit
    ; void scePthreadExit(void *value_ptr)
    xor rdi, rdi            ; Exit with NULL (or could use r13 for return value)
    mov rax, qword [ascePthreadExit]
    test rax, rax
    jz .fallback_exit       ; Safety: fall back to syscall if ptr is null
    
    call rax
    
    ; scePthreadExit should not return, but just in case:
.fallback_exit:
    xor rdi, rdi
    call sys_thr_exit
    
    ; Should never reach here
    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

;-----------------------------------------------------------------------------
; Main shellcode entry
;-----------------------------------------------------------------------------
shellcode:
    ; Load thread-local storage base into FS
    mov rdi, qword [thr_initial]
    mov rsi, qword [rdi]
    mov rdi, qword [rsi + 0x1E0]
    call amd64_set_fsbase
    
    ; Create thread with our wrapper as entry point
    ; int scePthreadCreate(ScePthread *thread, const ScePthreadAttr *attr,
    ;                      void *(*entry)(void*), void *arg, const char *name)
    lea r8, [str_threadName]
    xor rcx, rcx
    lea rdx, [thread_wrapper]
    xor rsi, rsi
    lea rdi, [hThread]
    mov r12, qword [ascePthreadCreate]
    call r12
    
    ; Check for error
    test eax, eax
    jnz .thread_failed
    
    ; Join thread (wait for completion)
    ; int scePthreadJoin(ScePthread thread, void **value_ptr)
    mov rdi, qword [hThread]
    xor rsi, rsi
    mov r12, qword [ascePthreadJoin]
    call r12
    
.thread_failed:
    ; Set completion flag
    mov byte [ShellCodeComplete], 1
    
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