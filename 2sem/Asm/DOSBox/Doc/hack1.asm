.model tiny
.data
	inp db 'inp.txt', 00h
	entr db 'Reading password from input file', 13, 10, 'Press any key...', 13, 10, '$'
	den db 'Permission denied$'
	acp db 'Accepted$'
	
.code
	org 100h
	
start:
	mov ah, 09h
	mov dx, offset entr
	int 21h
	
	call entrpsw
	
	mov ch, 00h
	cmp ch, cl
	je ok
	
	mov ah, 09h
	mov dx, offset den
	int 21h
	mov ax, 4c00h
	int 21h
	
	ok:
		mov ah, 09h
		mov dx, offset acp
		int 21h
		mov ax, 4c00h
		int 21h

	psw db 'Gleb,kushai hleb$'
		
entrpsw:
	push bp
	sub sp, 10h
	mov bp, sp
	
	;getch:
	;	mov ah, 00h
	;	int 16h
	;	cmp al, 0dh
	;	je enough
	;	mov byte ptr [bp + si], al
	;	inc si
	;	jmp getch
		
	mov ah, 00h										;
	int 16h											; "Press any key"
		
	mov ax, 3d00h									;
	mov dx, offset inp								; Open input file
	int 21h											; 
	
	mov dx, bp
	mov bx, ax
	mov cx, 1
getch:	
	mov ah, 3fh										; Read it 
	int 21h											; byte 
	cmp ax, cx										; by byte
	jne enough										;
	
	inc dx
	jmp getch
	
	enough:
		mov cx, offset psw
		push cx
		push cs
		call strlen
		
		push cx
		mov cx, offset psw
		push cx
		push cs
		push bp
		push ss
		call memcmp
		add sp, 14
		
	add sp, 10h
	pop bp
ret
				
strlen:	
	push bp
	mov bp, sp
	
	mov es, word ptr ss:[bp + 4]
	mov di, word ptr ss:[bp + 6]
	
	mov ah, 00h
	mov al, '$'
	mov cx, 0ffffh
	repne scasb
	neg cx
	sub cx, 2
	
	pop bp
ret
				
memcmp:
	push bp
	mov bp, sp
	
	mov es, ss:[bp + 4]
	mov di, ss:[bp + 6]
	mov ds, ss:[bp + 8]
	mov si, ss:[bp + 10]
	mov cx, ss:[bp + 12]
	
	cmp cx, 0000h
	je above
	
	repe cmpsb
	
	ja above
	je equal
	jb below
	
	above:
		mov cl, 01h
		pop bp
	ret
	
	equal:
		mov cl, 00h
		pop bp
	ret
	
	below:
		mov cl, 0ffh
		pop bp
	ret

end start
		