;=============================================================================
;                 Задание # 15. Файл 15calc.asm
;       Перевод десятичного числа в систему с любым основанием
;               Copyright (c) 2002 Andreev Andrey
;=============================================================================
.model tiny

color           = 0eh
x1              = 10
x2              = 33
y1              = 2
y2              = 4

.code
org 100h

start:          mov ax, 0b800h
                mov es, ax
; рисование первой рамки
                mov bl, x1
                mov bh, x2
                mov dl, y1
                mov dh, y2
                cld
                call draw_frame
; вывод надписи 'Введите число > '
                mov si, offset msg1
                mov di, ((y1+1)*80 + x1 + 2) * 2
                call print_str
; ввод числа в ax
                push bx dx
                mov bx, di
                mov dx, bx
                add dx, 8d
                call input_symb
                pop dx bx
                push ax         ; сохранение введённого числа
; рисование второй рамки
                mov bl, x1
                mov bh, x2
                mov dl, y2 + 1
                mov dh, y2 + 1 + y2 - y1
                cld
                call draw_frame
; вывод надписи 'Введите основание > '
                mov si, offset msg2
                mov di, ((y2+2)*80+x1+2)*2
                call print_str
; ввод основания степени
                push bx dx
                mov bx, di
                mov dx, bx
                add dx, 8d
                call input_symb
                pop dx bx
; при невведённой степени умолчание - 2
                cmp ax, 0000h
                je default_osn

default_next:   mov dx, ax
                push dx         ; сохранение степени системы
; рисование рамки результатa
                mov bl, x1
                mov bh, x2
                mov dl, y2 + 4
                mov dh, y2 + 4 + y2 - y1
                cld
                call draw_frame

; вывод результата
                std
                mov di, ((y2+5)*80+x2-2)*2
                pop dx          ; восстановление степени
                pop ax          ; восстановление числа
                call show_ax
; вывод надписи 'Результат > '
                cld
                mov si, offset msg3
                mov di, ((y2+5)*80+x1+2)*2
                call print_str
; выход
                mov ax, 4c00h
                int 21h

default_osn:    mov ax, 2d
                jmp default_next

include 15calclb.asm

.data
msg1            db 'Введите число > $'
msg2            db 'Основание (2) > $'
msg3            db 'Результат > $'

end start
;=============================================================================
;                       Конец...
;=============================================================================
