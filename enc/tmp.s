_set_xmm:
	lea rdi, [rel Key_Schedule]
	movdqu xmm0, [rdi]
	movdqu xmm1, [rdi+0x10]
	movdqu xmm2, [rdi+0x20]
	movdqu xmm3, [rdi+0x30]
	movdqu xmm4, [rdi+0x40]
	movdqu xmm5, [rdi+0x50]
	movdqu xmm6, [rdi+0x60]
	movdqu xmm7, [rdi+0x70]
	movdqu xmm8, [rdi+0x80]
	movdqu xmm9, [rdi+0x90]
	movdqu xmm10, [rdi+0xa0]


	aeskeygenassist xmm2, xmm1, 0x1
	call key_expand_key
	aeskeygenassist xmm2, xmm1, 0x2
	call key_expand_key 
	aeskeygenassist xmm2, xmm1, 0x4 
	call key_expand_key 
	aeskeygenassist xmm2, xmm1, 0x8 
	call key_expand_key 
	aeskeygenassist xmm2, xmm1, 0x10 
	call key_expand_key
	aeskeygenassist xmm2, xmm1, 0x20 
	call key_expand_key 
	aeskeygenassist xmm2, xmm1, 0x40
	call key_expand_key
	aeskeygenassist xmm2, xmm1, 0x80 
	call key_expand_key 
	aeskeygenassist xmm2, xmm1, 0x1b 
	call key_expand_key 
	aeskeygenassist xmm2, xmm1, 0x36
	call key_expand_key
	call _set_xmm
	movdqu xmm15, [rsi]
	pxor xmm15, 	  xmm0
	aesenc xmm15,     xmm1
	aesenc xmm15,     xmm2
	aesenc xmm15,     xmm3
	aesenc xmm15,     xmm4
	aesenc xmm15,     xmm5
	aesenc xmm15,     xmm6
	aesenc xmm15,     xmm7
	aesenc xmm15,     xmm8
	aesenc xmm15,     xmm9
	aesenclast xmm15, xmm10
	movdqu [rsi], xmm15