helloworld_shellcode:
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
    mov al, 4 ; utiliser al revient à ne pas laisser de null-bytes dans le shellcode
    mov bl, 1 ; bl et non ebx, pour les mêmes raisons que ci-dessus
    jmp helloworld_string ; ici intervient le jmp-call-pop.

helloworld_shellcode_next:
    pop ecx ; cette instruction n'utilise pas de null-byte non plus ! :)
    mov dl, 13
    int 0x80
    xor eax, eax ; eax = 0
    inc eax ; incrémente eax, donc eax = 1 
    xor ebx, ebx ; le code de retour sera zéro
    int 0x80 

helloworld_string:
    call helloworld_shellcode_next
    db `Hello world!\n`
