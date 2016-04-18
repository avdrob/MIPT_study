;*************************************************
; SAV_MUS.ASM     Version 2.0     12/20/90 00:17am (c) Ded
;*************************************************
my_program segment
assume cs:my_program, ds:my_program
org 100h
;------------------------------------------------
	port_b       equ  61h         	; $? ??  Ð?????i
					; ??O??i ????
	command_reg  equ  43h         	; ????? ?????255
	channel_2    equ  42h         	; ????? 2 ?????255

	DV      equ 1190000             ; h???? ???a?
	TACT    equ 3                   ; i?u?? ?? 1/18.2 - ????					; ?? ??
	DOd1    equ DV/138              ; h???i?a? ?????
	RE1     equ DV/146              ;
	MI1     equ DV/164              ;
	FAd1    equ DV/185              ;
	SOL1    equ DV/196              ;
	SOLd1   equ DV/207              ;
	LA1     equ DV/220              ;
	SI1     equ DV/247              ;

	DOd2    equ DV/277              ;
	RE2     equ DV/294              ;

	PAUSE   equ 10                  ; Ð?? ??(???S?a??
					; 119 ??
;================================================
start:					; ???Ð???
	mov ah, 09h                     ; e®I?? ?- ?©u? 1
	lea dx, mess1
	int 21h
 	call set_timer                  ; ??"? U ?????? ???
	call initsound                  ; ?g?"? U ?????a?
	cld                             ; SI ?h????b?  ?S??
	mov cx, NSOUNDS                 ;  CX - ?? ??	lea si, mus_tab                 ;  SI - $? ?????? ??
next:	lodsw                           ; ?G?U ??
	call sound                      ; ?GU ??
	call delay                      ; ?U 1 ??	loop next                       ; ? ?t????
	call nosound                    ; aS?????? - e??U ??
	call restore_timer              ; ®?-"? U ????????
					; ???
	mov ah, 09h                     ; ??%?? ?-S?©u? 2
	lea dx, mess2
	int 21h
	mov ax, 4c00h                   ; e??  D???? 0 (aSO.k.)
	int 21h  	                ; e??...

	mus_tab dw PAUSE, RE2,RE2,RE2,RE2           ; ???????
	        dw        DOd2,DOd2,DOd2
		dw        SOLd1
		dw        SI1
		dw PAUSE, LA1
		dw PAUSE, SOL1
		dw PAUSE, MI1
		dw PAUSE, RE1,RE1
		dw        DOd1
		dw        RE1
		dw        FAd1,FAd1
		dw        MI1
		dw        FAd1
		dw        MI1,MI1,MI1,MI1
		dw        RE1,RE1,RE1

	NSOUNDS equ ($-mus_tab)/2         ; ?? ??
	mess1 db 'Copyright (c) Ded, 1990. '
	      db 'Welcome to PC music!', 13,10, '$'
	mess2 db 'Thank you for attention.$'
;================================================
new08 proc far                          ; ?? !?? ??? ? ??u?
					; ??uu?i aUu??a??  0
					; ?a?h??delay_flag (? Ð?
					; ?? delay, ??o ? ?"???
	inc cs:number_ticks             ; ?u??%??? ?? (e?®©
	cmp cs:number_ticks, TACT       ; aU?????
	jb exit08                       ; ?? e??
	mov cs:delay_flag, 0            ; G - 1) ???U ?a?h??
	mov cs:number_ticks, 0          ;      2) ?a?U ?? ??
exit08:	jmp old08                       ; e?: e? U ????????
					; ???

	old08        dd ?               ; $? ??????? ???
	number_ticks dw 0               ; ??? ?? (e?®©
	delay_flag   dw 0               ; ?# ???? ?h??
new08 endp
;-------------------------------------------------
set_timer proc                          ; ???? ??u?i ????
					; ÐO???)??
	mov ax, 3508h                   ; ??? D?#35 - G?$? ???
					; ÐO???08
	int 21h                         ; ??%?Dý
	mov word ptr old08, bx          ; ????U $? ???????:
	mov word ptr old08+2, es        ; ?? ???, ?? ???
	mov ax, 2508h                   ; ??? D?#25 - ?????U?
	lea dx, new08                   ; $? ?®i ???? ???
	int 21h                         ; ??"? U ??????
	ret                             ;
set_timer endp                          ; (Destroys AX, BX, DX, ES)
;-------------------------------------------------
restore_timer proc                      ; ®?-"? U ?? ????
	push ds                         ; ???U DS
	mov ax, 2508h                   ; ??? D?#25 - ?????U?
	lds dx, old08                   ; $? ??????? ???
	int 21h                         ; ??%?Dý
	pop ds                          ; ®?-"? U DS
	ret                             ;
restore_timer endp                      ; (Destroys AX, DX)
;-------------------------------------------------
initsound proc                          ; ?g??? ?????255
	in al, port_b     		; ???? ?? ?
   	or al, 3                        ; ??"? U 3 ??? h?
   	out port_b, al                  ; ??%????
     	mov al, 10110110b               ; ??"? U ?? ???
					; ?-+? 2 ?????255
     	out command_reg, al             ; ??%??G?? ???
	ret                             ;
initsound endp                          ; (Destroys AX)
;-------------------------------------------------
sound proc near                         ; ??"? U ???a??
   	out channel_2, al               ; ??%???? `? ???   	mov al, ah                      ; ??? out ????? ?? AL
   	out channel_2, al               ; ??%???? `?
	ret                             ;
sound endp                              ; (Destroys AX)
;-------------------------------------------------
nosound proc                            ;
	in   al, port_b     		; ????3 ??? h? ?? B
   	and  al, 11111100b     		; ??O??i ????
   	out  port_b, al                 ;
	ret                             ;
nosound endp                            ; (Destroys AX)
;------------------------------------------------
delay proc                              ; ?U, ?? ???S??? delay_flag
delay1:	cmp delay_flag, 0               ; ?# ??u?
	jne delay1                      ; ?? ?U, ?? ????
	mov delay_flag, 1               ; G - ?  ?-"? U ?I
	ret                             ; ?®??%??delay endp                              ;
;------------------------------------------------
my_program ends
;************************************************
end start