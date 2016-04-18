.model tiny
.data
	tada db '%d', 00h
	
	dcm = 0ah
	bin = 01h
	hxd = 04h
		
	hex_num:
		db '0123456789ABCDEF'
.code
	org 100h

start:
	mov bx, 6542
	push bx
	mov bx, offset tada
	push bx
	push ds
	call printf
	
	mov ax, 4c00h
	int 21h
	
	
	
;===============================================
;	Prints a string with parameters.
;
;	Format: 'abc %par def', 00h
;		par = 'd' - print decimal number;
;		par = 'x' - print hex number;
;		par = 'b' - print binary number;
;		par = 's' - print string;
;		par = 'c' - print symbol;
;		par = '%' - print '%'
;
;	Entry: 
;		pushl arg1
;		pushl arg2
;		...
;		pushl arg n
;		pushl string offset
;
;		Here's n = number of parameters.
;		args are numbers to print themselves
;			or ASCII codes.
;		In case of %s it must be:
;			push string_offset
;			push string_segment
;		
;	Exit: ES:[DI] -> last symbol (00h) of 
;			the string.
;
;	Destr: AH, DL, SI
;		By secondary functions:
;			AX, DX, CX
;===============================================
printf:
	push bp
	mov bp, sp
	sub sp, 2
	
	mov es, [bp + 4]
	mov di, [bp + 6]
	mov [bp - 2], word ptr 8
	
outp:
	mov dl, es:[di]
	
	mov ah, '%'
	cmp ah, dl
	je id
	
	mov ah, 00h
	cmp ah, dl
	je exit
	
	mov ah, 02h
	int 21h
	inc di
	jmp outp
	
	exit:
	add sp, 2
	pop bp
	
ret
	
	id:
		inc di
		mov dl, es:[di]
		
		mov ah, 's'
		cmp ah, dl
		je strng
		
		mov ah, 'c'
		cmp ah, dl
		je chr
		
		mov ah, '%'
		cmp ah, dl
		je perc
		
		mov ah, 'x'
		cmp ah, dl
		je hex
		
		mov ah, 'b'
		cmp ah, dl
		je bnr
		
		mov ah, 'd'
		cmp ah, dl
		je dcml

		
	dcml:
		mov si, [bp - 2]
		push [bp + si]
		call getd
		add sp, 2
		
		inc di
		add [bp - 2], word ptr 2
		jmp outp
		
	bnr:
		mov si, [bp - 2]
		push [bp + si]
		call getb
		add sp, 2
		
		inc di
		add [bp - 2], word ptr 2
		jmp outp
		
	hex:	
		push di
		
		mov si, [bp - 2]
		push [bp + si]
		call geth
		add sp, 2
		pop di
		
		inc di
		add [bp - 2], word ptr 2
		jmp outp
		
	perc:
		mov ah, 02h
		int 21h
		inc di
		jmp outp
		
	chr:
		mov si, [bp - 2]
		
		mov ah, 02h
		mov dl, [bp + si]
		int 21h
		
		inc di
		add [bp - 2], word ptr 2
		jmp outp
		
	strng:
		push di
		push es
		
		mov si, [bp - 2]
		push [bp + si + 2]
		push [bp + si]
		call prtstr
		add sp, 4
		pop es
		pop di
		
		inc di
		add [bp - 2], word ptr 4
		jmp outp
;===============================================


		;===============================================
		;	Prints a string StrSeg:[Str_Offset] ending
		;		with 00h.
		;
		;	Entry:
		;		push Str_Offset
		;		push Str_Seg
		;
		;	Exit:
		;		ES:[DI] -> the last string's 
		;			symbol (00h);
		;		AH = DL = 00h.
		;
		;	Destr:
		;		none
		;===============================================
		prtstr:
			push bp
			mov bp, sp
			
			mov es, [bp + 4]
			mov di, [bp + 6]
			
			outch:
				mov dl, es:[di]
				
				mov ah, 00h
				cmp ah, dl
				je exit_prtstr
				
				mov ah, 02h
				int 21h
				inc di
				jmp outch
		
		exit_prtstr:
			pop bp
			
		ret
		;===============================================
		
		;===============================================
		;	Prints a hexademical number.
		;
		;	Entry: 
		;		push number.
		;
		;	Exit: BX = the number.
		;		  DI -> array of hex digits 
		;						('0' - 'F').
		;
		;	Destr: AX, CX, DX.
		;===============================================
		geth:
			push bp
			mov bp, sp
			
			mov bx, [bp + 4]
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
				
			pop bp	
		ret
		;===============================================
		
		;===============================================
		;	Prints a binary number.
		;
		;	Entry: 
		;		push number.
		;
		;	Exit: BX = the number.
		;
		;	Destr: AX, CX, DX.
		;===============================================
		getb:
			push bp
			mov bp, sp
			
			mov cl, 10h
			mov bx, [bp + 4]
			
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
				
			pop bp
		ret
		;===============================================
		
		;===============================================
		;	Prints a decimal number.
		;
		;	Entry: 
		;		push number.
		;
		;	Exit: BX = 0Ah;
		;		  CX = 0.
		;
		;	Destr: DX.
		;===============================================
		getd:
			push bp
			mov bp, sp
			
			mov ax, [bp + 4]
			xor cx, cx
			xor dx, dx
			mov bx, word ptr dcm
			
			get_num3:
				div bx
				
				push dx
				inc cx
				
				xor dx, dx
				cmp dl, al
				jne get_num3
			
			mov ah, 02h
			
			pop_out:
				pop dx
				add dl, '0'
				int 21h
				loop pop_out
			
			pop bp
		ret 
		;===============================================
			
end start