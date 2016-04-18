;-----------------------------------------------------------------------------
;                 Задание # 14. Файл 14shwreg.asm
;    Вывод содержимого регов, используя библиотеку 13lib.asm
;                (c) 2001 Copyleft  Andreev Andrey
;-----------------------------------------------------------------------------

.model tiny

;-----------------------------------------------------------------------------
; Константы

string_color = 0eh
x1	     = 10
x2	     = 20
y1	     = 5
y2	     = 9

;-----------------------------------------------------------------------------
.code
org 100h

start:		mov ax, 0b800h  ; загрузка es'а (надо для процедур)
		mov es, ax
		
		call draw_frame	; вызов lib.asm'овской  процедуры 
		
		mov ax, 0f5h	; то, что надо вывести
		mov bx, 0a8h	;

; Вызов процедуры вывода содержимого рега AX

		push ax bx cx dx
		mov si, offset msg1
		mov di, (y1 * 80 + x2 - 2) * 2
		call show_ax	
		pop dx cx bx ax

; Вызов процедуры вывода содержимого рега BX

		push ax bx cx dx
		mov si, offset msg2
		mov di, ((y1 + 1) * 80 + x2 - 2) * 2
		xchg ax, bx
		call show_ax
		pop dx cx bx ax

; Вызов процедуры вывода содержимого рега CX

		push ax bx cx dx
		mov si, offset msg3
                mov di, ((y1 + 2) * 80 + x2 - 2) * 2
                xchg ax, cx
		call show_ax
		pop dx cx bx ax

; Вызов процедуры вывода содержимого рега DX

		push ax bx cx dx
		mov di, ((y1 + 3) * 80 + x2 - 2) * 2
		xchg ax, dx
		call show_ax
		pop dx cx bx ax

; print 'ax = '

		cld
		push ax di si
		mov si, offset msg1
		mov di, (y1 * 80 + x1 + 1) * 2
		call print_str

		mov si, offset msg2
		mov di, ((y1 + 1) * 80 + x1 + 1) * 2
		call print_str

		mov si, offset msg3
		mov di, ((y1 + 2) * 80 + x1 + 1) * 2
		call print_str

		mov si, offset msg4
		mov di, ((y1 + 3) * 80 + x1 + 1) * 2
		call print_str
		push si di ax

		mov ax, 4c00h	; выход
		int 21h

include 13lib.asm			; подключение библиотеки

;----------------------------------------------------------------------------
.data
msg1		db 'AX = $'
msg2		db 'BX = $'
msg3		db 'CX = $'
msg4		db 'DX = $'

end start

;----------------------------------------------------------------------------
; 			Конец...
;----------------------------------------------------------------------------
