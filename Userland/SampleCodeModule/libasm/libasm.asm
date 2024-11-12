GLOBAL sys_exit
GLOBAL sysReadChar
GLOBAL sysWrite
GLOBAL sys_execve
GLOBAL sys_time
GLOBAL callNewLine
GLOBAL callPutPixel

section .text

%macro syscallManager 1
    push rbp
    mov rbp, rsp

    mov r10, rcx    ;en C, el cuarto parametro corresponde a rcx, pero en asm es r10

    mov rax, %1 ;syscall id
    int 80h

    mov rsp, rbp
    pop rbp
    ret
%endmacro

sys_exit:
    syscallManager 1

sysReadChar:
    syscallManager 3

sysWrite:
    syscallManager 4

sys_execve:
    syscallManager 11

sys_time:
    syscallManager 13

; funciones nuevas no deben incluir el prefijo sys_ y deben comenzar de 191 en adelante

callNewLine:
    syscallManager 191

callPutPixel:
    syscallManager 192