;-----------------------------------------------------------------------------
;                 Задание # 11. Файл 11ax2hex.asm
;   Вывести содержимое рега ах в hex'е с помощью строки символов
;                (c) 2001 Copyleft  Andreev Andrey
;---------------------------------------------------------------------------  
.model tiny

.code
org 100h

start:		mov bx, 0b800h
		mov es, bx
		mov di, 16*2
		std
		
		mov dl, 10h
		mov ax, 0f5h

                mov bx, offset str

next:		div dl
		mov dh, al
                mov al, ah
                xlat
                mov ah, 4eh
                stosw
                mov al, dh
                mov ah, 0
                cmp al, 0
                jnz next
		
		mov ax, 4c00h
		int 21h
		
str 		db '0123456789ABCDEF'

end start
;----------------------------------------------------------------------------
;			Конец...
;----------------------------------------------------------------------------