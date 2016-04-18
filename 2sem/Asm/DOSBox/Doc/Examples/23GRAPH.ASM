;=============================================================================
;                 Задание # 23. Файл 23graph.asm
;               Пример работы в графическом режиме
;                Copyright (c) 2002 Andreev Andrey
;=============================================================================

.model tiny

locals  @@

start_x		= 50
start_y		= 10
y		= 60
end_x		= 100 

.code
org 100h

start:		mov ah, 0	; video mode 320x200x256
		mov al, 13h
		int 10h

		mov cx, start_x
		mov dx, y
		mov bl, end_x - start_x
		mov al, 4	; color
		mov bh, 0	; video page

                call draw_hline

		mov ax, 4c00h	; exit
		int 21h

;--Draw Hor Line procedure------------------------------------------------;
; entry:        al - color, bh - video page, cx - start_x, dx - y,        ;
;               bl - line lenght.                                         ;
; assumes:      video mode.                                               ;
; destr:        cx, ah, bl.                                               ;
; exit:         none.                                                     ;
;-------------------------------------------------------------------------;
draw_hline      proc
		mov ah, 0ch	; output pixel
@@next_pixel:   int 10h
		inc cx
		dec bl
		cmp bl, 0
                jne @@next_pixel
		ret
		endp
;--End of Draw Hor Line procedure-----------------------------------------;

;--Draw Ver Line procedure------------------------------------------------;
; entry:        al - color, bh - video page, cx - x, dx - start_y,        ;
;               bl - line lenght.                                         ;
; assumes:      video mode.                                               ;
; destr:        cx, ah, bl.                                               ;
; exit:         none.                                                     ;
;-------------------------------------------------------------------------;
draw_vline      proc
		mov ah, 0ch	; output pixel
@@next_pixel:   int 10h
                inc dx
		dec bl
		cmp bl, 0
                jne @@next_pixel
		ret
		endp
;--End of Draw Ver Line procedure-----------------------------------------;


end start
