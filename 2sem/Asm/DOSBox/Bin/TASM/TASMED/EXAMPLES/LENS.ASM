;***********************************
; LENS.ASM from TASMED examples.
; Copyright (C) 1998 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

; Execute it with .bmp file name as parameter.
; Bitmap must be uncompressed, no more than 320x200 in size.
; Use mouse to move around the image and buttons to change magnify factor.

        p486
        ideal
        model   tiny

RADIUS          = 50
STARTING_MAGNIFY equ 1.2
MAGNIFY_DELTA   equ 1.02

dataseg
radius2         dw RADIUS*RADIUS
magnify         dd STARTING_MAGNIFY
magnify_delta   dd MAGNIFY_DELTA
const_2         dw 2
const_ln2d2     dd 0.3465735903
const_ln2lo     dd -0.1864e-14

udataseg
lens_posx       dw ?
lens_posy       dw ?
temp_word       dw ?
temp_word2      dw ?
label   lens
lens_x          dw (RADIUS*2+1)*(RADIUS*2+1) dup (?)
lens_y          dw (RADIUS*2+1)*(RADIUS*2+1) dup (?)
label   header
                db 0012h dup (?)
label   size_x word
                db 0004h dup (?)
label   size_y word
                db 0020h dup (?)
bmp_palette     db 1024 dup     (?)
palette         db 768  dup (?)
stackseg        dw 0100h dup (?)
label   stack_top
label   program_end

codeseg
        startupcode

                db 'CODEMASTER  '

        mov     bl, 80h
        mov     si, bx
        mov     bl, [bx]
        mov     [si+bx+1], ax

        fninit

        call    _generate_lens

        lea     sp, [stack_top]

        mov     bp, cs
        lea     bx, [program_end+15]
        shr     bx, 4
        add     bp, bx
        mov     fs, bp
        add     bp, 1000h
        mov     gs, bp

        mov     ax, 3d00h
        mov     dx, 0082h
        int     21h
        jc      @exit_program
        mov     bx, ax

        mov     ah, 3fh
        lea     dx, [header]
        mov     cx, 54
        int     21h

        mov     ah, 3fh
        mov     cx, 1024
        lea     dx, [bmp_palette]
        int     21h

        lea     si, [bmp_palette]
        lea     di, [palette]
        mov     cx, 256
@@next_color:
        lodsd
        shr     al, 2
        bswap   eax
        shr     eax, 8
        shr     al, 2
        shr     ah, 2
        stosd
        dec     di
        loop    @@next_color

        push    fs
        call    _clear_segment

        mov     si, [size_y]
        mov     di, [size_x]
        mov     cx, 200
        sub     cx, si
        shr     cx, 1
        sub     cx, 200
        fastimul bp, cx, -320
        mov     dx, 320
        sub     dx, di
        shr     dx, 1
        add     dx, bp

        push    fs
        pop     ds

@@next_scanline:
        mov     ah, 3fh
        mov     cx, di
        int     21h
        sub     dx, 320
        dec     si
        jnz     @@next_scanline

        mov     ah, 3eh
        int     21h

        mov     ax, 0013h
        int     10h

        xor     ax, ax
        int     33h
        mov     ax, 7
        xor     cx, cx
        mov     dx, 319
        int     33h
        inc     al
        mov     dx, 199
        int     33h
        mov     al, 4
        mov     cx, 160
        mov     dx, 100
        int     33h

        push    cs
        pop     ds

        xor     ax, ax
        mov     cx, 768
        lea     si, [palette]
        mov     dx, 03c8h
        out     dx, al
        inc     dx
        rep     outsb

@next_frame:
        mov     ax, 0003h
        int     33h
        mov     [lens_posx], cx
        mov     [lens_posy], dx
        or      bx, bx
        jz      @@no_button
        dec     bx
        jz      @@left_button
        dec     bx
        jnz     @@no_button
        fld     [magnify]
        fdiv    [magnify_delta]
        fstp    [magnify]
        call    _generate_lens
        jmp     @@no_button
@@left_button:
        fld     [magnify]
        fmul    [magnify_delta]
        fstp    [magnify]
        call    _generate_lens

@@no_button:
        push    gs
        call    _clear_segment

        mov     cx, 16000
        xor     si, si
        xor     di, di
        segfs   rep movsd

        xor     si, si
        mov     cx, -RADIUS
@@outer:
        mov     dx, -RADIUS
@@inner:
        mov     di, [si+lens_x]
        add     di, [lens_posx]
        js      @@no_pixel
        cmp     di, 319
        ja      @@no_pixel
        mov     ax, [si+lens_y]
        add     ax, [lens_posy]
        js      @@no_pixel
        cmp     ax, 199
        ja      @@no_pixel
        xchg    ah, al
        add     di, ax
        shr     ax, 2
        add     di, ax
        mov     bx, dx
        add     bx, [lens_posx]
        js      @@no_pixel
        cmp     bx, 319
        ja      @@no_pixel
        mov     ax, cx
        add     ax, [lens_posy]
        js      @@no_pixel
        cmp     ax, 199
        ja      @@no_pixel
        xchg    ah, al
        add     bx, ax
        shr     ax, 2
        add     bx, ax
        mov     al, [fs:di]
        mov     [gs:bx], al
@@no_pixel:
        inc     si
        inc     si
        inc     bx
        inc     dx
        cmp     dx, RADIUS
        jle     @@inner
        add     bx, 320-2*RADIUS-1
        inc     cx
        cmp     cx, RADIUS
        jle     @@outer

        push    0a000h
        pop     es
        xor     si, si
        xor     di, di
        mov     cx, 16000
        seggs   rep movsd

        mov     ah, 1
        int     16h
        jz      @next_frame

        xor     ah, ah
        int     16h

        mov     ax, 0003h
        int     10h

@exit_program:
        int     20h

_clear_segment:
        pop     ax      es
        push    ax
        xor     di, di
        mov     cx, 16000
        xor     eax, eax
        rep     stosd
        ret

_pow:
        fxch
        fldln2
        fxch
        fyl2x
        fmulp
        fldln2
        fld     st(1)
        fprem
        fld     [const_ln2d2]
        fcomp   st(1)
        fnstsw  ax
        sahf
        jae     @@test_neg
        fsub    st, st(1)
        jmp     @@start

@@test_neg:
        fld     [const_ln2d2]
        fchs
        fcomp   st(1)
        fstsw   ax
        sahf
        jbe     @@start
        fadd    st, st(1)

@@start:
        fxch    st(2)
        fsub    st, st(2)
        fdivrp  st(1), st
        frndint
        fxch    st(1)
        fld     [const_ln2lo]
        fmul    st, st(2)
        fsubp   st(1), st
        fldl2e
        fmul

        ftst
        fnstsw  ax
        sahf
        jb      @@neg
        f2xm1
        fld1
        faddp   st(1), st
        jmp     @@next

@@neg:
        fchs
        f2xm1
        fld1
        fadd    st(1), st
        fdivrp  st(1), st

@@next:
        fscale
        fstp    st(1)

        ret

_generate_lens:
        xor     si, si
        mov     bp, RADIUS
        mov     dx, bp
        neg     dx
@@outer:
        mov     [temp_word2], dx
        mov     cx, bp
        neg     cx
@@inner:
        mov     [temp_word], cx
        fild    [temp_word]
        fimul   [temp_word]
        fild    [temp_word2]
        fimul   [temp_word2]
        faddp
        fidiv   [radius2]
        ftst
        fnstsw  ax
        sahf
        jz      @@distance_zero
        fld     st
        fld     [magnify]
        call    _pow
        fdiv    st, st(1)
        fxch
@@distance_zero:
        fld1
        fcompp
        fnstsw  ax
        sahf
        jbe     @@greater1
        fld     st
        fimul   [temp_word]
        fistp   [word lens_x+si]
        fimul   [temp_word2]
        fistp   [word lens_y+si]
        jmp     @@next
@@greater1:
        mov     [lens_x+si], cx
        mov     [lens_y+si], dx
        fstp    st
@@next:
        inc     si
        inc     si
        inc     cx
        cmp     cx, bp
        jle     @@inner
        inc     dx
        cmp     dx, bp
        jle     @@outer

        ret

        end
