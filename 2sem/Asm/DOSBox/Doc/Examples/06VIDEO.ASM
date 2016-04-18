;-----------------------------------------------------------------------------
;                   Задание # 6. Файл 06video.asm
;     Вывод "Hello!" в (15,20). Причем 'Hel' цветом 4eh, а 'lo' - 2eh.
;                 (c) 2001 Copyleft  Andreev Andrey
;-----------------------------------------------------------------------------

.model tiny

;-----------------------------------------------------------------------------

x		= 15
y		= 20
videoseg	= 0b800h
color1		= 4eh
color2		= 2eh

;-----------------------------------------------------------------------------
.code
org 100h
start:		mov ax, videoseg
		mov es, ax
		mov ah, color1
		mov si, offset msg
		mov di, (y * 80 + x) * 2

again:		lodsb		        ; mov al, [si]
		cmp al, '$'
		je  done
			
		cmp al, '~'		; команда смены цвета
		je  changecolor
		stosw           	; mov es:[di], ax / inc di
		jmp again

changecolor:	lodsb
		mov ah, al
		jmp again 

done:		mov ax, 4c00h
		int 21h

.data
msg 		db 'Hel~', 2eh, 'lo$'		

end start

;-----------------------------------------------------------------------------
;				Конец...
;-----------------------------------------------------------------------------