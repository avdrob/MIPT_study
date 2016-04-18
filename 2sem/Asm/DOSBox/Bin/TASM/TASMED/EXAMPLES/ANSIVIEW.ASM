;*****************************************
; ANSIVIEW (Tiny ANSI interpreter).
; Copyright (C) 12/09/1997 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;*****************************************

; Warning: this demo using copper effect and temporarily disables Windows 95
; multitasking. So use carefully.

        p386
        ideal
        model   tiny

MAX_TEXT_SIZE   = 56*1024
MAX_ANSI_PARAMS = 16
SCREEN_SIZE     = 8000h
LINE_LEN_C      = 80
LINE_LEN_B      = LINE_LEN_C*2
ROWS            = 24
DISPLAY_START_L = 4
DISPLAY_START_B = LINE_LEN_B*DISPLAY_START_L
DISPLAY_START_P = DISPLAY_START_L shl 4
MAX_ORIGIN      = (ROWS+1) shl 4
MIN_ORIGIN      = MAX_ORIGIN-DISPLAY_START_P-1

k_ESC           = 01h
k_SPACE         = 39h
k_UP            = 48h
k_DOWN          = 50h
k_PGUP          = 49h
k_PGDN          = 51h
k_HOME          = 47h
k_END           = 4fh
k_ENTER         = 1ch

dataseg
tbl_ansi_code   db 'HfABCDJKsum', 0
tbl_ansi_jmp    dw _curpos, _curpos, _moveup, _movedown, _moveright, _moveleft
                dw _clrscr, _clreoln, _savepos, _restpos, _attr
tbl_key_code    db k_UP, k_DOWN, k_SPACE, k_ENTER, k_ESC, k_HOME
                db k_END, k_PGDN, k_PGUP, 0
tbl_key_jmp     dw _up, _down, _stop, _panel, _exit, _home, _end, _pgdn, _pgup
tbl_color_xlat  db 0, 4, 2, 6, 1, 5, 3, 7
limit           dw 0
panel_dir       dw -1
panel_ofs       dw MAX_ORIGIN
info            db 0dh, '[0m[3C[1;33m[1;37mUp Down PgUp PgDn Home End[1;31m Scroll  [1;37mEnter[31m Panel on/off  [1;37mSpace[31m Stop[1;37m  Esc[1;31m Exit'
                db 0dh, '[6C[1;36mANSIVIEW[0;37m is [1;31mtiny [37mANSI [31mviewer [0;37mcoded by [1;37mCodeMaster[0;37m. [1;33mFidonet: [37m2:5002/39[0m', 1ah

udataseg
text            db MAX_TEXT_SIZE dup (?)
attr            db ?
ansi_params     db MAX_ANSI_PARAMS dup (?)
ansi_numparams  dw ?
saved           dw ?
pal             db 768 dup (?)
cpal            db 768 dup (?)

codeseg
        startupcode

        cld

        mov     ax, 1681h
        int     2fh

        mov     ax, 0003h
        int     10h

        mov     ah, 01h
        mov     cx, 2020h
        int     10h

        movzx   bx, [0080h]
        mov     [byte bx+0081h], 0

        mov     ax, 3d00h
        mov     dx, 0082h
        int     21h
        jc      @@getpal

        mov     bx, ax
        mov     ah, 3fh
        lea     dx, [text]
        mov     cx, -1
        int     21h
        mov     si, ax
        mov     [text+si], 1ah

        mov     ah, 3eh
        int     21h

@@getpal:
        xor     ax, ax
        lea     di, [pal]
        mov     cx, 768
        mov     dx, 03c7h
        out     dx, al
        inc     dx
        inc     dx
        rep     insb

        lea     di, [cpal]
        mov     cx, 384
        xor     ax, ax
        rep     stosw
        call    _setcpal

        mov     ah, 01001100b
        call    _write8042

        push    0b800h
        pop     es

        mov     dx, DISPLAY_START_P
        call    _vsplit

        xor     di, di
        mov     ax, 0720h
        mov     cx, LINE_LEN_C*DISPLAY_START_L-16
        rep     stosw

        xor     di, di
        lea     si, [info]
        call    _interprete

        lea     si, [text]
        mov     di, DISPLAY_START_B
        call    _interprete

@@fadein:
        xor     bp, bp

        lea     si, [cpal]
        lea     di, [pal]
        mov     cx, 768
@@fadein_loop:
        lodsb
        cmp     al, [di]
        je      @@no_inc
        inc     bp
        inc     [byte si-1]
@@no_inc:
        inc     di
        loop    @@fadein_loop

        call    _setcpal
        or      bp, bp
        jnz     @@fadein

@@move_panel:
        mov     dx, DISPLAY_START_P
        call    _vsplit
        cmp     [panel_ofs], MIN_ORIGIN
        jne     @@move_panel

        mov     ax, [limit]
        xor     dx, dx
        mov     bx, LINE_LEN_B
        div     bx
        add     ax, DISPLAY_START_L-ROWS
        inc     ax
        sal     ax, 4
        cmp     ax, DISPLAY_START_P
        jge     @@unsigned
        mov     ax, DISPLAY_START_P
@@unsigned:
        mov     bp, ax

        xor     ax, ax
        mov     dx, DISPLAY_START_P
        mov     di, 2
        mov     bx, bp
        jmp     @draw

@init:
        mov     dx, DISPLAY_START_P

_stop:
        xor     di, di
        xor     bx, bx

@draw:
        cmp     dx, DISPLAY_START_P
        jl      @init
        cmp     dx, bp
        jle     @@draw_waitkey
        mov     dx, bp
        jmp     _stop

@@draw_waitkey:
        pusha
        call    _vsplit
        popa

@@wait_kbd:
        in      al, 60h
        cmp     al, 0e0h
        je      @@move
        or      al, al
        jns     @@interprete
        xor     ah, ah

@@move:
        add     dx, di
        cmp     dx, bx
        je      _stop
        jmp     @draw

@@interprete:
        cmp     al, ah
        je      @@move
        mov     ah, al
        mov     [limit], bx
        lea     si, [tbl_key_code]
        lea     bx, [tbl_key_jmp]
        call    _scantbl
        jmp     @@move

_pgup:
        mov     bx, dx
        sub     bx, (ROWS-DISPLAY_START_L)      shl 4
        mov     di, -8
        ret

_pgdn:
        mov     bx, (ROWS-DISPLAY_START_L)      shl 4
        add     bx, dx
        mov     di, 8
        ret

_home:
        xor     bx, bx
        mov     di, -16
        ret

_end:
        mov     bx, bp
        mov     di, 16
        ret

_up:
        xor     bx, bx
        dec     di
        dec     di
        cmp     di, -16
        jg      @@ret
        mov     di, -16
@@ret:
        ret

_down:
        mov     bx, bp
        inc     di
        inc     di
        cmp     di, 16
        jl      @@ret
        mov     di, 16
@@ret:
        ret

_panel:
        mov     bx, [limit]
        mov     cx, [panel_dir]
        jcxz    @@move
@@neg:
        neg     cx
@@ret:
        mov     [panel_dir], cx
        ret
@@move:
        inc     cx
        cmp     [panel_ofs], MAX_ORIGIN
        je      @@neg
        jmp     @@ret

_exit:
        cmp     [panel_ofs], MAX_ORIGIN
        je      @@halt
        mov     [panel_dir], 1
        push    dx
        call    _vsplit
        pop     dx
        jmp     _exit

@@halt:
        push    ds
        pop     es

        lea     si, [pal]
        lea     di, [cpal]
        mov     cx, 384
        rep     movsw

@@fadeout:
        xor     bp, bp
        lea     si, [cpal]
        mov     di, si
        mov     cx, 768

@@fadeout_loop:
        lodsb
        or      al, al
        jz      @@no_dec
        inc     bp
        dec     al
@@no_dec:
        stosb
        loop    @@fadeout_loop

        call    _setcpal
        or      bp, bp
        jnz     @@fadeout

        mov     ax, 0003h
        int     10h

        mov     ah, 01001101b
        call    _write8042

        mov     ax, 1682h
        int     2fh

        int     20h

_setcpal:
        call    _vsync
        lea     si, [cpal]
        xor     ax, ax
        mov     cx, 768
        mov     dl, 0c8h
        out     dx, al
        inc     dx
        rep     outsb
        in      al, 60h
        ret

_vsync:
        mov     dx, 03dah
@@1:
        in      al, dx
        test    al, 08h
        jz      @@1
@@2:
        in      al, dx
        test    al, 08h
        jnz     @@2
        ret

_skipline:
        mov     dl, 0dah
@@1:
        in      al, dx
        test    al, 01h
        jnz     @@1
@@2:
        in      al, dx
        test    al, 01h
        jz      @@2
        ret

_vsplit:
        cli

        mov     ch, dl
        mov     cl, -1
        and     ch, 0fh
        and     dl, 0f0h
        fastimul bx, dx, LINE_LEN_C shr 4

        call    _vsync

        pusha
        mov     cx, [panel_ofs]
@@skip:
        call    _skipline
        loop    @@skip

        mov     cx, MAX_ORIGIN
        sub     cx, [panel_ofs]
        jle     @@no_copper
        mov     bl, 63
@@line:
        call    _skipline
        mov     dl, 0c8h
        xor     al, al
        out     dx, al
        inc     dx
        mov     al, bl
        out     dx, al
        xor     al, al
        out     dx, al
        mov     al, 63
        sub     al, bl
        out     dx, al
        dec     bl
        loop    @@line

        dec     dl
        xor     al, al
        out     dx, al
        inc     dx
        out     dx, al
        out     dx, al
        out     dx, al

@@no_copper:
        mov     cx, [panel_ofs]
        add     cx, [panel_dir]
        mov     [panel_ofs], cx
        cmp     cx, MAX_ORIGIN
        je      @@zero
        cmp     cx, MIN_ORIGIN
        jne     @@no
@@zero:
        mov     [panel_dir], 0
@@no:

        mov     bx, cx
        push    bx
        mov     dl, 0d4h
        mov     al, 09h
        out     dx, al
        inc     dx
        in      al, dx
        shr     bx, 9
        and     al, 0bfh
        add     bl, al
        dec     dx
        mov     al, 09h
        out     dx, al
        inc     dx
        mov     al, bl
        out     dx, al
        dec     dx
        pop     bx

        mov     al, 18h
        out     dx, al
        inc     dx
        mov     al, bl
        out     dx, al
        dec     dx
        mov     al, 07h
        out     dx, al
        inc     dx
        in      al, dx
        dec     dx
        mov     cl, al
        mov     al, 07h
        out     dx, al
        mov     al, cl
        and     al, 0efh
        add     al, bh
        inc     dx
        out     dx, al
        popa

        mov     dl, 0d4h
        mov     ah, bh
        mov     al, 0ch
        out     dx, ax
        mov     ah, bl
        inc     al
        out     dx, ax

        mov     dl, 0d4h
        mov     ah, ch
        mov     al, 08h
        out     dx, ax
        mov     dl, 0c0h
        mov     al, 33h
        out     dx, al
        mov     al, cl
        out     dx, al

        sti

        ret

_getlineofs:
        mov     ax, di
        xor     dx, dx
        mov     bx, LINE_LEN_B
        div     bx
        mul     bx
@ret:
        ret

_interprete:
@@next_char:
        cmp     di, SCREEN_SIZE
        jae     @ret

        lodsb
        cmp     al, 1ah
        je      @ret
        cmp     al, 1bh
        je      @@esc
        cmp     al, 0dh
        je      @@next_line
        cmp     al, 0ah
        je      @@next_char
        mov     ah, [attr]
        stosw
        call    _getlineofs
        cmp     ax, [limit]
        jb      @@no_limit
        mov     [limit], ax
@@no_limit:
        jmp     @@next_char

@@next_line:
        call    _getlineofs
        add     ax, LINE_LEN_B
        mov     di, ax
        jmp     @@next_char

@@esc:
        lodsb
        cmp     al, '['
        jne     @@next_char

        lea     bp, [ansi_params]
        xor     ax, ax
        xor     cx, cx
@@next_param:
        xor     bx, bx
@@parse_param:
        lodsb
        cmp     al, ';'
        je      @@end_param
        cmp     al, '0'
        jb      @@end_param
        cmp     al, '9'
        ja      @@end_param
        inc     cx
        sub     al, '0'
        imul    bx, bx, 10
        add     bx, ax
        jmp     @@parse_param
@@end_param:
        jcxz    @@no_param
        mov     [bp], bl
        inc     bp
@@no_param:
        cmp     al, ';'
        je      @@next_param

        sub     bp, offset      ansi_params
        mov     [ansi_numparams], bp
        push    si
        lea     si, [tbl_ansi_code]
        lea     bx, [tbl_ansi_jmp]
        call    _scantbl
        pop     si
        jmp     @@next_char

_scantbl:
        cmp     [byte si], 0
        je      @ret3
        cmp     al, [si]
        je      @@found
        inc     si
        inc     bx
        inc     bx
        jmp     _scantbl
@@found:
        jmp     [word bx]

_curpos:
        mov     ax, 1
        mov     bx, ax
        cmp     [ansi_numparams], 0
        je      @@goto
        mov     al, [ansi_params]
        cmp     [ansi_numparams], 1
        je      @@goto
        mov     bl, [ansi_params+1]
@@goto:
        mov     di, bx
        dec     ax
        dec     di
        mov     dx, LINE_LEN_B
        mul     dx
        shl     di, 1
        add     di, ax
        add     di, DISPLAY_START_B
@ret3:
        ret

_moveup:
        mov     [byte @move+1], 0efh    ; add   di, const       postbyte

_move:
        mov     cx, 1
        cmp     [ansi_numparams], 0
        je      @move
        mov     cl, [ansi_params]
@move:
        sub     di, LINE_LEN_B
        loop    @move

@check_di:
        cmp     di, DISPLAY_START_B
        jge     @@normal_di
        mov     di, DISPLAY_START_B
@@normal_di:
        ret

_movedown:
        mov     [byte @move+1], 0c7h    ; sub   di, const       postbyte
        jmp     _move

_moveright:
        mov     [byte @moveside], 03h   ; add di, bx    opcode

_moveside:
        mov     bx, 1
        cmp     [ansi_numparams], 0
        jz      @@no_params
        mov     bl, [ansi_params]
@@no_params:
        shl     bx, 1

@moveside:
        add     di, bx
        ret

_moveleft:
        mov     [byte @moveside], 2bh   ; add di, bx    opcode
        call    _moveside
        jmp     @check_di

_clrscr:
        cmp     [ansi_numparams], 1
        jne     @ret2
        cmp     [ansi_params], 2
        jne     @ret2
        mov     di, DISPLAY_START_B

_clearscreen:
        push    di
        mov     cx, SCREEN_SIZE
@put:
        sub     cx, di
        shr     cx, 1
        mov     ah, [attr]
        mov     al, ' '
        rep     stosw
        pop     di
@ret2:
        ret

_clreoln:
        call    _getlineofs
        push    di
        mov     cx, ax
        add     cx, LINE_LEN_B
        jmp     @put

_savepos:
        mov     [saved], di
        ret

_restpos:
        mov     di, [saved]
        ret

_attr:
        lea     si, [ansi_params]
        mov     cx, [ansi_numparams]
@@next:
        mov     al, [si]
        mov     bl, [attr]
        lodsb
        cmp     al, 30
        jb      @@special
        cmp     al, 40
        jae     @@bkg
        sub     al, 30
        call    _xlat_color
        and     bl, 0f8h
        jmp     @@set_loop
@@bkg:
        sub     al, 40
        call    _xlat_color
        and     bl, 8fh
        shl     al, 4
@@set_loop:
        or      bl, al
        jmp     @@loop
@@special:
        or      al, al
        jnz     @@no_normal
        mov     bl, 07h
        jmp     @@loop
@@no_normal:
        cmp     al, 1
        jne     @@no_bold
        or      bl, 08h
        jmp     @@loop
@@no_bold:
        cmp     al, 5
        jne     @@no_blink
        or      bl, 80h
        jmp     @@loop
@@no_blink:
        cmp     al, 7
        jne     @@no_reverse
        mov     bl, 70h
        jmp     @@loop
@@no_reverse:
        and     bl, 0f0h
        mov     dl, bl
        shr     dl, 4
        or      bl, dl
@@loop:
        mov     [attr], bl
        loop    @@next
        ret

_xlat_color:
        push    bx
        lea     bx, [tbl_color_xlat]
        xlat
        pop     bx
        ret

_write8042:
        cli
        mov     al, 60h
        out     64h, al
@@1:
        in      al, 64h
        test    al, 00000010b
        jnz     @@1
        mov     al, ah
        out     60h, al
        sti
        ret

        end
