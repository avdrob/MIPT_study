;=============================================================================
;                 Задание # 27. Файл 27graph.asm
;               Пример работы в графическом режиме
;                Copyright (c) 2002 Andreev Andrey
;=============================================================================

.model tiny

color		= 02h
getmaxx		= 320
getmaxy		= 200

x1              = 0
y1              = 100
x2              = 160
y2              = 0

.code
org 100h

start:		mov ax, 0013h
		int 10h

		mov ax, 0a000h
		mov es, ax

		mov al, color	
		mov bx, x1
		mov dx, y1
		call give_di
		mov cx, getmaxx
		call draw_hline

                mov bx, x2
		mov dx, y2
		call give_di
		mov cx, getmaxy
		call draw_vline

                push bx cx
                mov bx, cs
                mov ds, bx
;               mov bx, offset Show_File_Flag     ; Show_File_Flag = true
;               add byte ptr [bx], 1
                pop cx bx

		mov ax, 4c00h
		int 21h
;-------------------------------------------------------------------------;
; entry:        bx - x, dx - y;                                           ;
; return:       di - y * getmaxx + x                                      ;
;-------------------------------------------------------------------------;
give_di         proc

                push ax bx dx
                mov ax, dx
		mov bx, getmaxx
                mul bx
                pop dx
		pop bx
                add ax, bx
                mov di, ax
                pop ax

                ret
                endp
;-------------------------------------------------------------------------;
include 26lib.asm

;x1	db	00
;y1	db	100

end start
