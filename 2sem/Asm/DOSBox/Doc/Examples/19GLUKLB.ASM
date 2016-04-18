;=============================================================================
;                 Задание # 19. Файл 19gluklb.asm
;               Библиотека: Процедура печати строки
;                Copyright (c) 2002 Andreev Andrey
;=============================================================================

;---Print string procedure----------------------(c) Andreev Andrey, 2002--;
; entry:        si - offset msg; di - start adr to output; ah - color.    ;
; assume:       ds - data segment.                                        ;
; destr:        ax, es, di, si.                                           ;
;-------------------------------------------------------------------------;
print_str       proc
                push ax
                mov ax, 0b800h
                mov es, ax
                pop ax

next_print:     lodsb
                cmp al, '$'
                je done_print
                cmp al, 0dh     ; new line (0dh - scan-code of Enter-key)
                je new_line
                stosw
                jmp next_print

new_line:       push ax
                mov ax, di
                shr ax, 1
nl_loop:        sub ax, 80
                cmp ax, 80
                jg nl_loop
                add di, 80*2
                shl ax, 1
                sub di, ax
                add di, 2
                inc si
                pop ax
                jmp next_print

done_print:     ret
                endp
;---End of print string procedure-----------------------------------------;