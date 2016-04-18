	.model tiny
	
	.data 
		dcm = 0ah
		hxd = 04h
		
		hex_num:
			db '0123456789ABCDEF'
	.code
	org 100h
	
start:
	call capt_num
	call nwln
	call geth
	
	mov ax, 4c00h
	int 21h
	
;============================================
capt_num:
	mov ah, 00h
	int 16h
	cmp al, 0dh
	je exit1
	
	mov ah, 02h
	xchg al, dl
	int 21h
	xchg al, dl
	
	mov ah, 00h
	sub al, '0'
	xchg ax, bx
	mov dx, dcm
	mul dx
	xor dx, dx
	add ax, bx
	xchg ax, bx
	jmp capt_num

exit1:
ret
;============================================

;============================================
nwln:	
	mov ah, 02h
	mov dl, 0ah
	int 21h
	mov dl, 0dh
	int 21h
ret
;============================================

;============================================
geth:
	mov cl, 04h
	mov di, offset hex_num
	
	get_num1:
		mov dx, bx
		mov al, hxd
		dec cl
		mul cl
		inc cl
		xchg al, cl
		shr dx, cl
		xchg al, cl
		and dx, 000fh
		
		push di
		add di, dx
		mov dl, byte ptr cs:[di]
		pop di
		mov ah, 02h
		int 21h
		
		mov ch, 0h
		cmp cl, ch
		dec cl
		jne get_num1
ret
;============================================

end start