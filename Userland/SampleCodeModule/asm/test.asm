GLOBAL test

section .text

test:
push rbp
mov rbp, rsp
push rdi
push rsi
push rdx
push rcx

mov rdi, 1
mov rsi, msg
mov rdx, len
mov rcx, 2
int 80h

pop rcx
pop rdx
pop rsi
pop rdi
mov rsp, rbp
pop rbp
ret


section .data
msg db "TEST123", 10, 0
len equ $-msg 