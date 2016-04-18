;=============================================================================
;                  Задание # 27. Файл 27lib.asm
;          Пример работы в графическом режиме (библиотека)
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
