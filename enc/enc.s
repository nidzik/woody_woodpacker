BITS 64

global encrypt

; encrypt(char *key, char *value)
;				rdi,	rsi

; keys are in xmm6 to xmm15

key_expansion_128:
	pshufd xmm2, xmm2, 0xff ; "shuffle packed double word"
	vpslldq xmm3, xmm1, 0x4 ; "shift double quadword left logical"
	pxor xmm1, xmm3
	vpslldq xmm3, xmm1, 0x4
	pxor xmm1, xmm3
	vpslldq xmm3, xmm1, 0x4
	pxor xmm1, xmm3
	pxor xmm1, xmm2
	ret

encrypt:
	push rbx
	movdqu xmm1, [rdi] ; move key in xmm1
	aeskeygenassist xmm5, xmm1, 0x1 ; round 1
	call key_expansion_128
	movdqu xmm6, xmm1
	aeskeygenassist xmm5, xmm1, 0x2 ; round 2
	call key_expansion_128
	movdqu xmm7, xmm1
	aeskeygenassist xmm5, xmm1, 0x4 ; round 3
	call key_expansion_128
	movdqu xmm8, xmm1
	aeskeygenassist xmm5, xmm1, 0x8 ; round 4
	call key_expansion_128
	movdqu xmm9, xmm1
	aeskeygenassist xmm5, xmm1, 0x10 ; round 5
	call key_expansion_128
	movdqu xmm10, xmm1
	aeskeygenassist xmm5, xmm1, 0x20 ; round 6
	call key_expansion_128
	movdqu xmm11, xmm1
	aeskeygenassist xmm5, xmm1, 0x40 ; round 7
	call key_expansion_128
	movdqu xmm12, xmm1
	aeskeygenassist xmm5, xmm1, 0x80 ; round 8
	call key_expansion_128
	movdqu xmm13, xmm1
	aeskeygenassist xmm5, xmm1, 0x1b ; round 9
	call key_expansion_128
	movdqu xmm14, xmm1
	aeskeygenassist xmm5, xmm1, 0x36 ; round 10
	call key_expansion_128
	movdqu xmm15, xmm1
	movdqu xmm1, [rsi]
	aesenc xmm1, xmm6
	aesenc xmm1, xmm7
	aesenc xmm1, xmm8
	aesenc xmm1, xmm9
	aesenc xmm1, xmm10
	aesenc xmm1, xmm11
	aesenc xmm1, xmm12
	aesenc xmm1, xmm13
	aesenc xmm1, xmm14
	aesenclast xmm1, xmm15
	; generate keys with aeskeygenassist
	; 9 rounds of aesenc
	; 1 last round of aesenclast
	movdqu [rsi], xmm1
	pop rbx
	ret
