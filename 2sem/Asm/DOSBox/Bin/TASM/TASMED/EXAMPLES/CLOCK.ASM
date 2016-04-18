;***********************************
; CLOCK.ASM from TASMED examples.
; Copyright (C) 1997 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

        p386
        ideal
        model   tiny

MAGIC_CONSTANT1 = 1103515245
MAGIC_CONSTANT2 = 12345

MOUSE_SENSITIVITY = 64

$CM$_HEIGHT     = 5
$CM$_OFFSET     = 190*320+250
$CM$_COLOR      = 85

dataseg
$cm$            dq 01110001110011110011111010001000111001111011111011111011110b
                dq 10001010001010001010000011011001001010000000100010000010001b
                dq 10000010001010001011110010101010001001110000100011110010001b
                dq 10001010001010001010000010001011111000001000100010000011110b
                dq 01110001110011110011111010001010001011110000100011111010001b
digits          db 038h, 06ch, 0c6h, 0c6h, 0c6h, 0deh, 0f6h, 0c6h, 06ch, 038h
                db 018h, 038h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 03ch
                db 03ch, 066h, 0c6h, 00ch, 00ch, 018h, 030h, 030h, 060h, 0feh
                db 078h, 0cch, 006h, 03ch, 006h, 006h, 006h, 006h, 0cch, 078h
                db 00ch, 01ch, 03ch, 06ch, 0cch, 0cch, 06ch, 03eh, 00ch, 00ch
                db 0feh, 0c0h, 0c0h, 0f8h, 00ch, 006h, 006h, 006h, 0cch, 078h
                db 03ch, 060h, 0c0h, 0f8h, 0cch, 0c6h, 0c6h, 0c6h, 0cch, 078h
                db 0feh, 00ch, 00ch, 018h, 018h, 030h, 030h, 030h, 030h, 030h
                db 038h, 06ch, 0c6h, 0c6h, 07ch, 0c6h, 0c6h, 0c6h, 06ch, 038h
                db 038h, 06ch, 0c6h, 0c6h, 0c6h, 066h, 03eh, 006h, 00ch, 078h
                db 000h, 000h, 000h, 018h, 018h, 000h, 000h, 018h, 018h, 000h
h               dw ?
                db ':'
m               dw ?
                db ':'

udataseg
s               dw ?
printaddr       dw ?
color           db ?
randseed        dd ?
mousex          dw ?
mousey          dw ?
mousebuttons    dw ?
pal             db 768 dup (?)
stackseg        dw 200  dup (?)
label   stack_top
label   program_end

codeseg
        startupcode

                db "CODEMASTER ", 1ah

        lea     sp, [stack_top]

        push    0040h
        pop     gs

        xor     bx, bx
        xor     cx, cx
        xor     dx, dx
        mov     ax, 0003h
        int     33h
        mov     [mousex], cx
        mov     [mousey], dx
        mov     [mousebuttons], bx

        mov     ax, cs
        lea     bx, [program_end+15]
        shr     bx, 4
        add     ax, bx
        mov     fs, ax

        mov     es, ax
        xor     di, di
        xor     eax, eax
        mov     cx, 16384
        rep     stosd

        push    cs
        pop     es
        lea     di, [pal]
        mov     cx, 192
        rep     stosd

        push    [dword gs:006ch]
        pop     [randseed]

        mov     ax, 0013h
        int     10h

        mov     bp, 85
        mov     cl, 63
        xor     bx, bx
        lea     di, [pal]
@@generate_palette:
        mov     ax, bx
        shl     ax, 6
        cwd
        div     bp
        mov     [byte di], al
        mov     [byte di+85*3], cl
        mov     [byte di+85*3+1], al
        mov     [byte di+85*6], cl
        mov     [byte di+85*6+1], cl
        mov     [byte di+85*6+2], al
        add     di, 3
        inc     bx
        cmp     bx, bp
        jb      @@generate_palette

        xor     ax, ax
        mov     cx, 768
        lea     si, [pal]
        mov     dx, 03c8h
        out     dx, al
        inc     dx
        rep     outsb

@main_loop:
        push    fs
        pop     es

        mov     ah, 2ch
        int     21h
        mov     al, ch
        call    @@ascii
        mov     [h], ax
        mov     al, cl
        call    @@ascii
        mov     [m], ax
        mov     al, dh
        push    offset @@draw
@@ascii:
        aam
        add     ax, '00'
        xchg    ah, al
        ret

@@draw:
        mov     [s], ax
        lea     si, [h]
        mov     cx, 8
        mov     bp, 5
        mov     bx, 320*80+1
        mov     [printaddr], offset _print1
        call    _print

@@process_screen:
        push    es
        pop     ds
        mov     di, 320*2
@@process_loop:
        xor     ax, ax
        movzx   dx, [byte di-1]
        add     ax, dx
        mov     dl, [byte di+1]
        add     ax, dx
        mov     dl, [byte di]
        add     ax, dx
        mov     dl, [byte di+320]
        add     ax, dx
        shr     ax, 2
        mov     dl, [byte di-320]
        add     ax, dx
        shr     ax, 1
        cmp     al, 128
        jbe     @@less_128
        dec     al
        jmp     @@putpixel
@@less_128:
        cmp     al, 2
        jbe     @@lessequ_2
        sub     al, 3
        jmp     @@putpixel
@@lessequ_2:
        xor     al, al
@@putpixel:
        mov     [di-320*2], al
        mov     [di-320], al
        inc     di
        cmp     di, 320*204
        jb      @@process_loop

        mov     dx, 03dah
        mov     cx, 2
@@retrace1:
        in      al, dx
        test    al, 8
        jnz     @@retrace1
@@retrace2:
        in      al, dx
        test    al, 8
        jz      @@retrace2
        loop    @@retrace1

        push    cs 0a000h
        pop     es
        mov     cx, 16384
        xor     si, si
        xor     di, di
        rep     movsd
        pop     ds

        lea     si, [h]
        mov     cx, 8
        mov     bp, 5
        mov     bx, 320*80-320
        mov     [printaddr], offset _print3
        call    _print
        mov     bx, 320*80+1
        mov     [printaddr], offset _print2
        call    _print

        lea     si, [$cm$+7]
        mov     di, $CM$_OFFSET
        mov     bp, $CM$_HEIGHT
        mov     dx, 8*8
@@91:
        mov     bl, [si]
        dec     si
        mov     al, $CM$_COLOR
        mov     cx, 8
@@93:
        shl     bl, 1
        jnc     @@92
        stosb
        jmp     @@97
@@92:
        inc     di
@@97:
        dec     dx
        jnz     @@96
        mov     dx, 8*8
        add     di, 320-8*8
        add     si, 16
        dec     bp
        jz      @@check_mouse
@@96:
        loop    @@93
        jmp     @@91

@@check_mouse:
        mov     ax, 0003h
        int     33h
        sub     cx, [mousex]
        and     ch, 07fh
        sub     dx, [mousey]
        and     dh, 07fh
        add     cx, dx
        cmp     cx, MOUSE_SENSITIVITY
        jae     @@halt
        cmp     bx, [mousebuttons]
        jne     @@halt
        mov     ax, [gs:001ah]
        cmp     ax, [gs:001ch]
        je      @main_loop
        mov     [gs:001ch], ax

@@halt:
        mov     ax, 0003h
        int     10h

        int     20h

_random:
        mov     eax, [cs:randseed]
        imul    eax, MAGIC_CONSTANT1
        add     eax, MAGIC_CONSTANT2
        mov     [cs:randseed], eax
        ret

_print:
        pusha
@@char_loop:
        lodsb
        pusha
        xor     ah, ah
        shl     ax, 3
        mov     si, ax
        shr     ax, 2
        add     si, ax
        add     si, offset      digits-'0'*10
        mov     cx, 10
@@height_loop:
        lodsb
        push    cx bx
        mov     ah, al
        mov     cx, 8
@@width_loop:
        shl     ah, 1
        jnc     @@skip_bar
        mov     di, bx
        call    [cs:printaddr]
@@skip_bar:
        add     bx, bp
        loop    @@width_loop
        pop     bx
        mov     cx, bp
@@add_320:
        add     bx, 320
        loop    @@add_320
        pop     cx
        loop    @@height_loop
        popa
        push    cx
        mov     cx, bp
        shl     cx, 3
        add     bx, cx
        pop     cx
        loop    @@char_loop
        popa
        ret

_print1:
        push    cx
        mov     cx, bp
@@hor_line:
        push    cx
        mov     cx, bp
        push    eax
        call    _random
        rep     stosb
        pop     eax cx
        add     di, 320
        sub     di, bp
        loop    @@hor_line
        pop     cx
        ret

_print2:
        push    cx
        mov     cx, bp
@@hor_line:
        push    cx
        mov     cx, bp
        xor     al, al
        rep     stosb
        pop     cx
        add     di, 320
        sub     di, bp
        loop    @@hor_line
        pop     cx
        ret

_print3:
        push    cx
        mov     cx, bp
        inc     cx
        inc     cx
@@hor_line:
        push    cx
        mov     cx, bp
        mov     al, -2
        inc     cx
        inc     cx
        rep     stosb
        dec     di
        dec     di
        pop     cx
        add     di, 320
        sub     di, bp
        loop    @@hor_line
        pop     cx
        ret

        end
