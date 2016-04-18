;***********************************
; STARS2.ASM from TASMED examples.
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

NUM_STARS       = 200
MAX_SIZE        = 4
DELAY           = 20

X               = 0
Y               = 2
SZ              = 4
COLOR           = 5
EXPANDING       = 6

$CM$_HEIGHT     = 5
$CM$_OFFSET     = 190*320+250
$CM$_COLOR      = 7

dataseg
$cm$            dq 01110001110011110011111010001000111001111011111011111011110b
                dq 10001010001010001010000011011001001010000000100010000010001b
                dq 10000010001010001011110010101010001001110000100011110010001b
                dq 10001010001010001010000010001011111000001000100010000011110b
                dq 01110001110011110011111010001010001011110000100011111010001b

udataseg
stars           db NUM_STARS*8 dup (?)
pal             db 256*3 dup (?)
randseed        dd ?
mousex          dw ?
mousey          dw ?
mousebuttons    dw ?
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

        lea     di, [pal]
        xor     ax, ax
        stosw
        stosb

        mov     cx, 8
        mov     bl, 63
@@pp2:
        mov     dl, 7
@@pp1:
        mov     al, dl
        and     al, 1
        mul     bl
        stosb
        mov     al, dl
        shr     al, 1
        and     al, 1
        mul     bl
        stosb
        mov     al, dl
        shr     al, 2
        and     al, 1
        mul     bl
        stosb
        dec     dl
        jns     @@pp1
        sub     bl, 8
        loop    @@pp2

        push    [dword gs:006ch]
        pop     [randseed]

        mov     ax, 0013h
        int     10h

        xor     ax, ax
        mov     cx, 768
        lea     si, [pal]
        mov     dx, 03c8h
        out     dx, al
        inc     dx
        rep     outsb

        mov     cx, NUM_STARS
        xor     eax, eax
@@create_stars:
        push    eax
        call    _create_star
        pop     eax
        inc     eax
        loop    @@create_stars

@@main_loop:
        push    fs
        pop     es
        xor     di, di
        mov     cx, 16384
        xor     eax, eax
        rep     stosd

        lea     si, [stars]
        mov     cx, NUM_STARS
@@draw_star:
        push    cx
        movsx   cx, [si+SZ]
        or      cx, cx
        jle     @@no_draw
        mov     dl, [si+COLOR]
        mov     di, [si+X]
        mov     ax, [si+Y]
        push    si
        xor     bx, bx
        xor     bp, bp
        xchg    ah, al
        add     di, ax
        shr     ax, 2
        add     di, ax
@@draw_4pixel:
        mov     [es:di+bx], dl
        neg     bx
        mov     [es:di+bx], dl
        neg     bx
        mov     [es:di+bp], dl
        neg     bp
        mov     [es:di+bp], dl
        neg     bp
        inc     bp
        add     bx, 320
        add     dl, 8
        loop    @@draw_4pixel
        pop     si
@@no_draw:
        pop     cx
        add     si, 8
        loop    @@draw_star

        mov     dx, 03dah
        mov     cx, 6
@@retrace:
        in      al, dx
        test    al, 8
        jnz     @@retrace
@@retrace2:
        in      al, dx
        test    al, 8
        jz      @@retrace2
        loop    @@retrace

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
        jne     @@halt

        lea     si, [stars]
        mov     cx, NUM_STARS
@@calc_star:
        mov     al, [si+SZ]
        or      al, al
        jl      @@delayed
        add     al, [si+EXPANDING]
        jz      @@new
        cmp     al, MAX_SIZE
        jb      @@next
        neg     [byte si+EXPANDING]
        jmp     @@next
@@new:
        mov     eax, NUM_STARS
        sub     ax, cx
        call    _create_star
        jmp     @@next2
@@delayed:
        inc     al
@@next:
        mov     [si+SZ], al
@@next2:
        add     si, 8
        loop    @@calc_star

        jmp     @@main_loop

@@halt:
        mov     ax, 0003h
        int     10h

        int     20h

_create_star:
        push    ecx
        lea     di, [stars+eax*8]
        call    _random
        mov     ecx, eax
        xor     dx, dx
        mov     bx, DELAY
        div     bx
        neg     dx
        mov     [di+SZ], dl
        mov     eax, ecx
        rcr     eax, 7
        xor     dx, dx
        mov     bx, 320-MAX_SIZE*2
        div     bx
        add     dx, MAX_SIZE
        mov     [di+X], dx
        mov     eax, ecx
        xor     dx, dx
        mov     bx, 200-MAX_SIZE*2
        div     bx
        add     dx, MAX_SIZE
        mov     [di+Y], dx
        and     cl, 7
        mov     [di+COLOR], cl
        mov     [byte di+EXPANDING], 1
        pop     ecx
        ret

_random:
        mov     eax, [cs:randseed]
        imul    eax, MAGIC_CONSTANT1
        add     eax, MAGIC_CONSTANT2
        mov     [cs:randseed], eax
        ret

        end
