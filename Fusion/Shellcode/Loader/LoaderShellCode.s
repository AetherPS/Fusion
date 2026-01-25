BITS 64
DEFAULT REL

magic: db 'SHEL'
entry: dq shellcode

sceKernelLoadStartModule: dq 0
scePthreadExit: dq 0
ModuleHandle: dd 0 
ModulePath times 4096 db 0

; Main shellcode function.
shellcode:
	xor r9, r9
    xor r8, r8
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    lea rdi, [ModulePath]
    mov r12, qword [sceKernelLoadStartModule]
	call r12
	mov dword [ModuleHandle], eax

	xor rdi, rdi
	mov r12, qword [scePthreadExit]
	call r12

	xor eax, eax
	ret