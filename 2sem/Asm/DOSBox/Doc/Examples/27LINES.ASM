;=============================================================================
;                 Задание # 27. Файл 27lines.asm
;               Пример работы в графическом режиме
;                Copyright (c) 2002 Andreev Andrey
;=============================================================================

.model tiny

color   = 4
x       = 160
y       = 100

; y = 5 * x + 3
k	= 5
b	= 3

.code
org 100h

start:          mov ax, 0013h   ; set grafic mode 13 - 320x200x16
                int 10h

                mov ax, 0a000h  ; video segment
                mov es, ax

                mov bx, x	; center of screen
                mov dx, y
                mov al, color
                call out_pix

		mov di, 320*100	; horiz line
		mov cx, 320
		call draw_hline

		mov di, 160	; vert line
		mov cx, 200
		call draw_vline

		mov bx, x
		mov cx, k
		mov dx, b
		call get_function
		mov ax, color
		call out_pix

                mov ax, 4c00h	; exit
                int 21h
;-------------------------------------------------------------------------;
; entry:	bx - x, ax - k, cx - b.
; exit:		dx = bx * ax + cx.
; destr:	ax, dx(?).
;-------------------------------------------------------------------------;
get_function	proc

		mul bx
		add ax, cx
		mov dx, ax

		ret
		endp
;-------------------------------------------------------------------------;

;--Output Pixel procedure-------------------------------------------------;
; entry:        bx - x, dx - y, al - color.                               ;
; destr:        di.                                                       ;
; assumes:      es = 0a000h.                                              ;
;-------------------------------------------------------------------------;
out_pix         proc

                push ax bx
                mov ax, dx
                mov bx, 320d
                mul bx
                pop bx
                add ax, bx
                mov di, ax
                pop ax
                stosb

                ret
                endp
;--End of out pixel procedure---------------------------------------------;

include 	27lib.asm

end start