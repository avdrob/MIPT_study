		model	tiny
		.code
		org	100h
start:
		mov	ax,168bh
		xor	bx,bx
		int	2fh

		push	es
		xor	ax,ax
		mov	es,ax
		mov	al,es:[449h]		;не должен быть #7
		mov	ah,es:[485h]		;должно быть 16
		pop	es			;т.к. в программе используется
		cmp	al,7
		je	no_vga_mode
		cmp	ah,16
		je	vga_mode		;фонт 8x16
no_vga_mode:

;		call	if_windows		;если нет windows - OK
;		jne	vga_mode		;иначе нет - в окне MS-DOS символы
						;остануться неизмененными
		mov	a,'('
		mov	b,'c'
		mov	c,')'

		jmp	print
				
		;если 8x14 или 8x8 - то надо брать другие
		;фонты и ниже в программе использовать
		;не константы, а переменные
		;но я обычно когда занимаюсь такой чепухой
		;ограничиваюсь 8x16, а в остальных случаях
		;вывожу обычные символы
vga_mode:

		call	open_read	;открыть ЗГ для чтения

		push	ds
		mov	cx,2		;сколько символов
		mov	bx,215		;код первого
		lea	di,save_char	;куда сохранять
		mov	ax,0a000h
		mov	ds,ax
cycle_1:
		mov	byte ptr al,bl
		inc	bl
		xor	ah,ah
		shl	ax,5		;на символ в ЗГ отводиться 32 байта
		mov	si,ax
		mov	ax,cx		;сохранить cx
		mov	cx,16		;описание - 16 байт на символ
		cld
		rep	movsb
		mov	cx,ax		;восстановить cx
		loop	cycle_1
		pop	ds

		call	close_read	;закрыть ЗГ для чтения
		
		call	open_write	;открыть ЗГ для записи

		push	es
		mov	cx,2		;сколько символов
		mov	bx,215		;код первого
		lea	si,new_char	;новые символы

		mov	ax,0b800h	
		mov	es,ax
cycle_2:
		mov	byte ptr al,bl
		inc	bl
		xor	ah,ah
		shl	ax,5		;на символ в ЗГ отводиться 32 байта
		mov	di,ax
		mov	ax,cx		;сохранить cx
		mov	cx,16		;описание - 16 байт на символ
		cld
		rep	movsb
		mov	cx,ax		;восстановить cx
		loop	cycle_2
		pop	es

		call	close_write	;закрыть ЗГ для записи
print:
		lea	dx,copyright
		mov	ah,9
		int	21h
		ret


open_read	proc	near
		cli
		mov	dx,3c4h
		mov	ax,0704h
		out	dx,ax
		mov	dx,3ceh
		mov	ax,0204h
		out	dx,ax
		mov	ax,5
		out	dx,ax
		mov	ax,0406h
		out	dx,ax
		ret
open_read	endp


close_read	proc	near
		mov	dx,3c4h
		mov	ax,0302h
		out	dx,ax
		mov	ax,0304h
		out	dx,ax
		mov	dx,3ceh
		mov	ax,4
		out	dx,ax
		mov	ax,1005h
		out	dx,ax
		sub	ax,ax
		mov	es,ax
		mov	ax,0e06h
		out	dx,ax
		sti
		ret
close_read	endp

open_write	proc	near
		cli
		mov	dx,03c4h
		mov	ax,0402h
		out	dx,ax
		mov	ax,0404h
		out	dx,ax
		mov	dx,03ceh
		mov	ax,0c06h
		out	dx,ax
		ret
open_write	endp

close_write	proc	near
		mov	dx,3ceh
		mov	ax,0e06h
		out	dx,ax
		mov	dx,3c4h
		mov	ax,0304h
		out	dx,ax
		mov	ax,0302h
		out	dx,ax
		sti
		ret
close_write	endp

		.data

new_char	db	00000000b
		db	00000000b
		db	00000111b
		db	00011100b
		db	00110000b
		db	00110011b
		db	01100110b
		db	01100110b
		db	01100110b
		db	01100110b
		db	00110011b
		db	00110000b
		db	00011100b
		db	00000111b
		db	00000000b
		db	00000000b

		db      00000000b
		db      00000000b
		db	11100000b
		db	00111000b
		db	00001100b
		db	11001100b
		db	01100110b
		db	00000110b
		db	00000110b
		db	01100110b
		db	11001100b
		db	00001100b
		db	00111000b
		db	11100000b
		db      00000000b
		db      00000000b


copyright	db	'DEMO. Copyright '
a		db	'╫'
b		db	'╪'
c		db	' '
		db	' Sergey Chehuta 1998-2000',13,10,36

save_char	db	32 dup(?)	;16 на символ

		end	start
