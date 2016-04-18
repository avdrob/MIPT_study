;***********************************
; TRACER.ASM from TASMED examples.
; Copyright (C) 1997 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

; Sets TF flag and displays current CS:IP in top-left corner of the screen.

        p386
        ideal
        model   tiny

attr            = 1fh
code            = 'TRCR'

macro   writeln s:rest
local   @@1, @@2
        call    @@2
@@1             db s, 0dh, 0ah
@@2:
        pop     dx
        mov     ah, 40h
        xor     bx, bx
        mov     cx, @@2-@@1
        int     21h
endm    writeln

codeseg
        startupcode

        jmp     @start

        assume  ds:nothing, es:nothing, ss:nothing

tsrlabel        dd code

_displayhex:
        mov     cx, 0404h
@@nc:
        rol     ax, cl
        push    ax
        and     ax, 000fh
        aaa
        aad     17
        mov     ah, attr
        add     al, '0'
        stosw
        pop     ax
        dec     ch
        jnz     @@nc
        ret

_int01h:
        push    bp
        mov     bp, sp
        pusha
        push    es 0b800h
        pop     es
        cld
        xor     di, di
        mov     ax, [bp+4]
        call    _displayhex
        mov     ax, attr shl 8+':'
        stosw
        mov     ax, [bp+2]
        call    _displayhex
        pop     es
        popa
        pop     bp
        iret

_int08h:
        push    bp
        mov     bp, sp
        or      [byte bp+7], 1
        pop     bp
                db 0eah
label   oldint08h       dword
oldint08h_ofs   dw ?
oldint08h_seg   dw ?

        assume  ds:@data, es:@data, ss:@stack

@start:
        mov     ax, [002ch]
        mov     es, ax
        mov     ah, 49h
        int     21h

        mov     ax, 3508h
        int     21h
        cmp     [es:tsrlabel], code
        je      @uninstall

        mov     [oldint08h_ofs], bx
        mov     [oldint08h_seg], es

        mov     ax, 2508h
        lea     dx, [_int08h]
        int     21h

        mov     ax, 2501h
        lea     dx, [_int01h]
        int     21h

@install:
        pushf
        pop     ax
        or      ah, 1
        push    ax
        popf

        writeln "Installed, re-run to uninstall."

        lea     dx, [@start]
        int     27h

@uninstall:
        pushf
        pop     ax
        and     ah, not 1
        push    ax
        popf

        mov     ax, 2508h
        lds     dx, [es:oldint08h]
        int     21h

        push    cs
        pop     ds
        writeln "Uninstalled."

        mov     ah, 49h
        int     21h

        int     20h

        end
