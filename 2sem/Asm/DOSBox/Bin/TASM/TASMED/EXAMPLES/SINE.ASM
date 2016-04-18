;***********************************
; SINE.ASM from TASMED examples.
; Copyright (C) 1998 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

; Execute it with .bmp file name as parameter.
; Bitmap must be uncompressed, no more than 320x200 in size.

        p486
        ideal
        model   tiny

AMPLITUDE       = 20
LONGITUDE       = 1
SINE_WAVE_X     = 1
SINE_WAVE_Y     = 1

dataseg
pi_d_128        dd 0.0245436926         ; pi/128
amplitude       dw AMPLITUDE
longitude       dw LONGITUDE
alpha           dw 0
beta            dw 0

udataseg
label   header
                db 0012h dup (?)
label   size_x word
                db 0004h dup (?)
label   size_y word
                db 0020h dup (?)
bmp_palette     db 1024 dup     (?)
palette         db 768  dup (?)
temp_word       dw ?
sin             dw 256 dup (?)
cos             dw 256 dup (?)
stackseg        dw 0100h dup (?)
label   stack_top
label   program_end

codeseg
        startupcode

                db 'CODEMASTER  '

        fninit

        mov     bl, 80h
        mov     si, bx
        mov     bl, [bx]
        mov     [si+bx+1], ax

        lea     sp, [stack_top]

        xor     cx, cx
        xor     di, di
@@next:
        mov     [temp_word], cx
        fild    [temp_word]
        fmul    [pi_d_128]
        fimul   [longitude]
        fsincos
        fimul   [amplitude]
        fistp   [cos+di]
        fimul   [amplitude]
        fistp   [sin+di]
        inc     di
        inc     di
        inc     cl
        jnz     @@next

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

        push    cs
        pop     ds

        xor     ax, ax
        mov     cx, 768
        lea     si, [palette]
        mov     dx, 03c8h
        out     dx, al
        inc     dx
        rep     outsb

@@next_frame:
        push    gs
        call    _clear_segment

        xor     si, si
        xor     cx, cx
@@row_loop:
        xor     dx, dx
@@column_loop:
ifdef   SINE_WAVE_X
        mov     bx, cx
else
        mov     bx, dx
endif
        add     bx, [alpha]
        xor     bh, bh
        shl     bx, 1
        mov     bp, [cos+bx]
        add     bp, dx
        js      @@no_pixel
        cmp     bp, 319
        ja      @@no_pixel
ifdef   SINE_WAVE_Y
        mov     bx, dx
else
        mov     bx, cx
endif
        add     bx, [beta]
        xor     bh, bh
        shl     bx, 1
        mov     bx, [sin+bx]
        mov     ax, bx
        add     ax, cx
        js      @@no_pixel
        cmp     ax, 199
        ja      @@no_pixel
        xchg    ah, al
        add     bp, ax
        shr     ax, 2
        add     bp, ax
        mov     al, [fs:bp]
        mov     [gs:si], al
@@no_pixel:
        inc     si
        inc     dx
        cmp     dx, 320
        jne     @@column_loop
        inc     cx
        cmp     cx, 200
        jne     @@row_loop

        push    0a000h
        pop     es
        xor     si, si
        xor     di, di
        mov     cx, 16000
        seggs   rep movsd

        inc     [byte alpha]
        sub     [byte beta], 2

        mov     ah, 1
        int     16h
        jz      @@next_frame

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

        end
