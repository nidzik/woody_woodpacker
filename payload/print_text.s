BITS 64							;
section     .text
	global _start
	
_start:							;tell linker entry point
	mov		rdi, 0x555555554500
	mov 	rsi, 4096
	mov		rdx, 0x07
	mov		rax, 0xa
	syscall

	mov 	r14, 0x555555554580
	xor		rcx, rcx
	cmp 	rcx, 0x1d2
	jg 		end
	beginning:
	inc 	rcx
	xor		BYTE [r14], 0x42
	lea		r14, [r14 + 1]
	cmp 	rcx, 0x1d2
	jle 	beginning
	end:

	
	


	mov	rdi, rax   		; error code 0	
	mov	rax, 60  		; use the _exit syscall
   	syscall         		; make syscall

	xor		BYTE [r14], 0x42
	mov 	rbx, r14
	lea		rbx, [rbx + 1]
	lea		r14, [r14 + 1]
	xor		BYTE [r14], 0x42


	xor		rax, rax

	

	

	mov		r10, 0x42
	mov		rcx, 0x00
	
	


