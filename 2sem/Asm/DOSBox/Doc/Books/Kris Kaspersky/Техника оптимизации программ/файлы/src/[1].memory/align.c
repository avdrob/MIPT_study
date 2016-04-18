/* ---------------------------------------------------------------------------
 *
 *						УТИЛИТА ДЕМОНСТРИРУЮЩАЯ ВЛИЯНИЕ
 *		ВЫРАВНИВАНИЯ АДРЕСОВ НА СКОРОСТЬ ОБРАБОТКИ БОЛЬШИХ ОБЪЕМОВ ДАННЫХ
 *		=================================================================
 *
 * Build 0x001 06.07.2002
--------------------------------------------------------------------------- */
// КОНФИГУРАЦИЯ
#define BLOCK_SIZE		(8*M)			// размер обрабатываемого блока

#include <DoCPU.h>
main()
{
	int *p1;
	int a, x = 0;

	// TITLE
	PRINT("= = = влияние выравнивания адресов на скорость обработки блоков данных = = =\n");
	PRINT_TITLE;

	// выделяем память
	p1 = malloc(BLOCK_SIZE);


	/*------------------------------------------------------------------------
	 *
	 *						БЕЗ РАЗВОРОТА ЦИКЛА
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		// выр. чтение
		for (a = 0; a < BLOCK_SIZE; a += sizeof(int))
			x+= *(int *) ((char *)p1 + a);
	A_END(0)

	VVV;
	A_BEGIN(1)
		// не выр. чтение
		for (a = 0; a < BLOCK_SIZE; a += sizeof(int))
			x+= *(int *) ((char *)p1 + a + 1 /* <- уналиджим */);
		A_END(1)

	VVV;
	A_BEGIN(2)
		// выр. write
		for (a = 0; a < BLOCK_SIZE; a += sizeof(int))
			*(int *) ((char *)p1 + a) = x;
	A_END(2)

	VVV;
	A_BEGIN(3)
		// не выр. write
		for (a = 0; a < BLOCK_SIZE; a += sizeof(int))
			*(int *) ((char *)p1 + a + 1) = x;
	A_END(3)

	/*------------------------------------------------------------------------
	 *
	 *					С РАЗВОРОТОМ ЦИКЛА НА 8 ИТЕРАЦИЙ
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(4)
		// Выр. чтение
		for (a = 0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			x += *(int *) ((char *)p1 + a + 0 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 1 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 2 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 3 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 4 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 5 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 6 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 7 * sizeof(int));
		}
	A_END(4)


	VVV;
	A_BEGIN(5)
		// не выр. чтение
		for (a = 0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			x += *(int *) ((char *)p1 + a + 0 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 1 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 2 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 3 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 4 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 5 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 6 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 7 * sizeof(int) + 1);
		}
	A_END(5)


	VVV;
	A_BEGIN(6)
		// выр. write
		for (a = 0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			*(int *) ((char *)p1 + a + 0 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 1 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 2 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 3 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 4 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 5 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 6 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 7 * sizeof(int)) = x;
		}
	A_END(6)


	VVV;
	A_BEGIN(7)
		// не выр. write
		for (a = 0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			*(int *) ((char *)p1 + a + 0 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 1 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 2 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 3 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 4 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 5 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 6 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 7 * sizeof(int) + 1) = x;
		}
	A_END(7)
	
	// вывод результатов на консоль
	Lx_OUT("unalign read  without unroll",Ax_GET(0),Ax_GET(1));
	Lx_OUT("unalign write without unroll",Ax_GET(2),Ax_GET(3));
	Lx_OUT("unalign read  with    unroll",Ax_GET(4),Ax_GET(5));
	Lx_OUT("unalign write with    unroll",Ax_GET(6),Ax_GET(7));
}

_P_S()
{
/*
	"Есть воин, который будет воевать -- по любому поводу. Он будет сражаться,
	сражение у него в крови. Из-за наличия людей такого типа мир не может жить
	в мире. Это невозможно. Раз в каждые десять лет должна быть большая война.
	И если вы хотите избежать больших войн, тогда пусть у вас будут небольшие,
	локальные  войны,  но  итог  останется тем же". Из-за атомных и водородных
	бомб  большая  война  стала  почти  невозможна.  Вот  почему по всему миру
	столько  горячих  точек,  внутренних  конфликтов:  во  Вьетнаме,  Кашмире,
	Бангладеш,  Израиле,  множество мелких войн, но итог будет тем же. За пять
	тысяч  лет  человек  участвовал  в пятнадцати тысячах войн, по три войны в
	год."
														О-шо "Пустая Лодка"
*/
}