	.model tiny
	.code
	org 100h

start:	mov bx, 4729
	mov ah, 02h
	mov cl, 04h
	mov ch, 1h
	
getw:	call getc
	int 21h
	cmp ch, cl
	je stop
	inc ch
	jmp getw

getc:	mov dh, 0h
	mov di, bx

	sha:	mov dl, cl
		sub dl, ch
		cmp dh, dl
		je enough
		shr di, cl
		inc dh
		jmp sha
	
	enough:	and di, 000Fh
		add di, 30h
		cmp di, '9'
		jb okc
		je okc
		add di, 07h
		jmp okc
	
	okc:	mov dx, byte ptr di
ret			

stop:	mov ax, 4c00h
	int 21h	

end	start							