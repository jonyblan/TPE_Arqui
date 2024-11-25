GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq12Handler
GLOBAL _irq80Handler

GLOBAL _exception0Handler   ;div por 0
GLOBAL _exception6Handler   ;invalid opcode
GLOBAL _printRegisters

EXTERN irqDispatcher
EXTERN syscallDispatcher
EXTERN exceptionDispatcher
EXTERN printRegisters

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
    catchRegisters
	mov rdi, %1 ; pasaje de parametro
	mov rsi, regs ; array de registros
	call exceptionDispatcher
	iretq
%endmacro

%macro catchRegisters 0
    mov [regs + 8], rax
    mov rax, [rsp]      ;rip
    mov [regs], rax
    mov [regs + 16], rbx
    mov [regs + 24], rcx
    mov [regs + 32], rdx
    mov [regs + 40], rsi
    mov [regs + 48], rdi
    mov [regs + 56], rbp
    mov rax, [rsp + 24] ;cs
    mov [regs + 64], rax
    mov [regs + 72], r8
    mov [regs + 80], r9
    mov [regs + 88], r10
    mov [regs + 96], r11
    mov [regs + 104], r12
    mov [regs + 112], r13
    mov [regs + 120], r14
    mov [regs + 128], r15
    mov rax, [rsp + 16] ;flags
    mov [regs + 136], rax
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    ret

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    ret

_irq80Handler:
    ;los registros como parametros de C son del orden:
    ;RDI RSI RDX RCX R8 R9 (usamos 5)
	mov r8, r10
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, rax
	call syscallDispatcher
	iretq

_irq12Handler:
	irqHandlerMaster 12

;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
    push rax
    in al, 0x60
    cmp al, 0x1D
    pop rax
    je _printRegisters
    irqHandlerMaster 1


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
    exceptionHandler 6

;Catch registers values
_printRegisters:
    catchRegisters
    pushState
    mov rdi, regs
    call printRegisters
    ; signal pic EOI (End of Interrupt)
    mov al, 20h
    out 20h, al
    popState
    iretq

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1
    regs resq 18