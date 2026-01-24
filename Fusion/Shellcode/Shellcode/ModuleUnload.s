BITS 64
DEFAULT REL

magic: db 'SHEL'
entry: dq shellcode

asceKernelStopUnloadModule: dq 0
ascePthreadExit: dq 0
ModuleHandle: dd 0 
Result: dd 0

; Main shellcode function.
shellcode:
    xor r9, r9
    xor r8, r8
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
	mov rdi, [ModuleHandle]
	mov r12, qword [asceKernelStopUnloadModule]
	call r12
	mov dword [Result], eax

	xor rdi, rdi
	mov r12, qword [ascePthreadExit]
	call r12

	xor eax, eax
	ret