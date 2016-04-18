;-----------------------------------------------------------------------------
;                  Задание # 8. Файл 08video.asm
;        Рамка вокруг стертой пробелами прямоугольой формы
;                (c) 2001 Copyleft  Andreev Andrey
;-----------------------------------------------------------------------------

.model tiny

;-----------------------------------------------------------------------------
; Описание констант

videoseg        = 0b800h
probel          = 0e00h         ; символ, которым заполняется прямоугольник
string_color    = 0eh

left_up         = 0ec9h         ; ╔             символы рамки          -->
hor_line        = 0ecdh         ; ═
right_up        = 0ebbh         ; ╗
ver_line        = 0ebah         ; ║
right_down      = 0ebch         ; ╝
left_down       = 0ec8h         ; ╚             символы рамки          <--

x1              = 10            ; размеры прямоугольника -->
x2              = 70
y1              = 5
y2              = 10            ; размеры прямоугольника <--

;-----------------------------------------------------------------------------
.code
org 100h

start:

; Подготовка к заливке прямоугольника пробельными символами

                mov ax, videoseg
                mov es, ax
                mov ax, probel
                mov di, (y1 * 80 + x1) * 2
                mov cx, x2 - x1         ; Window width
                mov si, 0000h           ; si -> bx. if si = bx, exit
                mov bx, y2 - y1         ; Window height

;Заливка прямоугольника пробелами (два цикла)

x1_to_x2:       stosw                   ; mov es:[di], ax  /  inc di
                loop x1_to_x2

y1_to_y2:       add di, (80d - (x2 - x1)) * 2
                inc si
                mov cx, x2 - x1
                cmp si, bx
                jne x1_to_x2

; Рисование рамки вокруг ранее нарисованного прямоугольника

                mov di, (y1 * 80 + x1 - 81) * 2
                mov ax, left_up
                stosw                   ; вставка верхнего левого угла рамки

                mov ax, hor_line        ; рисование верхней линии рамки
                mov cx, x2 - x1
                rep stosw

                mov ax, right_up
                stosw                   ; рисование верхнего правого угла рамки

                                        ; рисование вертикальных линий рамки
                mov di, (y1 * 80 + x1 - 1) * 2
                mov ax, ver_line
                mov si, 1d              ; счетчик до двух

right_ver:      mov cx, y2 - y1            ; <-------------+
                                           ;               |
vertical:       stosw                      ; <--+          |
                add di, 79d * 2            ;    | цикл 1   |
                loop vertical              ; <--+          |
                                           ;               | цикл 2
                mov di, (y1 * 80 + x2) * 2 ;               |
                add si, 1d                 ;               |
                cmp si, 2d                 ;               |
                je  right_ver              ; <-------------+

                mov ax, left_down       ; рисование нижнего левого угла рамки
                mov di, (y2 * 80 + x1 - 1) * 2
                stosw

                mov ax, hor_line        ; рисование нижней горизонтальной рамки
                mov cx, x2 - x1
                rep stosw
                                        ; и последнее!
                mov ax, right_down      ; рисование нижнего правого угла рамки
                stosw

; вывод надписи

                mov di, (y1 * 80 + x1 + 2) * 2
                mov si, offset msg
                mov ah, string_color
again:          lodsb
                cmp al, '$'
                je done
                stosw
                jmp again

; Выход из программы

done:           mov ax, 4c00h
                int 21h

.data
msg             db '(c) 2001 Copyleft Andreev Andrey.$'

end start

;----------------------------------------------------------------------------
;                               Конец...
;----------------------------------------------------------------------------