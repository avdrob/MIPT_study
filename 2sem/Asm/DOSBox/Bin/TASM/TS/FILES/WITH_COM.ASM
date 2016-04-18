;пример запуска программ используя вторую
;копию command.com

	model	tiny
	.code
	org	100h
start:
	lea	bx,heep+300h	;конец программы+
	mov	dx,bx		;300-20 байт под стек
	mov	cl,4		;
	shr	bx,cl		;
	mov	ah,4ah		;перераспределить
	int	21h		;память
	jnc	ok_mem
	ret
ok_mem:
	sub	dx,20		;отнять, чтоб не
	cli			;затереть MCB
	pop	ax		;извлечь из старого стека...
	mov	sp,dx		;переключение стека
	push	ax		;... и занести в новый

	mov	_ss,ss		;запомнить
	mov	_sp,sp		;SS и SP
	sti
	
	lea	ax,com_line	;занесение
	lea	bx,param	;адреса
	mov	[bx]+2,ax	;параметров
	mov	ax,cs		;командной
	mov	[bx]+4,ax	;строки


	lea	bx,param	;параметры
	lea	dx,filename	;имя файла
	mov	ax,4b00h	;загрузка и запуск
	int	21h		;

	mov	ax,cs		;восстановление
	mov	ds,ax		;измененных
	mov	es,ax		;регистров
	cli			;
	mov	ss,_ss		;
	mov	sp,_sp		;
	sti
	ret


;коммандная строка - длина,вторая копия,команда,13
;			│    ┌──┘         │     │
;			│    │     ┌──────┘     │
;			│    │     │          ┌─┘  
com_line	db	16,' /C format a: /u',13
filename	db	'c:\command.com',0
param		dw	7 dup(0)	; - параметры
_ss		dw	0
_sp		dw	0
heep		db	''
	end	start
