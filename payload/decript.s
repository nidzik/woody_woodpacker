BITS 64;
%define MPROTECT 0xa

;int mprotect(void *addr, size_t len, int prot);

_start:
	mov rax, MPROTECT //0xA is the # for the protect syscall
	mov rdi, 0x580 // address in %rdi. I hard coded address to start so i could see what it looked like when I dumped the bytes of the instruction
	mov rsi, " // size in %rsi
	mov rdx, 0x7 // permissions in %rdx (read/write/exec)
	syscall
