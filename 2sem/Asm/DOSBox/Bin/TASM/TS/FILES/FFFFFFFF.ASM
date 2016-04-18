        model   tiny
        .code
        org     100h
start:
        mov     dx,0ffffh
        mov     ax,0ffffh
        call    long_to_str
        lea     dx,string
        mov     ah,9
        int     21h
        ret

long_to_str:
        lea     si, string+23
again:  
	or      dx,dx
        jne     dalee
        cmp     ax,10
        jb      konec
dalee:
        mov     cx,ax
        mov     ax,dx
        xor     dx,dx
        div     ten
        mov     bx,ax
        mov     ax,cx
        div     ten
        or      dl,30h
        mov     [si],dl
        mov     dx,bx
        dec     si
        jmp     short   again
konec:
        or      al,30h
        mov     [si],al
	ret

ten     dw      10
string  db      '0FFFFFFFFh ==',12 dup(' ') ,13,10,36

        end     start
