GLOBAL call_sys_read
GLOBAL call_sys_write
GLOBAL call_timeClock
GLOBAL call_regState
GLOBAL call_zoomOut
GLOBAL call_zoomIn
GLOBAL call_sleepms
GLOBAL call_clear
GLOBAL call_cursor
GLOBAL call_delete_cursor
GLOBAL call_clear_screen
GLOBAL call_getBuffPosition
GLOBAL call_getLastKey
GLOBAL call_set_font_size

section .text

%macro call_to_handler 1
    push rbp
    mov rbp, rsp
    mov r9, r8      ;arg 5
    mov r8, rcx     ;arg 4
    mov rcx, rdx    ;arg 3
    mov rdx, rsi    ;arg 2
    mov rsi, rdi    ;arg 1
    mov rdi, %1     ;syscall deseada
    int 80h         ;interrupcion 80
    mov rsp, rbp
	pop rbp
    ret
%endmacro

call_sys_write:
    call_to_handler 1
call_sys_read:
    call_to_handler 2
call_timeClock:
    call_to_handler 3
call_regState:
    call_to_handler 4
call_zoomOut:
    call_to_handler 5
call_zoomIn:
    call_to_handler 6
call_sleepms:
    call_to_handler 7
call_clear:
    call_to_handler 8
call_cursor:
    call_to_handler 9
call_delete_cursor:
    call_to_handler 10
call_clear_screen:
    call_to_handler 11
call_getBuffPosition:
    call_to_handler 12
call_getLastKey:
    call_to_handler 13
call_set_font_size:
    call_to_handler 14