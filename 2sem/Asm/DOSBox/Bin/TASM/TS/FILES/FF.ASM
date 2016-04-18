;Find File
;(c) Chehuta Sergey
;<support@whitetown.com>
;2:4614/46.56
;
;tasm /m ff.asm
;tlink /t/x ff.obj
;
; 216 байт

                model   tiny

                .code
                org     100h
start:
                mov     ax,2524h
                lea     dx,int_24
                int     21h

                mov     bp,3
                mov     ah,19h
                int     21h
                mov     dl,al
                mov     ah,0eh
                int     21h
                add     al,41h
                mov     bl,al
nachalo:
                mov     ah,4eh
                mov     cx,37h
                lea     dx,path
                int     21h
                jc      quit

no_problem:
                test    byte ptr ds:[95h],10h
                jnz     dir

                call    clear
                lea     si,path
                cld
sled:
                mov     ah,0eh
                mov     bh,0
                lodsb
                cmp     al,'*'
                je      end_dir
                int     10h
                jmp     short sled
end_dir:
                mov     si,9eh
sled1:
                mov     ah,0eh
                mov     bh,0
                lodsb
                cmp     al,0
                je      end_name
                int     10h
                jmp     short sled1
end_name:
                mov     al,13
                int     10h
                jmp     short next
dir:
                mov     si,9eh
                cmp     byte ptr [si],'.'
                je      next
                lea     di,path
                add     di,bp
                cld
next_char:
                lodsb
                stosb
                inc     bp
                cmp     al,0
                jne     next_char
                dec     di
                call    form
                mov     si,80h
                mov     cx,22
push_loop:
                lodsw
                push    ax
                loop    push_loop
to_nachalo:
                jmp     short nachalo
next:
                mov     ah,4fh
                int     21h
                jnc     no_problem
quit:
                cmp     bp,3
                je      exit
                lea     di,path
                add     di,bp
                dec     di
found:
                dec     di
                dec     bp
                cmp     byte ptr [di],'\'
                jne     found
                call    form
                mov     cx,22
                mov     di,80h+42
                std
pop_loop:
                pop     ax
                stosw
                loop    pop_loop
                jmp     short next

exit:
                mov     al,path
                cmp     bl,al
                jbe     clear
                inc     al
                mov     path,al
                mov     bp,3

                jmp     short to_nachalo
clear:
                mov     ax,0a20h
                mov     cx,80
                mov     bh,0
                int     10h
                ret

form:
                lea     si,maska
                cld
                mov     cx,5
                rep     movsb
                ret

int_24:
                mov     al,3
                iret

                .data

maska           db      '\*.*',0

path            db      'C:\*.*',0

        end     start

