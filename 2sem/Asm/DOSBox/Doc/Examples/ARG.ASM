;*************************************************
; ARG.ASM (c) Ded
;*************************************************
code segment
assume cs:code, ds:code, es:code
org 100h
;=================================================
arg proc far
start:  mov bx, 80h
	mov cx, [bx]
	mov ah, ':'
	call nextarg
	call nextarg
	call nextarg
	mov dx, si
	mov ah, 09h
	int 21h

	int 20h
arg endp
;-------------------------------------------------
	; Scans for next argument in cmd line
	; Entry: AH contains char to skip (separator)
	;        CX contains command line length
	; Exit:  AL contains 1st matched char
	;	 DS:SI points to this
	; Destroys: AL, CX, SI
nextarg proc
	mov si, cmd_ofs			; Cmd line offset
	cld
char:	lodsb				; AL <- cmd line byte
	cmp al, ah
	je cont                         ; Separator found
	cmp al, ' '
	je cont                         ; Space found
	cmp al, '	'
	je cont                         ; Tab found
	loop char
cont:   dec cx
blank:	lodsb				; AL <- cmd line byte
	cmp al, ah
	loope blank                     ; Skip defined chars
	cmp al, ' '
	loope blank			; Skip spaces
	cmp al, '	'
	loope blank			; Skip tabs
	dec si
	mov cmd_ofs, si
	ret

	cmd_ofs dw 0081h                ; Current cmd line offset
nextarg endp
;=================================================
code ends
;*************************************************
end start




.