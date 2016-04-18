;***********************************
; INTRO.ASM from TASMED examples.
; Copyright (C) 1997 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

; A lot of people ask me for this source. So here you are. I know, it's not a
; good style, but I don't use comments in such tiny and simple programs like
; that.

;                                                  Happy hacking. CodeMaster.

        p386
        ideal
        model   tiny

MAGIC_CONSTANT1 = 1103515245
MAGIC_CONSTANT2 = 12345
MSK             = 3fh
HALT_DELAY      = 80

MOUSE_SENSITIVITY = 64

$CM$_HEIGHT     = 5
$CM$_OFFSET     = 190*320+250
$CM$_COLOR      = 85

dataseg
s               db 6, "Eugene"
                db 7, "Nonko"
                db 3, "also known as"
                db 4, "CodeMaster"
                db 3, "presents"
                db 6, "TASMED"
                db 4, "the"
                db 4, "ultimate"
                db 4, "workshell"
                db 4, "for"
                db 4, "your"
                db 4, "assembly"
                db 4, "language"
                db 4, "projects"
                db 0
                db 6, "TASMED"
                db 6, "is"
                db 5, "freeware"
                db 6, "enjoy!"
                db 0
                db 7, "LMD"
                db 6, "4ever!"
                db 0
s_ofs           dw offset s
cycle           db 0
keypressed      db 0
$cm$            dq 01110001110011110011111010001000111001111011111011111011110b
                dq 10001010001010001010000011011001001010000000100010000010001b
                dq 10000010001010001011110010101010001001110000100011110010001b
                dq 10001010001010001010000010001011111000001000100010000011110b
                dq 01110001110011110011111010001010001011110000100011111010001b

udataseg
randseed        dd ?
mousex          dw ?
mousey          dw ?
mousebuttons    dw ?
pal             db 768 dup (?)
label   font    dword
font_ofs        dw ?
font_seg        dw ?
stackseg        dw 200  dup (?)
label   stack_top
label   program_end

codeseg
        startupcode

                db "CODEMASTER ", 1ah

        cld

        mov     ax, 1681h
        int     2fh

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

        mov     ax, 1130h
        mov     bh, 06h
        int     10h
        mov     [font_ofs], bp
        mov     [font_seg], es

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

@@main_loop:
        push    fs
        pop     es
        inc     [cycle]

        cmp     [keypressed], 0
        jz      @@out_text
        dec     [keypressed]
        jnz     @@process_screen

        mov     ax, 0003h
        int     10h

        mov     ax, 1682h
        int     2fh

        int     20h

@@out_text:
        mov     cl, [cycle]
        test    cl, MSK
        jnz     @@skip_text

        mov     si, [s_ofs]
        push    si
        xor     cx, cx
        lodsb
@@char:
        inc     cx
        lodsb
        cmp     al, 20h
        jae     @@char
        lea     bx, [s_ofs]
        cmp     si, bx
        jb      @@no_end
        lea     si, [s]
@@no_end:
        mov     [s_ofs], si
        dec     [s_ofs]
        pop     si
        dec     cx
        jcxz    @@skip_text
        lodsb
        movzx   bp, al
        mov     ax, bp
        shl     ax, 2
        mul     cx
        mov     bx, 160
        sub     bx, ax
        add     bx, 320*120
        push    cx
        mov     cx, bp
@@sub_320_8:
        sub     bx, 320*8
        loop    @@sub_320_8
        pop     cx

@@char_loop:
        lodsb
        push    ds
        pusha
        xor     ah, ah
        shl     ax, 4
        lds     si, [font]
        add     si, ax
        mov     cx, 16
@@height_loop:
        lodsb
        push    cx bx
        mov     cx, 8
        mov     ah, al
        mov     al, -2
@@width_loop:
        shl     ah, 1
        jnc     @@skip_bar
        mov     di, bx
        push    cx
        mov     cx, bp
@@hor_line:
        push    cx
        mov     cx, bp
        rep     stosb
        pop     cx
        add     di, 320
        sub     di, bp
        loop    @@hor_line
        pop     cx
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
        pop     ds
        push    cx
        mov     cx, bp
        shl     cx, 3
        add     bx, cx
        pop     cx
        loop    @@char_loop

@@skip_text:
        call    _random
        mov     cx, ax
        and     cx, 001fh
@@next_line:
        push    cx
        call    _random
        and     ax, 001fh
        inc     ax
        shl     ax, 1
        mov     si, ax
        call    _random
        xor     dx, dx
        mov     cx, 300
        div     cx
        mov     di, dx
        add     di, 320*200
        call    _random
        or      al, 80h
        dec     al
        mov     cx, si
@@line:
        stosb
        mov     [es:di+320-1], al
        mov     [es:di+320*2-1], al
        mov     [es:di+320*3-1], al
        loop    @@line
        pop     cx
        loop    @@next_line

@@process_screen:
        push    es
        pop     ds
        mov     di, 320*2
@@process_loop:
        xor     ax, ax
        mov     cl, [cs:cycle]
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
        je      @@main_loop
        mov     [gs:001ch], ax

@@halt:
        cmp     [keypressed], 0
        jnz     @@main_loop

        push    fs
        pop     es
        mov     di, 320*200
        mov     cx, 960
        xor     ax, ax
        rep     stosw
        mov     [keypressed], HALT_DELAY
        jmp     @@main_loop

_random:
        mov     eax, [cs:randseed]
        imul    eax, MAGIC_CONSTANT1
        add     eax, MAGIC_CONSTANT2
        mov     [cs:randseed], eax
        ret

        end
