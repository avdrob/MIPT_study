	.model tiny
	.code
	org 100h

start:	mov ax, 65535
	xor dx, dx
	mov cx, 000ah
	mov di, sp
	
dig:	cmp ax, 0000h
	je enough
	div cx
	push dx
	xor dx, dx
	jmp dig		
	
enough:	mov ah, 02h

outp:	cmp sp, di
	je stop
	pop dx
	add dx, 30h
	int 21h
	jmp outp 																			
	
stop:	mov ax, 4c00h
	int 21h

end start