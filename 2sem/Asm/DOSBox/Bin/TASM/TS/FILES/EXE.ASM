		model	small			;модель памяти
		.code				;сегмент кода
start:
		mov	ax,seg @data		;"привязка"
		mov	ds,ax			;сегмента данных
		mov	ax,seg @stack

		mov	bx,z_seg		;последний сегмент
		mov	ax,es			;вычисляем размер
		sub	bx,ax			;программы
		mov	ah,4ah
		int	21h			;освобождаем лишнюю память
		jc	to_exit

		call	main			;главный модуль

to_exit:
		mov	ah,4ch
		int	21h

main		proc	near
		lea	dx,example_1
		mov	ah,9
		int	21h	
		ret
main		endp

		.data
example_1	db	'String...',13,10,36

		.stack	100


		.data?

example_2	db	?

z_seg		segment				;последний сегмент,
z_seg		ends				;т.к. они ^^^ расположены по алфавиту

		end	start
