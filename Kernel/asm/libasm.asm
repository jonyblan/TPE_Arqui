GLOBAL cpuVendor
GLOBAL fillRegisters
GLOBAL getInput

section .text

fillRegisters:
	push rbp
	mov rbp, rsp
	
	mov [rdi], rax 
	mov [rdi + 8], rbx
	mov [rdi + 16], rcx
	mov [rdi + 24], rdx
	mov [rdi + 32], r8
	mov [rdi + 40], r9
	mov [rdi + 48], r10
	mov [rdi + 56], r11
	mov [rdi + 64], r12
	mov [rdi + 72], r13
	mov [rdi + 80], r14

	mov rsp, rbp
	pop rbp
	ret

	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getInput:
    xor rax, rax
    in al, 0x60
    ret
