BITS 64

global decrypt

; decrypt(char *key, char *value)
;				rdi,	rsi

start:
	jmp decrypt


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

decrypt:
	push rbx
	movdqu xmm1, [rdi] ; move key in xmm0
	movdqu xmm0, xmm1 ; move key in xmm0
	movdqu xmm15, [rsi] ; move value in xmm15
	aeskeygenassist xmm2, xmm1, 0x1 ; generate key in xmm2
	call key_expansion_128
	movdqu xmm4, xmm1
	aeskeygenassist xmm2, xmm1, 0x2 ; round 2
	call key_expansion_128
	movdqu xmm5, xmm1
	aeskeygenassist xmm2, xmm1, 0x4 ; round 3
	call key_expansion_128
	movdqu xmm6, xmm1
	aeskeygenassist xmm2, xmm1, 0x8 ; round 4
	call key_expansion_128
	movdqu xmm7, xmm1
	aeskeygenassist xmm2, xmm1, 0x10 ; round 5
	call key_expansion_128
	movdqu xmm8, xmm1
	aeskeygenassist xmm2, xmm1, 0x20 ; round 6
	call key_expansion_128
	movdqu xmm9, xmm1
	aeskeygenassist xmm2, xmm1, 0x40 ; round 7
	call key_expansion_128
	movdqu xmm10, xmm1
	aeskeygenassist xmm2, xmm1, 0x80 ; round 8
	call key_expansion_128
	movdqu xmm11, xmm1
	aeskeygenassist xmm2, xmm1, 0x1b ; round 9
	call key_expansion_128
	movdqu xmm12, xmm1
	aeskeygenassist xmm2, xmm1, 0x36 ; round 10
	call key_expansion_128
	movdqu xmm13, xmm1
	pxor xmm15, xmm13
	aesdec xmm15, xmm12
	aesdec xmm15, xmm11
	aesdec xmm15, xmm10
	aesdec xmm15, xmm9
	aesdec xmm15, xmm8
	aesdec xmm15, xmm7
	aesdec xmm15, xmm6
	aesdec xmm15, xmm5
	aesdec xmm15, xmm4
	aesdeclast xmm15, xmm0
	movdqu [rsi], xmm15
	pop rbx
	ret
