;***********************************
; COLORS.ASM from TASMED examples.
; Copyright (C) 1997 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

        p386
        ideal
        model   tiny

ZERO            = 200-64

dataseg
b1              db 0
b2              db 20
b3              db 40
long            dw 256
amp             dw 200-32
const_2         dw 2

udataseg
x               dw ?
sintab          dw 256 dup (?)
temp            db 96 dup       (?)
pal             db 1200    dup (?)

codeseg
        startupcode

                db "CODEMASTER ", 1ah

        fninit
        lea     di, [sintab]
        xor     cx, cx
@@table_loop:
        push    cx
        mov     bp, sp
        fild    [word bp]
        fldpi
        fmul
        fimul   [const_2]
        fidiv   [long]
        fsin
        fimul   [amp]
        fistp   [word bp]
        pop     ax
        add     ax, ZERO
        stosw
        inc     cl
        jnz     @@table_loop

@@main_loop:
        push    cs
        pop     ds

        cli
        mov     dx, 03dah
@@retrace1:
        in      al, dx
        and     al, 8
        jnz     @@retrace1
@@retrace2:
        in      al, dx
        and     al, 8
        jz      @@retrace2

        lea     di, [pal]
        xor     ax, ax
        mov     cx, 600
        rep     stosw

        lea     di, [pal]
        mov     bl, [b1]
        call    _prepare

        lea     di, [pal+1]
        mov     bl, [b2]
        call    _prepare

        lea     di, [pal+2]
        mov     bl, [b3]
        call    _prepare

        mov     cx, 400
        lea     si, [pal]
@@copper:
        mov     dl, 0c8h
        xor     al, al
        out     dx, al
        inc     dx
        outsb
        outsb
        outsb

        mov     dl, 0dah
@@line1:
        in      al, dx
        test    al, 1
        jnz     @@line1
@@line2:
        in      al, dx
        test    al, 1
        jz      @@line2
        loop    @@copper
        sti

        inc     [b1]
        inc     [b2]
        inc     [b3]

        push    0040h
        pop     ds

        mov     ax, [001ah]
        cmp     ax, [001ch]
        je      @@main_loop
        mov     [001ch], ax

        mov     dl, 0c8h
        xor     al, al
        out     dx, al
        inc     dx
        out     dx, al
        out     dx, al
        out     dx, al

        int     20h

proc    _prepare
        xor     bh, bh
        shl     bx, 1
        lea     si, [sintab+bx]
        lodsw
        add     di, ax
        add     di, ax
        add     di, ax

        xor     al, al
        mov     cx, 64
@@next:
        stosb
        not     al
        add     di, 64*3-1
        stosb
        sub     di, 64*3-2
        neg     al
        loop    @@next
        ret
endp    _prepare

        end
