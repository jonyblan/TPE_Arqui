GLOBAL cpuVendor
GLOBAL ncTimeBcd
GLOBAL waitGetInputChar
GLOBAL key_pressed
GLOBAL fillRegisters

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


; TODO: Make this return a decimal instead of a BCD
; TODO: Make this able to show 2024 instead of 24
ncTimeBcd:
	push rbp
	mov rbp, rsp

	mov al, dil           ; Select hour register
	out 0x70, al
	in al, 0x71            ; Read seconds

	mov rsp, rbp
	pop rbp
	ret

; WARNING: Causes a forever loop until a key is pressed
; waits until a key is pressed and returns it in al
; https://stanislavs.org/helppc/8042.html
waitGetInputChar:
	push rbp
	mov rbp, rsp
.loop:
	in al, 0x64
	and al, 0x1
	jz .loop		; no data if it jumps
	in al, 0x60
	
	mov rsp, rbp
	pop rbp
	ret

key_pressed:
	push rbp
	mov rbp, rsp

	in al, 0x64
	and al, 0x1
	in al, 0x60
	
	mov rsp, rbp
	pop rbp
	ret
