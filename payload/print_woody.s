BITS 64;
global main

main:
	jmp get_string ; jmp-call-pop tips

print:
	mov rax, 0x1 ; sysscall write
	mov rdi, 0x1 ; stdout
	pop rsi ; cette instruction n'utilise pas de null-byte non plus ! :)
	mov rdx, 0xe ; 13 length of woody
	syscall
	jmp end

get_string:
	call print
	db `....WOODY....\n`
end:
	xor rax, rax
	xor rdi, rdi
	xor rdx, rdx
	xor rsi, rsi
