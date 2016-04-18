;=============================================================================
;       Задание # 20. Файл 20scr2fl.asm (программа Scr2File.com)
;
;        Резидентная система помощи - вспомогательная утилита
;        сохранения текста на экране в файл
;
;                Copyright (c) 2002 Andreev Andrey
;=============================================================================

;-- Resident program (call - PrntScr) for copy screen to file "c:\myfile" --;
;----------------------------------------------- (c) Andreev Andrey, 2002 --;
.model tiny

;--Constants------------------------------------------;
video_page      = 0  ; change it for copy different video pages

.386
.code
org 100h

start:          jmp main
;--Resident part of program------------------------------------------------>

;--New05 procedure - make PrntScr_down flag is true-----------------------;
new05           proc
                push bx ds

                mov bx, cs
                mov ds, bx
                mov bx, offset PrntScr_down     ; PrintScreen_down = true
                mov byte ptr [bx], 01h

                pop ds bx
                iret
                endp

;--New28 procedure - copy screen to file "c:\myfile"----------------------;
new28           proc
                push ax bx cx dx ds es

                mov bx, cs      ; load data segment
                mov ds, bx

                mov bx, offset PrntScr_down
                cmp byte ptr [bx], 00h
                je exit28

                mov bx, 0b800h
                mov es, bx
                mov bx, offset scr_text
                mov di, (80*25*2)*video_page

                mov cx, 80*25
grab_scr:       mov ax, es:[di]
                mov [bx], ax
                inc bx
                add di, 2
                loop grab_scr
 ;--Works with file-------------------------------------------------------;
                mov ah, 3dh     ; open file
                mov al, 2
                mov dx, offset myfile
                int 21h
                jc open_error
                push ax                 ; ax - file handle
                                ; write file
                pop bx
                push bx
                mov ah, 40h
                mov cx, 80*25
                mov dx, offset scr_text
                int 21h
                jc write_error
                                ; close file
close_file:     pop bx                  ; bx = file handle
                mov ah, 3eh
                int 21h
                jc close_error
 ;--End of works with file------------------------------------------------;

; exit form procedure new28h
exit28:         mov bx, offset PrntScr_down     ; PrintScreen_down = false
                mov byte ptr [bx], 0

                pop es ds dx cx bx ax

                db 0eah                 ; call Last ISR
OldISR          dd 0bad0badh

                endp

include 20shwerr.asm
include 20prtstr.asm
 ;--Resident data---------------------------------------------------------;
myfile          db 'c:\myfile', 0       ; file name (changeable)
PrntScr_down    db 00h
scr_text        db 'Here must be screen!!!'
scr_text_len    = $ - scr_text          ; buffer for screen (2000 bytes)
                db 80*25 - scr_text_len dup ('!')
;--End of resident part----------------------------------------------------<
;#########################################################################;
;--Not resident part - setup ints and make resident program---------------;
main:
; INT 05h
                mov ax, 0
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
;-- End of program -----------------------------(c) Andreev Andrey, 2002--;