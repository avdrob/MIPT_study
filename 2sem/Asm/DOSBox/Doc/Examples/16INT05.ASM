;=============================================================================
;                 Задание # 16. Файл 16int05.asm
;Простейшая резидентная программа, перехватывающая прерывание 05 (PrintScreeen)
;               Copyright (c) 2002 Andreev Andrey
;=============================================================================

;-----------------------------------------------(c) Andreev Andrey, 2002--;
; Occupy interrupt 05h (PrintScreen)                                      ;
;-------------------------------------------------------------------------;
.model tiny

.code
org 100h

start:          jmp main
;--Resident part - new int05----------------------------------------------;
new05           proc
                push es ax

                mov ax, 0b800h
                mov es, ax
                mov es:0000h, 4e41h     ; red 'A' -> screen
                pop ax es

                db 0eah                 ; call last ISR
OldISR          dd 0bad0badh

                endp

;--Not resident part - occupy int05 with saving last ISR and make resident;
main:           .386

                mov ax, 0               ; OldISR = ofs & seg INT05h
                mov ds, ax
                mov si, 05h*4
                mov ax, cs
                mov es, ax
                mov di, offset OldISR
                movsd

                mov ax, 0               ; перехват прерывания 05h
                mov ds, ax
                mov bx, 05h*4

                mov ax, offset new05  ; IntrTable [05h] = &new09
                mov [bx], ax
                mov ax, es
                mov [bx+2], es

                mov dx, offset main     ; оставление проги резидентом
                shr dx, 4
                inc dx
                mov ax, 3100h
                int 21h
end start
;--The end.-------------------------------------------------------------;