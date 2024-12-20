;GLOBAL sysExit
GLOBAL sysReadChar
GLOBAL sysWriteColor
GLOBAL sysTime
GLOBAL _hlt

GLOBAL callNewLine
GLOBAL callPutPixel
GLOBAL callClear
GLOBAL callScale
GLOBAL callPrintRegs
GLOBAL callPrintSystemTime
GLOBAL callTicksElapsed
GLOBAL beepSys

GLOBAL excDiv0
GLOBAL excInvalidOpCode

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

sysExit:
    syscallManager 1

sysReadChar:
    syscallManager 3

sysWriteColor:
    syscallManager 4

sysTime:
    syscallManager 13

; funciones nuevas no deben incluir el prefijo sys y deben comenzar de 191 en adelante

callNewLine:
    syscallManager 191

callPutPixel:
    syscallManager 192

callClear:
    syscallManager 193

callScale:
    syscallManager 194

callPrintRegs:
    syscallManager 195

callPrintSystemTime:
    syscallManager 196

callTicksElapsed:
    syscallManager 197

beepSys:
	syscallManager 6

; excepciones

excDiv0:
    mov rax, 1
    mov rbx, 0
    div rbx
    ret

excInvalidOpCode:
    mov cr6, rax
    ret

_hlt:
	sti
	hlt
	ret