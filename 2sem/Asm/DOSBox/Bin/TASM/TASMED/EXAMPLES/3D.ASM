;***********************************
; 3D.ASM from TASMED examples.
; Copyright (C) 1998 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

; Simple morphing 3D demo. Thanks to author of MORPH3D for great inspiration.
; Execute it with "-n" command line switch to supress retrace waiting cycles
; and increase speed. Perfomance of my Pentium 166MMX is 300 fps (under
; Windows 95, no background tasks).

        p386
        ideal
        model   tiny

MAGIC_CONSTANT1 = 1103515245
MAGIC_CONSTANT2 = 12345

NUM_POINTS      = 300
TIMEOUT         = 150
MORPH_TIME      = 100

ANGLE_GRADATIONS = 1024

NUM_OBJECTS     = 8

dataseg
tetraedr_data   db 4, 6
                dw 300/6
                db -41, 24, -17
                db 41, 24, -17
                db 0, -47, -17
                db 0, 0, 50
                db 1, 0
                db 0, 2
                db 0, 3
                db 2, 1
                db 1, 3
                db 3, 2
cube_data       db 8, 12
                dw 300/12
                db -35, -35, -35
                db -35, -35, 35
                db -35, 35, -35
                db -35, 35, 35
                db 35, -35, -35
                db 35, -35, 35
                db 35, 35, -35
                db 35, 35, 35
                db 0, 1
                db 0, 2
                db 1, 3
                db 3, 2
                db 5, 4
                db 4, 6
                db 5, 7
                db 6, 7
                db 4, 0
                db 1, 5
                db 2, 6
                db 7, 3
octaedr_data    db 6, 12
                dw 300/12
                db -35, -35, 0
                db -35, 35, 0
                db 35, 35, 0
                db 35, -35, 0
                db 0, 0, 50
                db 0, 0, -50
                db 0, 1
                db 0, 4
                db 0, 5
                db 1, 2
                db 2, 3
                db 3, 0
                db 4, 1
                db 4, 2
                db 4, 3
                db 5, 1
                db 5, 2
                db 5, 3
dodecaedr_data  db 20, 30
                dw 300/30
                db 0, -18, 48
                db 0, 18, 48
                db 0, -18, -48
                db 0, 18, -48
                db 18, 48, 0
                db -18, 48, 0
                db 18, -48, 0
                db -18, -48, 0
                db 48, 0, 18
                db 48, 0, -18
                db -48, 0, 18
                db -48, 0, -18
                db 29, 29, 29
                db -29, 29, 29
                db 29, -29, 29
                db -29, -29, 29
                db 29, 29, -29
                db -29, 29, -29
                db 29, -29, -29
                db -29, -29, -29
                db 0, 14
                db 0, 15
                db 1, 0
                db 1, 13
                db 2, 3
                db 2, 19
                db 3, 16
                db 3, 17
                db 4, 5
                db 5, 13
                db 6, 14
                db 7, 6
                db 8, 12
                db 8, 9
                db 9, 16
                db 9, 18
                db 10, 15
                db 11, 10
                db 12, 1
                db 12, 4
                db 13, 10
                db 14, 8
                db 15, 7
                db 16, 4
                db 17, 5
                db 17, 11
                db 18, 6
                db 18, 2
                db 19, 7
                db 19, 11
icosaedr_data   db 12, 30
                dw 300/30
                db 0, 0, 50
                db 36, -26, 22
                db 36, 26, 22
                db -14, 42, 22
                db -45, 0, 22
                db -14, -42, 22
                db 45, 0, -22
                db 14, 42, -22
                db -36, 26, -22
                db -36, -26, -22
                db 14, -42, -22
                db 0, 0, -50
                db 0, 1
                db 0, 3
                db 0, 4
                db 0, 5
                db 1, 2
                db 1, 6
                db 2, 0
                db 2, 3
                db 2, 7
                db 3, 4
                db 3, 8
                db 4, 5
                db 4, 9
                db 5, 1
                db 5, 10
                db 6, 2
                db 6, 7
                db 6, 11
                db 7, 3
                db 7, 8
                db 8, 4
                db 8, 9
                db 9, 5
                db 9, 10
                db 10, 1
                db 10, 6
                db 11, 7
                db 11, 8
                db 11, 9
                db 11, 10
copyright       db '(C) 1998 by CodeMaster 2:5002/39@fidonet.', 0dh, 0ah, '$'
frames          db ' frames, $'
seconds         db ' seconds.', 0dh, 0ah, '$'
fps             db ' frames per second.', 0dh, 0ah, '$'
creators        dw create_cube, create_torus, create_sphere, create_octaedr
                dw create_cone, create_dodecaedr, create_tetraedr, create_icosaedr
angle_inc       dd 0.0061359232         ; 2*pi/ANGLE_GRADATIONS
pi_d_5          dd 0.6283185307         ; pi/5
pi_d_10         dd 0.3141592654         ; pi/10
pi_d_14         dd 0.2243994753         ; pi/14
pi_d_15         dd 0.2094395102         ; pi/15
tps             dd -0.18206
dist_mul        dw 300
dist_add        dw 440
video_center_x  dw 160
video_center_y  dw 100
const_2         dw 2
const_8         dw 8
const_20        dw 20
const_50        dw 50
const_100       dw 100
label   zincr word
const_256       dw 256
const_10000     dw 10000
palette         db 00111000b
morph_time      dw MORPH_TIME

        even

label   data_start

udataseg
label   udata_start
label   temp_dword dword
label   temp_word       word
temp_byte       db ?
temp_byte2      db ?
label   temp_word2 word
temp_byte3      db ?
temp_byte4      db ?
label   temp_dword2 dword
temp_word3      dw ?
temp_word4      dw ?
center_x        dw ?
center_y        dw ?
eye_dist        dd ?
randseed        dd ?
start           dd ?
z               dd ?
morph_to        dw ?
label   zero_start
alpha           dw ?
beta            dw ?
gamma           dw ?
delta           dw ?
epsilon         dw ?
phase           dw ?
counter         dw ?
retrace         db ?
halting         db ?
shift           dw ?
num_frames      dd ?
morph_from      dd NUM_POINTS*3 dup (?)
label   zero_finish
sin             dd ANGLE_GRADATIONS dup            (?)
cos             dd ANGLE_GRADATIONS dup            (?)
objects         dd NUM_OBJECTS  dup (NUM_POINTS*3 dup (?))
morphed         dd NUM_POINTS*3 dup (?)
flat            dw NUM_POINTS*3 dup (?)
stack_seg       dw 0100h dup (?)
label   data_end

codeseg
        startupcode

        lea     bp, [udata_start]

        lea     di, [objects]
        lea     si, [creators+bp-data_start]
        mov     cx, NUM_OBJECTS
@@create_next_object:
        lodsw
        fninit
        push    si cx
        call    ax
        pop     cx      si
        loop    @@create_next_object

        lea     di, [zero_start+bp-udata_start]
        mov     cx, zero_finish-zero_start
        xor     al, al
        rep     stosb

        mov     si, 0081h
@@continue:
        lodsb
        cmp     al, 0dh
        je      @@end_cmdline
        cmp     al, '-'
        je      @@switch
        cmp     al, '/'
        jne     @@continue
@@switch:
        lodsb
        dec     si
        and     al, 0dfh
        cmp     al, 'N'
        jne     @@continue
        dec     [byte retrace+bp-udata_start]

@@end_cmdline:
; setup pseudo-random number generator
        push    0040h
        pop     gs
        mov     eax, [dword gs:006ch]
        mov     [randseed+bp-udata_start], eax
        mov     [start+bp-udata_start], eax

; setup virtual screen
        lea     sp, [data_end+bp-udata_start]
        mov     ax, cs
        mov     bx, sp
        shr     bx, 4
        inc     bx
        add     ax, bx
        mov     fs, ax
        mov     es, ax
        xor     di, di
        xor     eax, eax
        mov     cx, 16000
        rep     stosd

; setup sin and cos tables
        fninit
        xor     bx, bx
        mov     [temp_word+bp-udata_start], bx
@@next_angle:
        fld     [angle_inc]
        fimul   [word temp_word+bp-udata_start]
        fsincos
        fstp    [cos+bx]
        fstp    [sin+bx]
        inc     [word temp_word+bp-udata_start]
        add     bx, 4
        cmp     [word temp_word+bp-udata_start], ANGLE_GRADATIONS
        jnz     @@next_angle

        mov     ax, 0013h
        int     10h

        call    start_phase

        push    0a000h
        pop     es

        xor     ax, ax
        mov     dx, 03c8h
        out     dx, al
        inc     dx
        mov     cx, 256*3
@@1:
        out     dx, al
        loop    @@1

go:
; setup palette
        fninit
        mov     ax, 0300h
        mov     dx, 03c8h
        out     dx, al
        inc     dx
@@next_color:
        mov     si, 3
        mov     bl, [palette+bp-data_start]
        mov     ch, ah
        mov     cl, [halting+bp-udata_start]
        shr     cl, 1
        sub     ch, cl
        jns     @@positive
        xor     ch, ch
@@positive:
        mov     cl, ch
        mov     [temp_byte+bp-udata_start], cl
        mov     [byte temp_byte2+bp-udata_start], 0
        fild    [word temp_word+bp-udata_start]
        fimul   [word counter+bp-udata_start]
        fidiv   [morph_time]
        fistp   [word temp_word+bp-udata_start]
        mov     cl, [temp_byte+bp-udata_start]
@@next:
        mov     bh, bl
        and     bh, 00001001b
        mov     al, ch
        cmp     bh, 00001001b
        je      @@setcolor
        mov     al, cl
        cmp     bh, 00001000b
        je      @@setcolor
        mov     al, ch
        sub     al, cl
        cmp     bh, 00000001b
        je      @@setcolor
        xor     al, al
@@setcolor:
        out     dx, al
        shr     bl, 1
        dec     si
        jnz     @@next
        add     ah, 4
        cmp     ah, 63
        jbe     @@next_color

        cmp     [byte halting+bp-udata_start], 0
        jz      @@no_halting
        inc     [byte halting+bp-udata_start]
        cmp     [byte halting+bp-udata_start], 128
        jz      halt

@@no_halting:
        push    offset rotate
        test    [word phase+bp-udata_start], 1
        jz      do_nothing
        jmp     morph

rotate:
; rotate object
        fninit

        mov     di, [alpha+bp-udata_start]
        shl     di, 2
        fld     [sin+di]
        fimul   [word dist_mul+bp-data_start]
        fiadd   [word dist_add+bp-data_start]
        fistp   [word eye_dist+bp-udata_start]
        mov     di, [delta+bp-udata_start]
        shl     di, 2
        fld     [sin+di]
        fidiv   [word const_2+bp-data_start]
        fld1
        faddp
        fimul   [word video_center_x+bp-data_start]
        fistp   [word center_x+bp-udata_start]
        mov     di, [epsilon+bp-udata_start]
        shl     di, 2
        fld     [sin+di]
        fidiv   [word const_2+bp-data_start]
        fld1
        faddp
        fimul   [word video_center_y+bp-data_start]
        fistp   [word center_y+bp-udata_start]

        mov     cx, NUM_POINTS
        lea     bx, [morphed]
        lea     si, [flat]

@@rotate:
        fninit

        mov     di, [alpha+bp-udata_start]
        shl     di, 2

; _y = cos(alpha)*y-sin(alpha)*z
        fld     [dword bx+4]
        fmul    [cos+di]
        fld     [dword bx+8]
        fmul    [sin+di]
        fsubp

; _y
; _z = sin(alpha)*y+cos(alpha)*z
        fld     [dword bx+4]
        fmul    [sin+di]
        fld     [dword bx+8]
        fmul    [cos+di]
        faddp
        fxch

; _y _z
; _x = sin(beta)*_z+cos(beta)*x
        mov     di, [beta+bp-udata_start]
        shl     di, 2
        fld     st(1)
        fmul    [sin+di]
        fld     [dword bx]
        fmul    [cos+di]
        faddp

; _x _y _z
; x = cos(gamma)*_x-sin(gamma)*_y
        mov     di, [gamma+bp-udata_start]
        shl     di, 2
        fld     st
        fmul    [cos+di]
        fld     st(2)
        fmul    [sin+di]
        fsubp

; x _x _y _z
; y = sin(gamma)*_x+cos(gamma)*_y
        fld     st(1)
        fmul    [sin+di]
        fld     st(3)
        fmul    [cos+di]
        faddp

; y x _x _y _z
; z = cos(beta)*_z-sin(beta)*x
        mov     di, [beta+bp-udata_start]
        shl     di, 2
        fld     st(4)
        fmul    [cos+di]
        fld     [dword bx]
        fmul    [sin+di]
        fsubp

; z y x _x _y _z
        fist    [word temp_word+bp-udata_start]
        mov     al, [temp_byte+bp-udata_start]
        neg     al
        jns     @@unsigned
        xor     al, al
@@unsigned:
        shr     al, 3
        add     al, 7
        cmp     al, 15
        jbe     @@le15
        mov     al, 15
@@le15:
        mov     [byte si+4], al

        fiadd   [word zincr+bp-data_start]
        fxch
        fimul   [word eye_dist+bp-udata_start]
        fld     st(1)
        fdivp
        fiadd   [word center_y+bp-udata_start]
        fistp   [word si+2]
        fxch
        fimul   [word eye_dist+bp-udata_start]
        fdivrp
        fiadd   [word center_x+bp-udata_start]
        fistp   [word si]
        add     si, 6
        add     bx, 12
        dec     cx
        jnz     @@rotate

; fade virtual screen
        mov     bx, [shift+bp-udata_start]
        sub     bx, 4
        push    ds fs
        pop     ds
        mov     si, 64000
@@next_point:
        mov     eax, [si+bx]
        sub     al, 2
        jns     @@ok
        xor     al, al
@@ok:
        mov     [si+bx], eax
        sub     si, 4
        jnz     @@next_point
        pop     ds
        add     bx, 5
        and     bl, 00000011b
        mov     [shift+bp-udata_start], bx

; display object
        mov     si, NUM_POINTS*6-6
@@next_pixel:
        mov     di, [flat+si]
        or      di, di
        js      @@continue
        cmp     di, 319
        jg      @@continue
        mov     ax, [flat+si+2]
        or      ax, ax
        js      @@continue
        cmp     ax, 199
        jg      @@continue
        xchg    ah, al
        add     di, ax
        shr     ax, 2
        add     di, ax
        mov     al, [byte flat+si+4]
        mov     [fs:di], al
@@continue:
        sub     si, 6
        jns     @@next_pixel

; flip screen
        cmp     [byte retrace+bp-udata_start], 0
        jnz     @@no_retrace
        mov     dx, 03dah
@@retrace:
        in      al, dx
        test    al, 8
        jz      @@retrace
@@no_retrace:
        inc     [dword num_frames+bp-udata_start]
        xor     si, si
        xor     di, di
        mov     cx, 16000
        segfs
        rep     movsd

        inc     [word alpha+bp-udata_start]
        dec     [word beta+bp-udata_start]
        add     [word gamma+bp-udata_start], 2
        add     [word delta+bp-udata_start], 2
        dec     [word epsilon+bp-udata_start]

        lea     si, [alpha+bp-udata_start]
        mov     cx, 5
@@truncate:
        and     [word si], ANGLE_GRADATIONS-1
        inc     si
        inc     si
        loop    @@truncate

        mov     ah, 01h
        int     16h
        jz      go
        xor     ah, ah
        int     16h
        inc     [byte halting+bp-udata_start]
        jmp     go

halt:
        mov     ax, 0003h
        int     10h

        lea     dx, [copyright+bp-data_start]
        call    print

        fninit
        fild    [dword num_frames+bp-udata_start]
        xor     cx, cx
        call    print_dec
        lea     dx, [frames+bp-data_start]
        call    print
        mov     eax, [gs:006ch]
        sub     [start+bp-udata_start], eax
        fild    [dword start+bp-udata_start]
        fdiv    [dword tps+bp-data_start]
        mov     cx, 3
        call    print_dec
        lea     dx, [seconds+bp-data_start]
        call    print
        fdivp
        fimul   [word const_10000+bp-data_start]
        mov     cx, 3
        call    print_dec
        lea     dx, [fps+bp-data_start]
        call    print
        int     20h

print:
        mov     ah, 09h
        int     21h
        ret

print_dec:
        fist    [dword temp_dword+bp-udata_start]
        mov     eax, [temp_dword+bp-udata_start]
        mov     ebx, 10
@@convert:
        jcxz    @@no_point
        dec     cx
        jnz     @@no_point
        mov     dl, '.'-'0'
        push    dx offset       @@done
@@no_point:
        cdq
        div     ebx
        or      eax, eax
        push    dx
        jz      @@done
        call    @@convert
@@done:
        pop     ax
        add     al, '0'
        int     29h
        ret

create_torus:
        mov     cx, 29
@@outer:
        fninit
        mov     [temp_word+bp-udata_start], cx
        fild    [word temp_word+bp-udata_start]
        fmul    [dword pi_d_15+bp-data_start]
        fsincos
        fld     st(1)
        fimul   [word const_50+bp-data_start]
        fld     st(1)
        fimul   [word const_50+bp-data_start]

; center_x center_y cos sin
        mov     dx, 9
@@inner:
        mov     [temp_word+bp-udata_start], dx
        fild    [word temp_word+bp-udata_start]
        fmul    [dword pi_d_5+bp-data_start]
        fcos
        fld     st(3)
        fimul   [word const_20+bp-data_start]
        fmulp
        fadd    st, st(1)
        fstp    [dword di]

        fild    [word temp_word+bp-udata_start]
        fmul    [dword pi_d_5+bp-data_start]
        fcos
        fld     st(4)
        fimul   [word const_20+bp-data_start]
        fmulp
        fadd    st, st(2)
        fstp    [dword di+4]

        fild    [word temp_word+bp-udata_start]
        fmul    [dword pi_d_5+bp-data_start]
        fsin
        fimul   [word const_20+bp-data_start]
        fstp    [dword di+8]

        add     di, 12
        dec     dx
        jns     @@inner
        dec     cx
        jns     @@outer

        ret

create_sphere:
        mov     cx, 14
@@outer:
        mov     [temp_word2+bp-udata_start], cx
        fild    [word temp_word2+bp-udata_start]
        fmul    [dword pi_d_14+bp-data_start]
        fsincos
        fimul   [word const_50+bp-data_start]
        fxch
        fimul   [word const_50+bp-data_start]

; cur_radius center
        mov     [word temp_word+bp-udata_start], 20
@@inner:
        fild    [word temp_word+bp-udata_start]
        fmul    [dword pi_d_10+bp-data_start]
        fiadd   [word temp_word2+bp-udata_start]
        fsincos
        fmul    st, st(2)
        fstp    [dword di]
        fmul    st, st(1)
        fstp    [dword di+4]
        fxch
        fst     [dword di+8]
        fxch

        add     di, 12
        dec     [word temp_word+bp-udata_start]
        jnz     @@inner
        fstp
        fstp
        dec     cx
        jns     @@outer

        ret

create_cone:
        mov     cx, 14
@@outer:
        mov     [temp_word2+bp-udata_start], cx
        fild    [word temp_word2+bp-udata_start]
        fimul   [word const_8+bp-data_start]
        fld     st
        fisub   [word const_50+bp-data_start]
        fstp    [dword temp_dword2+bp-udata_start]
        mov     eax, [temp_dword2]
        fidiv   [word const_2+bp-data_start]
        mov     [word temp_word+bp-udata_start], 19
@@inner:
        fild    [word temp_word+bp-udata_start]
        fmul    [dword pi_d_10+bp-data_start]
        fiadd   [word temp_word2+bp-udata_start]
        fsincos
        fmul    st, st(2)
        fstp    [dword di]
        fmul    st, st(1)
        fstp    [dword di+4]

        mov     [di+8], eax

        add     di, 12
        dec     [word temp_word+bp-udata_start]
        jns     @@inner
        fstp
        dec     cx
        jns     @@outer

        ret

create_cube:
        lea     si, [cube_data]

interprete:
        xor     ax, ax
        xor     cx, cx
        lodsb
        fastimul bx, ax, 3
        lodsb
        mov     cl, al
        lodsw
        mov     dx, ax
        mov     [temp_word3+bp-udata_start], si
        add     si, bx
@@next_segment:
        fninit
        push    cx
        mov     cx, 2
@@next_point:
        xor     ax, ax
        lodsb
        fastimul bx, ax, 3
        lea     bx, [bx+2]
        add     bx, [temp_word3+bp-udata_start]
        push    cx
        mov     cx, 3
@@next_coord:
        mov     al, [bx]
        cbw
        mov     [temp_word+bp-udata_start], ax
        fild    [word temp_word+bp-udata_start]
        dec     bx
        loop    @@next_coord
        pop     cx
        loop    @@next_point

        mov     cx, dx
        mov     [temp_word+bp-udata_start], cx

@@next_pnt:
        mov     [temp_word2+bp-udata_start], cx
        fld     st(3)
        fld     st(1)
        call    store_point
        fld     st(4)
        fld     st(2)
        call    store_point
        fld     st(5)
        fld     st(3)
        call    store_point
        loop    @@next_pnt
        pop     cx
        loop    @@next_segment
        ret

store_point:
        fsub    st, st(1)
        fimul   [word temp_word2+bp-udata_start]
        fidiv   [word temp_word+bp-udata_start]
        faddp
        fstp    [dword di]
        add     di, 4
        ret

create_dodecaedr:
        lea     si, [dodecaedr_data]
        jmp     interprete

create_icosaedr:
        lea     si, [icosaedr_data]
        jmp     interprete

create_tetraedr:
        lea     si, [tetraedr_data]
        jmp     interprete

create_octaedr:
        lea     si, [octaedr_data]
        jmp     interprete

morph:
        fninit
        lea     si, [morph_from+bp-udata_start]
        mov     bx, [morph_to+bp-udata_start]
        lea     di, [morphed]
        mov     cx, NUM_POINTS*3
@@morph:
        fld     [dword bx]
        fld     [dword si]
        fsub    st, st(1)
        fidiv   [word morph_time+bp-data_start]
        fimul   [word counter+bp-udata_start]
        faddp
        fstp    [dword di]
        add     bx, 4
        add     si, 4
        add     di, 4
        loop    @@morph

do_nothing:
        dec     [word counter+bp-udata_start]
        jz      start_phase
        ret

start_phase:
        inc     [word phase+bp-udata_start]
        test    [word phase+bp-udata_start], 1
        jnz     @@create_object

        mov     al, [palette+bp-data_start]
        mov     ah, al
        and     ah, 00000111b
        shl     al, 3
        or      al, ah
        mov     [palette+bp-data_start], al
        mov     si, [morph_to+bp-udata_start]
        lea     di, [morph_from]
        mov     cx, NUM_POINTS*3
        push    es ds
        pop     es
        rep     movsd
        pop     es
        mov     [word counter+bp-udata_start], TIMEOUT
        ret
@@create_object:
        mov     [word counter+bp-udata_start], MORPH_TIME
        call    random
        mov     bx, ax
        shr     bx, 3
        and     bx, NUM_OBJECTS-1
        and     ah, 00000111b
        jnz     @@no_inc
        inc     ah
@@no_inc:
        and     ah, 00000111b
        mov     al, [palette+bp-data_start]
        shl     al, 3
        or      al, ah
        mov     [palette+bp-data_start], al
        imul    bx, 3600
        add     bx, offset      objects
        mov     [morph_to], bx
        ret

random:
        mov     eax, [randseed+bp-udata_start]
        imul    eax, MAGIC_CONSTANT1
        add     eax, MAGIC_CONSTANT2
        mov     [randseed+bp-udata_start], eax
        ret

        end
