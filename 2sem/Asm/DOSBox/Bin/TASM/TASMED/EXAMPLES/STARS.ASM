;***********************************
; STARS.ASM from TASMED examples.
; Copyright (C) 1997 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

        p386
        ideal
        model   tiny

MAGIC_CONSTANT1 = 1103515245
MAGIC_CONSTANT2 = 12345
NUM_STARS       = 511

MOUSE_SENSITIVITY = 64

$CM$_HEIGHT     = 5
$CM$_OFFSET     = 190*320+250
$CM$_COLOR      = 19

dataseg
$cm$            dq 01110001110011110011111010001000111001111011111011111011110b
                dq 10001010001010001010000011011001001010000000100010000010001b
                dq 10000010001010001011110010101010001001110000100011110010001b
                dq 10001010001010001010000010001011111000001000100010000011110b
                dq 01110001110011110011111010001010001011110000100011111010001b

udataseg
x               dw NUM_STARS    dup (?)
y               dw NUM_STARS    dup (?)
z               dw NUM_STARS    dup (?)
color           db NUM_STARS dup        (?)
pal             db 256*3 dup (?)
randseed        dd ?
mousex          dw ?
mousey          dw ?
mousebuttons    dw ?
stack_seg       dw 200 dup (?)
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

        lea     di, [pal]
        mov     cx, 384
        rep     stosw

        push    [dword gs:006ch]
        pop     [randseed]

        mov     ax, 0013h
        int     10h

        lea     di, [pal]
        xor     dl, dl
@@prepare_palette:
        mov     al, dl
        and     al, 3
        add     al, 3
        mov     ah, 10
        mul     ah
        mov     bh, al
        mov     al, dl
        shr     al, 4
        mul     bh
        stosb
        mov     al, dl
        shr     al, 3
        and     al, 1
        mul     bh
        stosb
        mov     al, dl
        shr     al, 2
        and     al, 1
        mul     bh
        stosb
        inc     dl
        cmp     dl, 32
        jb      @@prepare_palette

        xor     ax, ax
        mov     cx, 768
        lea     si, [pal]
        mov     dx, 03c8h
        out     dx, al
        inc     dx
        rep     outsb

        mov     cx, NUM_STARS
        xor     si, si
        xor     bp, bp
@@generate_star:
        call    _random
        mov     ebx, NUM_STARS
        xor     edx, edx
        div     ebx
        sub     dx, NUM_STARS/2
        mov     [x+si], dx
        call    _random
        xor     edx, edx
        div     ebx
        sub     dx, NUM_STARS/2
        mov     [y+si], dx
        mov     [z+si], cx
        and     al, 07h
        mov     [color+bp], al
        inc     bp
        inc     si
        inc     si
        loop    @@generate_star

@@main_loop:
        push    fs
        pop     es
        xor     di, di
        mov     cx, 16384
        xor     eax, eax
        rep     stosd

        mov     cx, NUM_STARS
        xor     si, si
        xor     bp, bp
@@2d:
        movzx   ebx, [z+si]
        push    bx
        dec     bx
        dec     bx
        jg      @@z_more0
        add     bx, NUM_STARS
@@z_more0:
        mov     [z+si], bx
        pop     bx
        movsx   eax, [x+si]
        sal     eax, 7
        cdq
        idiv    ebx
        add     ax, 160
        jl      @@outbounds
        cmp     ax, 319
        jg      @@outbounds
        mov     di, ax
        movsx   eax, [y+si]
        sal     eax, 7
        cdq
        idiv    ebx
        add     ax, 100
        jl      @@outbounds
        cmp     ax, 199
        jg      @@outbounds
        xchg    ah, al
        add     di, ax
        shr     ax, 2
        add     di, ax
        mov     al, [color+bp]
        shl     al, 2
        add     al, 3
        shr     bx, 7
        sub     al, bl
        stosb
@@outbounds:
        inc     bp
        inc     si
        inc     si
        loop    @@2d

        mov     dx, 03dah
@@retrace:
        in      al, dx
        test    al, 8
        jz      @@retrace

        push    0a000h
        pop     es
        mov     cx, 16384
        xor     si, si
        xor     di, di
        segfs
        rep     movsd

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
        jz      @@check_kbd
@@96:
        loop    @@93
        jmp     @@91

@@check_kbd:
        mov     ax, [gs:001ah]
        cmp     ax, [gs:001ch]
        je      @@check_mouse
        mov     [gs:001ch], ax
        jmp     @@halt

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
        je      @@main_loop

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

        end
