;=============================================================================
;                 Задание # 25. Файл 25graph.asm
;               Пример работы в графическом режиме
;                Copyright (c) 2002 Andreev Andrey
;=============================================================================

.model tiny

locals  @@

color		= 04h
x1		= 50
y1		= 60
x2		= 100
y2		= 100

.code
org 100h

start:		mov ah, 0	; video mode 320x200x256
		mov al, 13h
		int 10h

		mov ax, 0a000h
		mov es, ax
		mov al, color

                mov di, y1*320+x1
                mov cx, x2 - x1
                call draw_hline

                mov cx, y2 - y1
                call draw_vline
		
		mov cx, x2 - x1 + 1
		mov di, y2*320+x1
		call draw_hline

		mov cx, y2 - y1
		mov di, y1*320+x1
		call draw_vline

		mov ax, 4c00h	; exit
		int 21h

;--Draw Hor Line procedure------------------------------------------------;
; entry:        al - color, cx - line lenght, di - start address.         ;
; assumes:      video mode, es = 0a000h                                   ;
; destr:        cx, di.                                                   ;
; exit:         none.                                                     ;
;-------------------------------------------------------------------------;
draw_hline      proc

                rep stosb

                ret
		endp
;--End of Draw Hor Line procedure-----------------------------------------;

;--Draw Ver Line procedure------------------------------------------------;
; entry:        al - color, cx - line lenght, di - start address.         ;
; assumes:      video mode, es = 0a000h.                                  ;
; destr:        cx, ah, bl.                                               ;
; exit:         none.                                                     ;
;-------------------------------------------------------------------------;
draw_vline      proc

@@next_pixel:   mov es:[di], al
                add di, 320
                dec cx
                cmp cx, 0
                jne @@next_pixel

		ret
		endp
;--End of Draw Ver Line procedure-----------------------------------------;


end start
