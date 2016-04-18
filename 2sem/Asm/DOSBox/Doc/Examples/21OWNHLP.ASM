;=============================================================================
;        Задание # 21. Файл 21ownhlp.asm (файл OwnHelp.com)
;
;         Резидентная система помощи - Основная программа
;
;               Copyright (c) 2002 Andreev Andrey
;=============================================================================

;-------------------------------------------------------------------------;
; OwnHelp v0.1  Read info from file and show it. (c) Andreev Andrey, 2002 ;
;-------------------------------------------------------------------------;
.model tiny

.code
org 100h

start:          jmp main
;--When down PrntScr this proc make true PrintScreen_down flag------------;
new05           proc
                push bx ds

                mov bx, cs
                mov ds, bx
                mov bx, offset PrntScr_down     ; PrintScreen_down = true
                mov byte ptr [bx], 01h

                pop ds bx
                iret
                endp
;--If PrintScreen_down is true this proc open file and show info from him-;
new28           proc
                push ax bx cx dx ds es si di

                mov bx, cs      ; load data segment
                mov ds, bx

                mov bx, offset PrntScr_down
                cmp byte ptr [bx], 00h
                je exit28

 ;--Works with file-------------------------------------------------------;
                mov ah, 3dh     ; open file
                xor al, al
                mov dx, offset myfile
                int 21h
                jc open_error
                push ax                 ; ax - file handle
                                ; read file
                pop bx
                push bx
                mov cx, 80*25           ; number of bytes to read
                mov dx, offset text_of_file
                mov ah, 3fh
                int 21h
                jc read_error
                                ; close file
                pop bx                  ; bx = file handle
                mov ah, 3eh
                int 21h
                jc close_error
 ;--End of works with file-----------------------------------------------;


                mov bx, cs
                mov ds, bx
                mov bx, 0b800h  ; draw frame
                mov es, bx
;--If you delete ";;" in ALL strings with ";;", info been show in frame--;
;;                mov bl, 0
;;                mov bh, 79d
;;                mov dl, 0
;;                mov dh, 24d
;;                cld
;;                call draw_frame
                                ; show file
;;                mov si, offset file_text_msg    ; show head of window
;;                mov di, (80+25)*2
;;                mov ah, 0eh     ; head's color
;;                call print_str

                mov di, 0 ;;  +(80*2+1)*2              ; show file
                mov si, offset text_of_file
                mov ah, 0ah     ; standart color
                call print_str
; exit form procedure new28h
exit28:         mov bx, offset PrntScr_down     ; PrintScreen_down = false
                mov byte ptr [bx], 0

                pop di si es ds dx cx bx ax

                db 0eah                 ; call Last ISR
OldISR          dd 0bad0badh

                endp
;-------------------------------------------------------------------------;
include 21shwerr.asm
include 21prtstr.asm
;;include 21frame.asm           ; draw_frame library

myfile          db 'c:\myfile',0        ; File name. You can change it!
;;file_text_msg   db 'Text from "c:\myfile" : $'
PrntScr_down    db 00h
text_of_file    db 'TEXT FROM FILE!!!'  ; Buffer for text (2000 bytes)
text_of_file_len = $ - text_of_file
                db 80*25 - text_of_file_len  dup ('!')
;#########################################################################;
main:           .386
; INT 05h       mov ax, 0               ; перехват прерывания 05h
                mov ds, ax
                mov bx, 05h*4

                mov ax, offset new05  ; IntrTable [05h] = &new05
                mov [bx], ax
                mov ax, es
                mov [bx+2], es
; INT 28h
                mov ax, 0
                mov ds, ax
                mov si, 28h*4
                mov ax, cs
                mov es, ax
                mov di, offset OldISR
                movsd

                mov ax, 0               ; перехват прерывания 28h
                mov ds, ax
                mov bx, 28h*4

                mov ax, offset new28  ; IntrTable [28h] = &new28
                mov [bx], ax
                mov ax, es
                mov [bx+2], es

                mov dx, offset main     ; оставление проги резидентом
                shr dx, 4
                inc dx
                mov ax, 3100h
                int 21h

end start
;--End of program-------------------------------(c) Andreev Andrey, 2002--;