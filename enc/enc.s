BITS 64

%macro  genkey 1-2
	aeskeygenassist xmm2, xmm1, %1 ; round 2
	call key_expansion_128
	movdqu %2, xmm1
%endmacro

global encrypt

; encrypt(char *key, char *value, size_t len)
;				rdi,	rsi			rdx

start:
	jmp encrypt

key_expansion_128: ; expand key from xmm2
	pshufd xmm2, xmm2, 0xff ; "shuffle packed double word"
	vpslldq xmm3, xmm1, 0x4 ; "shift double quadword left logical"
	pxor xmm1, xmm3
	vpslldq xmm3, xmm1, 0x4
	pxor xmm1, xmm3
	vpslldq xmm3, xmm1, 0x4
	pxor xmm1, xmm3
	pxor xmm1, xmm2
	ret

; 0x36353433323139383736353433323130

encrypt:
	push rbx
	movdqu xmm1, [rdi] ; move key in xmm0
	movdqu xmm0, xmm1 ; move key in xmm0
	genkey 0x1, xmm4
	genkey 0x2, xmm5
	genkey 0x4, xmm6
	genkey 0x8, xmm7
	genkey 0x10, xmm8
	genkey 0x20, xmm9
	genkey 0x40, xmm10
	genkey 0x80, xmm11
	genkey 0x1b, xmm12
	genkey 0x36, xmm13
	; while on the size
	xor rdi, rdi
begin_loop:
	cmp rdx, rdi
	jle end
perform:
	movdqu xmm15, [rsi + rdi] ; move value in xmm15
	pxor xmm15, xmm0
	aesenc xmm15, xmm4
	aesenc xmm15, xmm5
	aesenc xmm15, xmm6
	aesenc xmm15, xmm7
	aesenc xmm15, xmm8
	aesenc xmm15, xmm9
	aesenc xmm15, xmm10
	aesenc xmm15, xmm11
	aesenc xmm15, xmm12
	aesenclast xmm15, xmm13
	movdqu [rsi + rdi], xmm15
	add rdi, 0x10
	jmp begin_loop
end:
	pop rbx
	ret
