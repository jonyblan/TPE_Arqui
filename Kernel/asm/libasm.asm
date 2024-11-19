GLOBAL cpuVendor
GLOBAL getInput
GLOBAL getSecs
GLOBAL getMins
GLOBAL getHours
GLOBAL getDay
GLOBAL getMonth
GLOBAL getYear
GLOBAL outb
GLOBAL inb

section .text

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
    xor rax, rax ;limpio rax
    in al, 0x60
    ret

 ;formato RTC:
 ;Register  Contents            Range
 ; 0x00      Seconds             0–59
 ; 0x02      Minutes             0–59
 ; 0x04      Hours               0–23 in 24-hour mode,
 ;                               1–12 in 12-hour mode, highest bit set if pm
 ; 0x06      Weekday             1–7, Sunday = 1
 ; 0x07      Day of Month        1–31
 ; 0x08      Month               1–12
 ; 0x09      Year                0–99
 ; 0x32      Century (maybe)     19–20?
 ; 0x0A      Status Register A
 ; 0x0B      Status Register B
 ; recuperado de https://wiki.osdev.org/CMOS#Getting_Current_Date_and_Time_from_RTC
getSecs:
    ;seteo bits de status B: el primero para 24h, el segundo para binario
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 2
    or al, 4
    out 71h, al

    ;get
    mov al, 0x00
    out 70h, al
    in al, 71h

    sti
    ret
getMins:
    ;seteo bits de status B: el primero para 24h, el segundo para binario
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 2
    or al, 4
    out 71h, al

    ;get
    mov al, 0x02
    out 70h, al
    in al, 71h

    sti
    ret
getHours:
    ;seteo bits de status B: el primero para 24h, el segundo para binario
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 2
    or al, 4
    out 71h, al

    ;get
    mov al, 0x04
    out 70h, al
    in al, 71h

    sti
    ret
getDay:
    ;seteo bits de status B: el primero para 24h, el segundo para binario
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 2
    or al, 4
    out 71h, al

    ;get
    mov al, 0x07
    out 70h, al
    in al, 71h

    sti
    ret
getMonth:
    ;seteo bits de status B: el primero para 24h, el segundo para binario
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 2
    or al, 4
    out 71h, al

    ;get
    mov al, 0x08
    out 70h, al
    in al, 71h

    sti
    ret
getYear:
    ;seteo bits de status B: el primero para 24h, el segundo para binario
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 2
    or al, 4
    out 71h, al

    ;get
    mov al, 0x09
    out 70h, al
    in al, 71h

    sti
    ret
outb:
	mov rdx, rdi
	mov rax, rsi
	out dx, al
	ret

inb:
	mov rdx, rdi
	in al, dx
	ret