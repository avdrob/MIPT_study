;=============================================================================
;        Задание # 22. Файл 22ownhlp.asm (файл OwnHelp.com)
;
;    Резидентная система помощи - Основная программа (версия 2)
; F11 - копирует текстовый экра в файл, F12 - показывает этот файл
;
;               Copyright (c) 2002 Andreev Andrey
;=============================================================================

;-------------------------------------------------------------------------;
; OwnHelp v0.2  Read info from file and show it. (c) Andreev Andrey, 2002 ;
;-------------------------------------------------------------------------;
.model tiny

video_page      = 0  ; change it for copy different video pages

locals @@

.code
org 100h

start:          jmp main
;-------------------------------------------------------------------------;
; F11 - copy screen to file                                               ;
; F12 - show file                                                         ;
;-------------------------------------------------------------------------;
new09		proc
                push es ax si di ds

		in al, 60h
                cmp al, 57h
                je copy_screen		
                cmp al, 58h
                je show_file

                pop ds di si ax es
		db 0eah ; jmp far
OldISR_09       dd 0bad0badh

show_file:
                in al, 61h
		and al, 1111111b
		out 61h, al
		or al, 80h
		out 61h, al

                mov bx, cs
                mov ds, bx
                mov bx, offset Show_File_Flag     ; Show_File_Flag = true
                mov byte ptr [bx], 01h

		mov al, 20h
		out 20h, al
                pop ds di si ax es
		iret

copy_screen:
                in al, 61h
		and al, 1111111b
		out 61h, al
		or al, 80h
		out 61h, al

                mov bx, cs
                mov ds, bx
                mov bx, offset Copy_Screen_Flag    ; Copy_Screen_Flag = true
                mov byte ptr [bx], 01h

		mov al, 20h
		out 20h, al
                pop ds di si ax es

                iret
                endp
;-------------------------------------------------------------------------;
new28           proc
                push ax bx cx dx ds es si di

                mov bx, cs      ; load data segment
                mov ds, bx

                mov bx, offset Show_File_Flag
                cmp byte ptr [bx], 00h
                jne show_file_28
                mov bx, offset Copy_Screen_Flag
                cmp byte ptr [bx], 00h
                jne copy_screen_28

		jmp @@exit
show_file_28:
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

                mov di, 0       ; show file
                mov si, offset text_of_file
                mov ah, 0ah     ; standart color
                call print_str
                jmp @@exit_cf
;--End of show file-------------------------------------------------------;
;===========================================================================;
copy_screen_28:

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
                 jmp @@exit_cf
;--End of copy screen to file---------------------------------------------;

; exit form procedure new28h
exit28:
@@exit_cf:      mov bx, offset Show_File_Flag     ; Show_File_Flag = false
                mov byte ptr [bx], 0
;                mov bx, offset Copy_Screen_Flag     ; Show_File_Flag = false
		inc bx
                mov byte ptr [bx], 0

@@exit:        pop di si es ds dx cx bx ax

                db 0eah                 ; call Last ISR
OldISR_28       dd 0bad0badh

                endp
;-------------------------------------------------------------------------;
include 22shwerr.asm
include 22prtstr.asm

myfile          db 'c:\myfile',0        ; File name. You can change it!
Show_File_Flag  db 00h
Copy_Screen_Flag db 00h
text_of_file    db 'TEXT FROM FILE!!!'  ; Buffer for text (2000 bytes)
text_of_file_len = $ - text_of_file
                db 80*25 - text_of_file_len  dup ('!')

scr_text        db 'Here must be screen!!!'
scr_text_len    = $ - scr_text          ; buffer for screen (2000 bytes)
                db 80*25 - scr_text_len dup ('!')

;#########################################################################;
main:           .386
; INT 09h
		xor ax, ax
		mov ds, ax
		mov si, 09h*4
		mov ax, cs
		mov es, ax
                mov di, offset OldISR_09
		movsd

		xor ax, ax		; перехват прерывания 09h
		mov ds, ax
		mov bx, 09h*4
		
		mov ax, offset new09  ; IntrTable [09h] = &new09
		mov [bx], ax
		mov ax, es
		mov [bx+2], es

		int 09h

; INT 28h
                mov ax, 0
                mov ds, ax
                mov si, 28h*4
                mov ax, cs
                mov es, ax
                mov di, offset OldISR_28
                movsd

                mov ax, 0               ; перехват прерывания 28h
                mov ds, ax
                mov bx, 28h*4

                mov ax, offset new28  ; IntrTable [28h] = &new28
                mov [bx], ax
                mov ax, es
                mov [bx+2], es

		int 28h

                mov dx, offset main     ; оставление проги резидентом
                shr dx, 4
                inc dx
                mov ax, 3100h
                int 21h

end start
;--End of program-------------------------------(c) Andreev Andrey, 2002--;
