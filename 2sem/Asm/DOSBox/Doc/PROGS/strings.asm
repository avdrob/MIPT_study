	.model tiny

	.data	
		beep db '0'
		tyc db '1'
	.code
	org 100h
	
start:

	mov ax, 4c00h
	int 21h
	
;==========================================
;	Counts string length.
;	Entry: ES:[DI] -> string.
;	Exit: CX = string length.
;		  AL = '$'.
;	Destr:	none.
;==========================================
strlen:
	mov ah, 00h
	mov al, '$'
	mov cx, 0ffffh
	repne scasb
	neg cx
	sub cx, 2
ret
;==========================================

;==========================================
;	Copies a string into another
;		memory block.
;	Entry: DS:[SI] -> string.
;		   ES:[DI] -> block should be
;				filled with the string.
;	Exit: DS:[SI] -> the byte after the last 
;						byte of src.
;		  ES:[DI] -> the byte after the last 
;						byte of dest.
;		  AL = '$'
;	Destr: CX = 0.
;==========================================
strcpy:
	push si
	xchg si, di
	push es
	push ds
	pop ds
	pop es
	call strlen
	
	push es
	push ds
	pop ds
	pop es
	pop di
	xchg si, di
	
	repne movsb
ret
;==========================================


;==========================================
;	Fills in block of CX bytes at ES:[DI]
;		with AL.
;	Entry: ES:[DI] -> the block should be
;				filled in.
;		   AL = the value byte.
;		   CX = number of bytes.
;	Exit: ES:[DI] -> the byte after the last
;						byte of the block.
;	Destr: CX = 0.
;==========================================
memset:
	rep stosb 
ret
;==========================================


;==========================================
;	Compares 2 memory blocks of 
;		particular size.
;	Entry: DS:[SI] -> the 1st one.
;		   ES:[DI] -> the 2nd one.
;		   CX = blocks size.	
;	Exit:
;		CL = 01h <=> 1st > 2nd.
;		CL = 00h <=> 1st = 2nd.
;		CL = FFh <=> 1st < 2nd.
;
;		DS:[SI] -> the last byte after the
;			1st block's end.
;		ES:[DI} -> the last byte after the 
;			2nd block's end.
;
;	Destr: none.
;==========================================
memcmp:
	repe cmpsb
	
	ja above
	je equal
	jb below
	
	above:
		mov cl, 01h
	ret
	
	equal:
		mov cl, 00h
	ret
	
	below:
		mov cl, -1h
	ret
;==========================================

end start