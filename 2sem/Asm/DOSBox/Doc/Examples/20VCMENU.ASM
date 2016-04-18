;=============================================================================
;                 Задание # 20. Файл 20vcmenu.asm
;       Резидентная программа-шутка, перехватывающая прерывание 09
;               Copyright (c) 2002 Andreev Andrey
;=============================================================================

;-------------------------------------------------------------------------;
;--New menu for Volkov Commander----------------(c) Andreev Andrey, 2002--;
;--Press F1-F10 to activate!----------------------------------------------;
;-------------------------------------------------------------------------;

.model tiny

.code
org 100h

start:          jmp main

msg     db '1~  03  ~ 2~Меню  ~ 3~HackIt~ 4~Debug ~ 5~Copy&D~ 6~Пере??~ 7~Дирка ~ 8~УБИТЬ!~ 9~format~10~KillMe ~$'

;--Constants for New05 proc--;
x               = 0          ;
y               = 24         ;
color1          = 07h        ;
color2          = 3bh        ; original = 30h
;-------------------------------------------------------------------------;
new05           proc
                push es ax dx bx di

                mov ax, 0b800h
                mov es, ax
                mov ah, color1
                mov bx, offset msg
                mov di, (y * 80 + x) * 2

again:          mov al, cs:[bx]
                inc bx
                cmp al, '$'
                je  done
                cmp al, '~'    
                je  changecolor
                mov es:[di], ax
                add di, 2
                jmp again

changecolor:    cmp ah, color1
                je use_color2
                mov ah, color1
                jmp again

use_color2:     mov ah, color2
                jmp again

done:           pop di bx dx ax es
                iret
                endp

;---Messages for menu------------------------------------------------------------------------;

msg_f1  db ' Жаль, но медицина здесь бессильна...       (А может это и к лучшему?)          $'
msg_f2  db ' Тебе надо меню? Оно же на нижней строчке!!! Смотри и выбирай!                  $'
msg_f3  db ' Нехорошо смотреть чужие файлы. А HACKать тем более!                            $'
msg_f4  db ' Всё равно не получится... Нечего и пытаться!                                   $'
msg_f5  db ' Copy And Delete. Оно те надо?..                                                $'
msg_f6  db ' Переименовать и переместить? Зачем?!                                           $'
msg_f7  db ' Ну зачем тебе лишняя дырка?                                                    $'
msg_f8  db ' ЗА ЧТО???                                                                      $'
msg_f9  db ' format c: processing...............................................            $'
msg_f10 db ' Это не выход из ситуации! Придумай что-нибудь получше!                         $'
;--------------------------------------------------------------------------------------------;

;--Constants for New09 proc--;
x2              = 0          ;
y2              = 15         ;
msg_color       = 4eh        ;
;----------------------------;

new09           proc
                push es ax di bx

                in al, 60h
                cmp al, 3bh 
                je f1
                cmp al, 3ch 
                je f2
                cmp al, 3dh 
                je f3
                cmp al, 3eh 
                je f4
                cmp al, 3fh 
                je f5
                cmp al, 40h 
                je f6
                cmp al, 41h 
                je f7
                cmp al, 42h 
                je f8
                cmp al, 43h 
                je f9
                cmp al, 44h 
                je f10

;  if (Scan-Kode != F1...F10) exit;

                pop bx di ax es
                db 0eah ; jmp far       ; call last ISR
OldISR          dd 0bad0badh

f1:             mov bx, offset msg_f1
                jmp output
f2:             mov bx, offset msg_f2
                jmp output
f3:             mov bx, offset msg_f3
                jmp output
f4:             mov bx, offset msg_f4
                jmp output
f5:             mov bx, offset msg_f5
                jmp output
f6:             mov bx, offset msg_f6
                jmp output
f7:             mov bx, offset msg_f7
                jmp output
f8:             mov bx, offset msg_f8
                jmp output
f9:             mov bx, offset msg_f9
                jmp output
f10:            mov bx, offset msg_f10

output:         int 05h
		mov ax, 0b800h
                mov es, ax
                mov di, (80*y2+x2)*2
		mov ah, msg_color
next_output:    mov al, cs:[bx]
                inc bx
                cmp al, '$'
                je exit
                mov es:[di], ax
                add di, 2
                jmp next_output

; exit without call last ISR :

exit:           in al, 61h
                and al, 01111111b
                out 61h, al
                or al, 80h
                out 61h, al

                mov al, 20h
                out 20h, al

                pop bx di ax es
                iret
                endp

;-------------------------------------------------------------------------;  
main:		.386

; INT 09h
                xor ax, ax
                mov ds, ax
                mov si, 09h*4
                mov ax, cs
                mov es, ax
                mov di, offset OldISR
                movsd

                xor ax, ax               
                mov ds, ax
                mov bx, 09h*4

                mov ax, offset new09             ; IntrTable [09h] = &new09
                mov [bx], ax
                mov ax, es
                mov [bx+2], es
; INT 05
                xor ax, ax            
                mov ds, ax
                mov bx, 05h*4
                mov word ptr [bx], offset new05  ; IntrTable [05h] = &new05
                mov ax, es
                mov [bx+2], es

; оставление проги резидентом

                mov dx, offset main     
                shr dx, 4
                inc dx
                mov ax, 3100h
                int 21h
end start
