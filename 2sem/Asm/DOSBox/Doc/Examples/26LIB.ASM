;=============================================================================
;                 Задание # 26. Файл 26lib.asm
;         Пример работы в графическом режиме (библиотека)
;                Copyright (c) 2002 Andreev Andrey
;=============================================================================

locals  @@
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

;--Output pixel procedure-------------------------------------------------;
; entry:        dl - x, dh - y, al - color.                               ;
; exit:         di = dh * 320 + dl.                                       ;
;-------------------------------------------------------------------------;
output_pixel	proc

		push ax bx
		mov di, 320
		mov al, dh
		mul di
		mov bl, dl
		mov bh, 0
		mov di, bx
		add di, ax
		pop bx ax
	
		stosb

		ret
		endp
;--End of output pixel procedure------------------------------------------;
