	.model tiny
	.code
	org 100h
	
start:	mov ah, 09h
	mov dx, offset piu
	int 21h
	
	mov ax, 4c00h
	int 21h

piu:	db 'Hey!$'	
	
end start																																																																													