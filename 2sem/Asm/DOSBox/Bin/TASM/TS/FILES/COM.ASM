		model	tiny		;модель памяти
		.code			;сегмент кода
		org	100h		;для .com-файла
start:
		lea	bx,heep+15	;смещение на конец программы + 15 байт
		mov	cl,4
		shr	bx,cl		;получить размер программы в параграфах
		mov	ah,4ah		;перераспределить память
		int	21h		;вызов функции DOS
		jc	to_exit		;если ошибка - выход

		pop	ax		;адрес возврата из старого стека...
		mov	sp,offset end_of_stack ;переключение стека
		push	ax		;...занести в новый


		call	main		;вызов главного модуля

to_exit:
		ret			;возврат в OS 

main		proc	near		;главная подпрограмма
		ret
main		endp

		.data			;сегмент данных

;var1		db	1		;переменная

		.data?

;var2		db	?		;еще одна переменная


		dw	100 dup(?)
end_of_stack	label	word

heep		label	byte		;метка конца программы

		end	start
