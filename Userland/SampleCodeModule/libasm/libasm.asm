GLOBAL writeInt
GLOBAL readInt
GLOBAL printDateTimeInt
GLOBAL getTicksInt

GLOBAL sys_exit
GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_execve
GLOBAL sys_time

section .text

%macro syscallManager 1
    push rbp
    mov rbp, rsp

    mov rax, %1 ;syscall id (linux)
    int 80h

    mov rsp, rbp
    pop rbp
    ret
%endmacro

sys_exit:
    syscallManager 1

sys_read:
    syscallManager 3

sys_write:
    syscallManager 4

sys_execve:
    syscallManager 11

sys_time:
    syscallManager 13

; funciones nuevas no deben incluir el prefijo sys_ y deben comenzar de 191 en adelante

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