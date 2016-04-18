	.model tiny 
	
	.data 
		pip db '5432$'
	
	.code
		org 100h

start:
	push offset pip
	push ds
	call strlen
	
	mov ax, 4c00h
	int 21h
	
;=================================================
;	Counts length of string <- SrcSeg:[SrcOffset].
;	Entry: push SrcOffset
;		   push SrcSeg
;	Exit: CX = string length.
;		  AL = '$'.
;	Destr:	none.
;=================================================
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
;=================================================


;=================================================
;	Copies a string SrcSeg:[SrcOffset] into
;					DestSeg:[DestOffset]
;	Entry: push SrcOffset
;		   push SrcSeg
;		   push DestOffset
;		   push DestSeg
;				
;	Exit: DS:[SI] -> the byte after the last 
;						byte of src.
;		  ES:[DI] -> the byte after the last 
;						byte of dest.
;		  AL = '$'
;	Destr: AX
;		   CX = 0.
;=================================================
strcpy:
	push bp
	mov bp, sp
	
	push ss:[bp + 10]
	push ss:[bp + 4]
	call strlen
	pop ax
	pop ax
	
	mov si, word ptr ss:[bp + 10]
	mov ds, word ptr ss:[bp + 8]
	mov di, word ptr ss:[bp + 6]
	repne movsb
	
	pop bp
ret
;=================================================

;=================================================
;	Fills in block <- DestSeg:[DestOffset]
;			with Num Val values.
;	Entry: push Num
;		   push Val
;		   push DestOffset
;		   push DestSeg
;	Exit: ES:[DI] -> the byte after the last
;						byte of the block.
;	Destr: CX = 0.
;=================================================
memset:
	push bp
	mov bp, sp
	
	mov es, ss:[bp + 4]
	mov di, ss:[bp + 6]
	mov al, byte ptr ss:[bp + 8]
	mov cx, ss:[bp + 10]
	rep stosb 
	
	pop bp
ret
;=================================================

;=================================================
;	Compares 2 memory blocks of 
;		particular size.
;	Entry: SrcSeg:[SrcOffset] -> the 1st one.
;		   DestSeg:[DestOffset] -> the 2nd one.
;		   Num = blocks size.	
;
;		   push Num
;		   push SrcOffset
;		   push SrcSeg
;		   push DestOffset
;		   push DestSeg
;
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
;=================================================
memcmp:
	push bp
	mov bp, sp
	
	mov es, ss:[bp + 4]
	mov di, ss:[bp + 6]
	mov ds, ss:[bp + 8]
	mov si, ss:[bp + 10]
	mov cx, ss:[bp + 12]
	
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
;=================================================	

end start