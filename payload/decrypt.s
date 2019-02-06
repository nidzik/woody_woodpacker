BITS 64
global main


main:
	;mov r13, 0xdeadbeef ; HARD CODED new entry point
	;mov r12, 0xdeadbeef ; HARD CODED text length
	;mov r11, 0xdeadbeef ; HARD CODED text offset
	mov r13, 0x830 ; HARD CODED new entry point
	mov r12, 0x1a2 ; HARD CODED text length
	mov r11, 0x530 ; HARD CODED text offset

	; get virt address
	lea rdi, [ rel main ]
	neg r13
	add rdi, r13 ; begining of the elf
	add rdi, r11 ; begining of the .text WORKING

	; align rdi for mprotect
	mov r10, rdi;
	and rdi, -0x1000;
	; get the alignement
	neg rdi
	add r10, rdi
	neg rdi
	add r12, r10 ; text length + alignement
	; mprotect
	mov rax, 0xa ; syscall mprotect 
	mov rsi, r12 ; length page_size maybe more next time ? HARD CODED
	mov rdx, 0x07 ; protection
	syscall

	; initialize for the loop
	add rdi, r10 ; come bactk to the start of .text
	mov r14, rdi ; buffer start
	neg r10
	add r12, r10 ; come back to the original r12, code length
	xor rcx, rcx ; set index to 0 

beginning:
	inc rcx ; inc index
	xor BYTE [r14], 0x42 ; xor on [r14]
	lea r14, [r14 + 1] ; inc our ptr
	cmp rcx, r12 ; cmp if we ave done enought HARD CODED
	jle  beginning ; no, keep doing
end: ; reset our variables
	xor rax, rax
	xor rdi, rdi
	xor rsi, rsi
	xor rdx, rdx
	xor r14, r14
	xor rcx, rcx
