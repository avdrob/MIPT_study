;=============================================================================
;                 Задание # 22. Файл 22shwerr.asm
;      Резидентная система помощи - библиотека вывода ошибок
;               Copyright (c) 2002 Andreev Andrey
;=============================================================================

;---Show error procedure------------------------(c) Andreev Andrey, 2002--;
; entry:        dx - offset message.                                      ;
; destr:        none.                                                     ;
; return:       !!!!jmp exit!!!!                                          ;
;-------------------------------------------------------------------------;
show_error      proc
                mov ah, 9h
                int 21h
                jmp exit28  ; procedure's finish - exit from program!!!
                endp
;---End of show error procedure-------------------------------------------;
                                ; show errors  -->
open_error:     mov dx, offset open_err_msg
                call show_error
close_error:    mov dx, offset close_err_msg
                call show_error
read_error:     mov dx, offset read_err_msg
                call show_error
write_error:    pop bx ; For stack balance!!!
                mov dx, offset write_err_msg
                call show_error ; show errors  <--

;error_exit:     mov ax, 4c00h           ; exit
;                int 21h


open_err_msg    db 'Sorry, but I do not open file "c:\myfile" !!!$'
close_err_msg   db 'Sorry, but I do not close file "c:\myfile" !!!$'
read_err_msg    db 'I dont read file "c:\myfile" !!!!!$'
write_err_msg   db 'I do not write file "c:\myfile" !!!$'
;--End of library---------------------------------------------------------;