GLOBAL writeInt
GLOBAL readInt
GLOBAL printDateTimeInt
GLOBAL getTicksInt

writeInt:
	push rbp
	mov rbp, rsp
	
	mov rax, 0x01
	int 80h

	mov rsp, rbp
	pop rbp
	ret

readInt:
	push rbp
	mov rbp, rsp
	
	mov rax, 0x00
	int 80h

	mov rsp, rbp
	pop rbp
	ret

printDateTimeInt:
	push rbp
	mov rbp, rsp
	
	mov rax, 0x02
	int 80h

	mov rsp, rbp
	pop rbp
	ret

getTicksInt:
	push rbp
	mov rbp, rsp
	
	mov rax, 0x03
	int 80h

	mov rsp, rbp
	pop rbp
	ret