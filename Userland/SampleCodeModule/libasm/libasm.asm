GLOBAL sys_exit
GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_execve
GLOBAL sys_time

section .text

%macro syscallManager 1
    push rbp
    mov rbp, rsp
    push rbx

    mov r9, r8
    mov r8, rcx
    mov rcx, rsi
    mov rbx, rdi
    mov rax, %1 ;syscall id (linux)
    int 80h

    pop rbx
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