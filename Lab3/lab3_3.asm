section .data

;AF_UNIX:  1
;SOC_STREAM: 1
;PROTOCOL: 0

section .text

global _start


_start:

	mov rax, 41 
	mov rdi, 2 ;AF_INET
	mov rsi, 1 ;SOC_STREAM
	mov rdx, 0
	syscall
	mov r8, rax	;stores fd of socket in r8
	
	mov qword [rsp-8], 0x0	;8 bytes of padding
	mov dword [rsp-12], 0x0100007f	;inet addr 127.0.0.1 (little endian)
	mov word [rsp-14], 0x5c11	;port 4444 (little endian)
	mov byte [rsp-16], 0x02	;AF_INET
	sub rsp, 16

	mov rax, 42
	mov rdi, r8 ;fd of socket
	mov rsi, rsp	;*uservaddr
	mov rdx, 16	;addrlen
	syscall
	
	mov rax, 60
	mov rdi, 0
	syscall
