GLOBAL writeInt

writeInt:
	push rbp
	mov rbp, rsp
	
	mov rax, 0x01
	int 80h

	mov rsp, rbp
	pop rbp
	ret