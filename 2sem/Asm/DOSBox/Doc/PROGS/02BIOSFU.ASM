	.model tiny
	.code
	org 100h

start:	mov ah, 0eh
	mov bx, offset piu
next:	mov al, [bx]
	cmp al, '$'
	je enough
	int 10h
	inc bx
	jmp next

enough:	mov ax, 4c00h
	int 21h

piu:	db 'Hey!$'
end start 