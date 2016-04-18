;***********************************
; RGB.ASM from TASMED examples.
; Copyright (C) 1997 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

; Assembler-C link demo

; How to compile this project in TASMED:
;  Compile ELLIPSE.C with any C compiler
;  Press Alt+P F to setup project files
;  Add RGB.ASM and ELLIPSE.OBJ to project
;  Set RGB.ASM as primary file if not set already
;  Press OK button
;  Save this project as RGB.PRJ
;  Project is ready to compilation and execution

        p386
        ideal
        model   tiny, c

vid_mode        = 0103h
vid_width       = 800
vid_height      = 600

;vid_mode        = 0013h
;vid_width       = 320
;vid_height      = 200

radius          = vid_height/4

dataseg
pal             db 0, 0, 0
                db 63, 0, 0
                db 0, 63, 0
                db 63, 63, 0
                db 0, 0, 63
                db 63, 0, 63
                db 0, 63, 63
                db 63, 63, 63
vid_bank        dw -1

codeseg
        extrn   c fillellipse:near

        public  hline
        public  hlpair

        startupcode

if      vid_mode lt 0100h
        mov     ax, vid_mode
else
        mov     ax, 4f02h
        mov     bx, vid_mode
endif
        int     10h

        xor     ax, ax
        mov     cx, 8*3
        lea     si, [pal]
        mov     dx, 03c8h
        out     dx, al
        inc     dx
        rep     outsb

        push    0a000h
        pop     es

        call    fillellipse, vid_width/2, vid_height/2-radius*577/1000, radius*vid_width*3/4/vid_height, radius, 1
        call    fillellipse, vid_width/2-radius/2, vid_height/2+radius*577/2000, radius*vid_width*3/4/vid_height, radius, 2
        call    fillellipse, vid_width/2+radius/2, vid_height/2+radius*577/2000, radius*vid_width*3/4/vid_height, radius, 4

        xor     ah, ah
        int     16h

        mov     ax, 0003h
        int     10h

        ret

proc    hline near      $$x1:word, $$y:word, $$x2:word, $$color:byte
uses    di
        mov     ax, [$$y]
        mov     bx, vid_width
        mul     bx
        mov     di, [$$x1]
        mov     cx, [$$x2]
        sub     cx, di
        inc     cx
        add     di, ax
        adc     dx, 0
        cmp     dx, [vid_bank]
        je      @@sbs1
        mov     ax, 4f05h
        xor     bx, bx
        mov     [vid_bank], dx
        int     10h
@@sbs1:
        mov     al, [$$color]
@@putpixel:
        add     [es:di], al
        inc     di
        jnz     @@sbs2
        push    ax
        inc     [vid_bank]
        mov     dx, [vid_bank]
        mov     ax, 4f05h
        xor     bx, bx
        int     10h
        pop     ax
@@sbs2:
        loop    @@putpixel
        ret
endp    hline

proc    hlpair near $$x:word, $$dx:word, $$y:word, $$dy:word, $$color:byte
uses    di
        mov     di, [$$x]
        mov     cx, [$$dx]
        sub     di, cx
        shl     cx, 1
        inc     cx
        mov     ax, [$$y]
        push    ax cx di
        sub     ax, [$$dy]
        mov     bx, vid_width
        mul     bx
        add     di, ax
        adc     dx, 0
        cmp     dx, [vid_bank]
        je      @@sbs1
        mov     ax, 4f05h
        xor     bx, bx
        mov     [vid_bank], dx
        int     10h
@@sbs1:
        mov     al, [$$color]
@@putpixel1:
        add     [es:di], al
        inc     di
        jnz     @@sbs2
        push    ax
        inc     [vid_bank]
        mov     dx, [vid_bank]
        mov     ax, 4f05h
        xor     bx, bx
        int     10h
        pop     ax
@@sbs2:
        loop    @@putpixel1
        pop     di      cx ax
        add     ax, [$$dy]
        mov     bx, vid_width
        mul     bx
        add     di, ax
        adc     dx, 0
        cmp     dx, [vid_bank]
        je      @@sbs3
        mov     ax, 4f05h
        xor     bx, bx
        mov     [vid_bank], dx
        int     10h
@@sbs3:
        mov     al, [$$color]
@@putpixel2:
        add     [es:di], al
        inc     di
        jnz     @@sbs4
        push    ax
        inc     [vid_bank]
        mov     dx, [vid_bank]
        mov     ax, 4f05h
        xor     bx, bx
        int     10h
        pop     ax
@@sbs4:
        loop    @@putpixel2
        ret
endp    hlpair

        end
