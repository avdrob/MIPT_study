	.model tiny
	.code
	org 100h

start:
	mov ax, 0b800h
	mov es, ax
	
	mov ah, 03h
	mov bh, 00h
	int 10h
	
	mov al, 050h
	mul dh
	mov dh, 00h
	add ax, dx
	mov cx, 0002h
	mul cx
	mov di, ax

	mov bx, offset pip
	
next:	
	mov al, [bx]
	cmp al, '$'
	je stop

	mov es:[di], al
	mov byte ptr es:[di+1], 4eh
	inc bx
	add di, 2
	jmp next

stop:	
	mov ax, 4c00h
	int 21h

pip:	
	db 'Hey!$'

end start