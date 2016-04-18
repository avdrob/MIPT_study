	.model tiny
	.code
	org 100h

start:	mov bx, 4
	mov ah, 02h
	mov cl, 10h
	mov ch, 1h
	
getw:	call getc
	int 21h
	cmp ch, cl
	je stop
	inc ch
	jmp getw

getc:	mov di, bx

	mov dl, cl
	sub dl, ch
	xchg cl, dl
	shr di, cl
	xchg cl, dl
	and di, 0001h
	add di, 30h
	mov dx, di
ret			

stop:	mov ax, 4c00h
	int 21h	

end	start							