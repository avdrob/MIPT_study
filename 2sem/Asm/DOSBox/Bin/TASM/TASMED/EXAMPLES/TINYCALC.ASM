;***********************************
; TINYCALC, version 2.5.
; Copyright (C) 1998 by CodeMaster.
; Fidonet: 2:5002/39.
; E-mail: cm@liceum.secna.ru.
;***********************************

; Tiny calculator written on pure assembly just for fun.
;
; Features
; ~~~~~~~~
;  Extreme tiny file size  (314 bytes or  251 bytes with error checking
;   disabled).
;  Integer  and  floating-point  numbers  (for example: 2 and 2.1). All
;   leading  zeroes  before  decimal point can be skipped (example: .2).
;   Exponential form is unsupported.
;  Four  binary arithmetic operations: addition, subtraction, multipli-
;   cation  and  division. Unary operators (minus and plus) are not sup-
;   ported.
;  Brackets  with  small  nesting  level  (up to 2-3). Nesting level is
;   limited by FPU stack size.
;  Result output with eight digits after decimal point.
;
; Usage example (error checking enabled)
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; C:>tinycalc.com
;
; > 2+3*4
; 14.00000000
; > 1.2*3.4
; 4.08000000
; > 2*(1+3)
; 8.00000000
; > 1.1-2.2*3.3/4.4-5.5
; -6.05000000
; > 1+(2*(1+2*(1+1)))
; 22.00000000
; > 1/(1-1)
; ?
; > 1/
; ?
; > -2
; ?
; > help
; ?
; > how can I exit?!?!
; ?
; >
; C:\>

        p386
        ideal
        jumps
        model   tiny

ERROR_CHECKING  = 1

codeseg
        startupcode

        lea     di, [data_start]
ifdef   ERROR_CHECKING
        mov     ax, 3500h
        int     21h
        mov     [word @movdx+1], bx
        mov     ah, 25h
        lea     dx, [error]
        int     21h

        jmp     next_expression

error:
        sti
        mov     sp, 0fffeh
        mov     al, '?'
        int     29h
endif

next_expression:
        xor     ebx, ebx

        mov     ah, 09h
        lea     dx, [prompt+di-data_start]
        int     21h

        mov     ah, 3fh
        lea     dx, [expr+di-data_start]
        mov     bl, 1
        dec     cx
        int     21h
        mov     si, dx
        cmp     [byte si], 13
        jne     evaluate

ifdef   ERROR_CHECKING
        mov     ax, 2500h
        push    es
        pop     ds
@movdx:
        mov     dx, 0000h
        int     21h
endif
        int     20h

evaluate:
        push    0000h
        fninit
next:
        lodsb
        or      dx, dx
        jnz     sign

digit:
        call    digit?
digit_cond:
        jz      @point
        jc      @digit
        jcxz    @@no_point
        dec     cx
@@div10:
        fidiv   [word ten+di-data_start]
        loop    @@div10
@@no_point:
        inc     dx
        jmp     @sign

@digit:
        jcxz    @@no_inccx
        inc     cx
@@no_inccx:
        mov     [di], ax
        fimul   [word ten+di-data_start]
        fiadd   [word di]
        jmp     next

@point:
        inc     cx
        jmp     next

sign:
        call    digit?
        jnc     @sign
        fldz
        xor     cx, cx
        xor     dx, dx
        dec     si
        jmp     next
@sign:
        mov     bl, 7
@@next:
        cmp     al, [bx+di+signs-1-data_start]
        je      @@found
        dec     bx
        jnz     @@next
ifdef   ERROR_CHECKING
        jmp     error
else
        jmp     next_expression
endif
@@found:
        xchg    ax, bx

@@do_op:
        cmp     al, 2
        je      @@push
        pop     bx
        cmp     bl, 2
        je      @@do_push
        cmp     ax, bx
        ja      @@push_op
        mov     bl, [opcodes+di+bx-4-data_start]
        mov     [byte opcode], bl
                db 0deh
opcode          db 00h
ifdef   ERROR_CHECKING
        xchg    ax, dx
        fnstsw  ax
        and     al, 1
        xchg    ax, dx
        jnz     error
endif
        jmp     @@do_op
@@push_op:
        push    bx
        cmp     al, 1
        je      @@output
@@do_push:
        cmp     al, 3
        je      next
@@push:
        push    ax
        jmp     next

@@output:
ifdef   ERROR_CHECKING
        fadd    st(1), st
        fnstsw  ax
        and     al, 1
        jz      error

        pop     ax
        or      ax, ax
        jnz     error
endif

        fldcw   [fpu_cw_chop+di-data_start]
        ftst
        fnstsw  ax
        sahf
        jae     @@unsigned
        mov     al, '-'
        int     29h
        fchs
@@unsigned:
        push    offset @@point
@@print:
        fist    [dword di]
        mov     eax, [di]
        jmp     @@recursive
@@point:
        fld1
        fxch
        fprem
        fld1
        faddp
        fimul   [dword scale_factor+di-data_start]
        fldcw   [fpu_cw_round+di-data_start]
        mov     cx, 1
        push    offset next_expression
        jmp     @@print

@@recursive:
        mov     bl, 10
        cdq
        div     ebx
        or      eax, eax
        push    dx
        jz      @@alldone
        call    @@recursive
@@alldone:
        pop     ax
        dec     cx
        js      @@pnt
        mov     al, '.'-'0'
@@pnt:
        add     al, '0'
        int     29h
        ret

digit?:
        cmp     al, '.'
        je      @@yes
        sub     al, '0'
        cmp     al, 10
        ret
@@yes:
        stc
        ret

scale_factor    dd 100000000
signs           db 13-'0', '('-'0', ')'-'0', '+'-'0', '-'-'0', '*'-'0', '/'-'0'
opcodes         db 0c1h, 0e9h, 0c9h, 0f9h
fpu_cw_chop     dw 0f7fh
fpu_cw_round    dw 037fh
label   ten word
prompt          db 0ah, 00h, 0dh, '> $'
data_start      dd ?
label   expr    byte

        end
