	.model tiny
	.code
	org 100h
	
start:	mov ah, 00h
		int 16h
	
		cmp al, 0dh
		je getb
	
		mov ah, 02h
		xchg al, dl
		int 21h
		xchg al, dl
	mov ah, 00h

	sub al, 30h
	xchg ax, bx
	mov dx, 000ah
	mul dx
	xor dx, dx
	add al, bl
	xchg ax, bx
	jmp start

getb:	call nwln
	mov ah, 02h
	mov cl, 10h
	mov ch, 1h
	
	getbw:	call getbc
		int 21h
		cmp ch, cl
		je next
		inc ch
		jmp getbw

	getbc:	mov di, bx

		mov dl, cl
		sub dl, ch
		xchg cl, dl
		shr di, cl
		xchg cl, dl
		and di, 0001h
		add di, 30h
		mov dx, di	
	ret		


next:	call nwln
	mov ah, 02h
	mov cl, 04h
	mov ch, 1h
	
gethw:	call gethc
	int 21h
	cmp ch, cl
	je stop
	inc ch
	jmp gethw

gethc:	mov dh, 0h
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


nwln:	mov ah, 02h
	mov dl, 0ah
	int 21h
	mov dl, 0dh
	int 21h
ret
			
stop:	mov ax, 4c00h
	int 21h

end start																												