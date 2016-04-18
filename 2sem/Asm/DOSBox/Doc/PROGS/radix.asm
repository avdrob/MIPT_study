	.model tiny
	
	.data 
		dcm = 0ah
		bin = 01h
		hxd = 04h
		
		hex_num:
			db '0123456789ABCDEF'
	.code
	org 100h
	
start:
	call capt_num
	call nwln
	call geth
	call nwln
	call getb
	
	mov ax, 4c00h
	int 21h
	
;===================================
;	Inputs number, then prints it.
;	Entry: none
;	Exit: BX = the number itself.
;	Destr: AX, BX, DX
;===================================
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
;===================================

;===================================
;	Prints a new line.
;	Entry: none.
;	Exit: none.
;	Destr: AL, DL.
;===================================
nwln:	
	mov ah, 02h
	mov dl, 0ah
	int 21h
	mov dl, 0dh
	int 21h
ret
;===================================


;===================================
;	Prints a binary number.
;	Entry: BX = number to print.
;	Exit: BX = still the number.
;	Destr: AX, CX, DX.
;===================================
getb:
	mov cl, 10h
	
	get_num1:
		mov dx, bx
		dec cl
		shr dx, cl
		inc cl
		and dx, bin
		add dl, '0'
		mov ah, 02h
		int 21h
	
		mov ch, 0h
		cmp cl, ch
		dec cl
		jne get_num1
ret
;===================================

;===================================
;	Prints a hexademical number.
;	Entry: BX = number to print.
;	Exit: BX = still the number.
;		  DI -> array of hex digits 
;						('0' - 'F').
;	Destr: AX, CX, DX, DI.
;===================================
geth:
	mov cl, 04h
	mov di, offset hex_num
	
	get_num2:
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
		mov dl, byte ptr [di]
		pop di
		mov ah, 02h
		int 21h
		
		mov ch, 0h
		cmp cl, ch
		dec cl
		jne get_num2
ret
;===================================

end start