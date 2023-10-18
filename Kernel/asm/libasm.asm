GLOBAL cpuVendor
GLOBAL get_seconds
GLOBAL get_minutes
GLOBAL get_hours
GLOBAL get_weekDay
GLOBAL get_monthDay
GLOBAL get_month
GLOBAL get_year
GLOBAL getKey


;AQUI VAN TODAS LAS FUNCIONES DE ASSEMBLER

section .text

;macro para armar stack
%macro enter_func 0
	push rbp
	mov rbp, rsp
%endmacro

;macro para desarmar stack
%macro leave_func 0
	mov rsp, rbp
	pop rbp
	ret
%endmacro


;macro para obtener el RTC - Real Time Clock and Memory (ports 70h & 71h)
%macro get_RTC_val 1
	;limpio los registros
	xor rax, rax
	xor rdi, rdi
	;mando al RTC el acceso al registro B
	mov al, 0x0B
	out 70h, al
	;obtengo en al el registro B, que es el status
	in al, 71h 
	;modifico el bit numero 2 arrancando de derecha a izquierda siendo el de mas a la derecha el bit 0
	;el bit numero 2 lo pongo en 1 y dejo el resto igual
	or al, 0x04 
	;pongo este nuevo status en el seteo del RTC
	;ahora me va a devolver lo pedido en formato de bits y no hexa
	out 71h, al
	;pongo el valor a buscar en al y lo obtengo en al
	mov al, %1
	out 70h, al
	in al, 71h
	;paso el valor de al, que esta en rax, a rdi
	;en 64bits se devuelve en rdi
	mov rdi, rax
%endmacro

;las siguientes funciones usan macros para
;armar stack
;obtener el valor de RTC segun el parametro pasado
;desarmar el stack
get_seconds:
	enter_func
	get_RTC_val 0
	leave_func

get_minutes:
	enter_func
	get_RTC_val 2
	leave_func

get_hours:
	enter_func
	get_RTC_val 4
	leave_func

get_weekDay:
	enter_func
	get_RTC_val 6
	leave_func

get_monthDay:
	enter_func
	get_RTC_val 7
	leave_func

get_month:
	enter_func
	get_RTC_val 8
	leave_func

get_year:
	enter_func
	get_RTC_val 9
	leave_func

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


getKey:
  enter_func
   mov rax, 0
   in al, 0x60       ; lee la TECLA PRESIONADA desde el puerto 60h
  leave_func
