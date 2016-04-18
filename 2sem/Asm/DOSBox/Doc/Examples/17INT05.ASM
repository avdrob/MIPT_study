;=============================================================================
;                 Задание # 17. Файл 17int05.asm
;Простейшая резидентная программа, перехватывающая прерывание 05 (PrintScreeen)
;               Copyright (c) 2002 Andreev Andrey
;=============================================================================

;-----------------------------------------------(c) Andreev Andrey, 2002--;
; Occupy int05h and show message if PrintScreen pressed                   ;
;-------------------------------------------------------------------------;
.model tiny

.code
org 100h

start:          jmp main

msg             db 'Hello! I am resident program! You want PrintScreen? Sorry, but I dont like it...$'

new05           proc
                push es ax dx bx di

        ; вывод надписи при вызове
                mov ax, 0b800h
                mov es, ax

x               = 0
y               = 5
color           = 4eh

                mov ah, color
                mov bx, offset msg
                mov di, (y * 80d + x) * 2
again:          mov al, cs:[bx]
                cmp al, '$'
                je  done
                mov es:[di], ax
                add di, 0002d
                inc bx
                cmp al, '$'
                jne again

done:           pop di bx dx ax es
                iret
                endp

;----------------------------------------------------------------------------
main:           mov ax, 0               ; перехват прерывания 05h
                mov ds, ax
                mov bx, 05h*4
                mov [bx], offset new05  ; IntrTable [05h] = &new09
                mov ax, es
                mov [bx+2], es

                mov dx, offset main     ; оставление проги резидентом
                shr dx, 4
                inc dx
                mov ax, 3100h
                int 21h

end start
