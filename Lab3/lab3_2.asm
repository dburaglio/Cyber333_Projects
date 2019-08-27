section .data

filename: db "/bin/sh"

section .text

global _start

_start:
	
	mov rax, 59
	mov rdi, filename
	lea rsi, [rsp+8]
	lea rdx, [rsp+24] 
	syscall
	mov rax, 60
	mov rdi, 0
	syscall
