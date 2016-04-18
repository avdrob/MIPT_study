/* ---------------------------------------------------------------------------
 * @
 *							УТИЛИТА ДЛЯ ОЦЕНКИ
 *					ЭФФЕКТИВНОСТИ РАЗВОРАЧИВАНИЯ ЦИКЛОВ
 *					====================================
 *
 * Build 0x001 29.06.2002
--------------------------------------------------------------------------- */

// конфигурация
#define BLOCK_SIZE	(8*M)			// размер обрабатываемого блока
/* конец конфигурации */

#include <DoCPU.h>

main()
{
	int a;
	int *p;
	int x = 0;

	// TITLE
	PRINT("= = = Демонстрация эффективности развертки циклов на операциях записи = = =\n");
	PRINT_TITLE;

	// выделение памяти
	p = malloc(BLOCK_SIZE);

	/* -----------------------------------------------------------------------
	 *
	 *					неоптимизированный вариант
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		for (a = 0; a < BLOCK_SIZE; a += sizeof(int))
			*(int *)((char *)p + a) = x;
	A_END(0)

	/* -----------------------------------------------------------------------
	 *
	 *					разворот на две итерации
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(1)
		for (a = 0; a < BLOCK_SIZE; a += 2*sizeof(int))
		{
			*(int *)((char*)p + a ) = x ;
			*(int *)((char*)p + a + sizeof(int)) = x ;
		}
	A_END(1)

	/* -----------------------------------------------------------------------
	 *
	 *					разворот на четыре итерации
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(2)
	for (a = 0; a < BLOCK_SIZE; a += 4*sizeof(int))
	{
		*(int *)((char*)p + a ) = x;
		*(int *)((char*)p + a + 1*sizeof(int)) = x;
		*(int *)((char*)p + a + 2*sizeof(int)) = x;
		*(int *)((char*)p + a + 3*sizeof(int)) = x;
	}
	A_END(2)

	/* -----------------------------------------------------------------------
	 *
	 *					разворот на восесь итерации
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(3)
		for (a = 0; a < BLOCK_SIZE; a += 8*sizeof(int))
		{
			*(int *)((char*)p + a ) = x;
			*(int *)((char*)p + a + 1*sizeof(int)) = x;
			*(int *)((char*)p + a + 2*sizeof(int)) = x;
			*(int *)((char*)p + a + 3*sizeof(int)) = x;
			*(int *)((char*)p + a + 4*sizeof(int)) = x;
			*(int *)((char*)p + a + 5*sizeof(int)) = x;
			*(int *)((char*)p + a + 6*sizeof(int)) = x;
			*(int *)((char*)p + a + 7*sizeof(int)) = x;
		}
	A_END(3)

	/* -----------------------------------------------------------------------
	 *
	 *					разворот на шестнадцать итераций
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(4)
		for (a = 0; a < BLOCK_SIZE; a += 16*sizeof(int))
		{
			*(int *)((char*)p + a ) = x;
			*(int *)((char*)p + a + 1*sizeof(int)) = x;
			*(int *)((char*)p + a + 2*sizeof(int)) = x;
			*(int *)((char*)p + a + 3*sizeof(int)) = x;
			*(int *)((char*)p + a + 4*sizeof(int)) = x;
			*(int *)((char*)p + a + 5*sizeof(int)) = x;
			*(int *)((char*)p + a + 6*sizeof(int)) = x;
			*(int *)((char*)p + a + 7*sizeof(int)) = x;

			*(int *)((char*)p + a + 8*sizeof(int)) = x;
			*(int *)((char*)p + a + 9*sizeof(int)) = x;
			*(int *)((char*)p + a + 10*sizeof(int)) = x;
			*(int *)((char*)p + a + 11*sizeof(int)) = x;
			*(int *)((char*)p + a + 12*sizeof(int)) = x;
			*(int *)((char*)p + a + 13*sizeof(int)) = x;
			*(int *)((char*)p + a + 14*sizeof(int)) = x;
			*(int *)((char*)p + a + 15*sizeof(int)) = x;
		}
	A_END(4)

	/* -----------------------------------------------------------------------
	 *
	 *					разворот на тридцать две итерации
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(5)
		for (a = 0; a < BLOCK_SIZE; a += 32*sizeof(int))
		{
			*(int *)((char*)p + a ) = x;
			*(int *)((char*)p + a + 1*sizeof(int)) = x;
			*(int *)((char*)p + a + 2*sizeof(int)) = x;
			*(int *)((char*)p + a + 3*sizeof(int)) = x;
			*(int *)((char*)p + a + 4*sizeof(int)) = x;
			*(int *)((char*)p + a + 5*sizeof(int)) = x;
			*(int *)((char*)p + a + 6*sizeof(int)) = x;
			*(int *)((char*)p + a + 7*sizeof(int)) = x;

			*(int *)((char*)p + a + 8*sizeof(int)) = x;
			*(int *)((char*)p + a + 9*sizeof(int)) = x;
			*(int *)((char*)p + a + 10*sizeof(int)) = x;
			*(int *)((char*)p + a + 11*sizeof(int)) = x;
			*(int *)((char*)p + a + 12*sizeof(int)) = x;
			*(int *)((char*)p + a + 13*sizeof(int)) = x;
			*(int *)((char*)p + a + 14*sizeof(int)) = x;
			*(int *)((char*)p + a + 15*sizeof(int)) = x;

			*(int *)((char*)p + a + 16*sizeof(int))  = x;
			*(int *)((char*)p + a + 17*sizeof(int))  = x;
			*(int *)((char*)p + a + 18*sizeof(int))  = x;
			*(int *)((char*)p + a + 19*sizeof(int))  = x;
			*(int *)((char*)p + a + 20*sizeof(int))  = x;
			*(int *)((char*)p + a + 21*sizeof(int))  = x;
			*(int *)((char*)p + a + 22*sizeof(int))  = x;
			*(int *)((char*)p + a + 23*sizeof(int))  = x;

			*(int *)((char*)p + a + 24*sizeof(int))  = x;
			*(int *)((char*)p + a + 25*sizeof(int)) = x;
			*(int *)((char*)p + a + 26*sizeof(int)) = x;
			*(int *)((char*)p + a + 27*sizeof(int)) = x;
			*(int *)((char*)p + a + 28*sizeof(int)) = x;
			*(int *)((char*)p + a + 29*sizeof(int)) = x;
			*(int *)((char*)p + a + 30*sizeof(int)) = x;
			*(int *)((char*)p + a + 31*sizeof(int)) = x;
		}
	A_END(5)

	/* -----------------------------------------------------------------------
	 *
	 *					разворот на шестьдесят четыре итерации
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(6)
		for (a = 0; a < BLOCK_SIZE; a += 64*sizeof(int))
		{
			*(int *)((char*)p + a ) = x;
			*(int *)((char*)p + a + 1*sizeof(int)) = x;
			*(int *)((char*)p + a + 2*sizeof(int)) = x;
			*(int *)((char*)p + a + 3*sizeof(int)) = x;
			*(int *)((char*)p + a + 4*sizeof(int)) = x;
			*(int *)((char*)p + a + 5*sizeof(int)) = x;
			*(int *)((char*)p + a + 6*sizeof(int)) = x;
			*(int *)((char*)p + a + 7*sizeof(int)) = x;

			*(int *)((char*)p + a + 8*sizeof(int)) = x;
			*(int *)((char*)p + a + 9*sizeof(int)) = x;
			*(int *)((char*)p + a + 10*sizeof(int)) = x;
			*(int *)((char*)p + a + 11*sizeof(int)) = x;
			*(int *)((char*)p + a + 12*sizeof(int)) = x;
			*(int *)((char*)p + a + 13*sizeof(int)) = x;
			*(int *)((char*)p + a + 14*sizeof(int)) = x;
			*(int *)((char*)p + a + 15*sizeof(int)) = x;

			*(int *)((char*)p + a + 16*sizeof(int)) = x;
			*(int *)((char*)p + a + 17*sizeof(int)) = x;
			*(int *)((char*)p + a + 18*sizeof(int)) = x;
			*(int *)((char*)p + a + 19*sizeof(int)) = x;
			*(int *)((char*)p + a + 20*sizeof(int)) = x;
			*(int *)((char*)p + a + 21*sizeof(int)) = x;
			*(int *)((char*)p + a + 22*sizeof(int)) = x;
			*(int *)((char*)p + a + 23*sizeof(int)) = x;

			*(int *)((char*)p + a + 24*sizeof(int)) = x;
			*(int *)((char*)p + a + 25*sizeof(int)) = x;
			*(int *)((char*)p + a + 26*sizeof(int)) = x;
			*(int *)((char*)p + a + 27*sizeof(int)) = x;
			*(int *)((char*)p + a + 28*sizeof(int)) = x;
			*(int *)((char*)p + a + 29*sizeof(int)) = x;
			*(int *)((char*)p + a + 30*sizeof(int)) = x;
			*(int *)((char*)p + a + 31*sizeof(int)) = x;

			*(int *)((char*)p + a + 32*sizeof(int)) = x;
			*(int *)((char*)p + a + 33*sizeof(int)) = x;
			*(int *)((char*)p + a + 34*sizeof(int)) = x;
			*(int *)((char*)p + a + 35*sizeof(int)) = x;
			*(int *)((char*)p + a + 36*sizeof(int)) = x;
			*(int *)((char*)p + a + 37*sizeof(int)) = x;
			*(int *)((char*)p + a + 38*sizeof(int)) = x;
			*(int *)((char*)p + a + 39*sizeof(int)) = x;

			*(int *)((char*)p + a + 40*sizeof(int)) = x;
			*(int *)((char*)p + a + 41*sizeof(int)) = x;
			*(int *)((char*)p + a + 42*sizeof(int)) = x;
			*(int *)((char*)p + a + 43*sizeof(int)) = x;
			*(int *)((char*)p + a + 44*sizeof(int)) = x;
			*(int *)((char*)p + a + 45*sizeof(int)) = x;
			*(int *)((char*)p + a + 46*sizeof(int)) = x;
			*(int *)((char*)p + a + 47*sizeof(int)) = x;

			*(int *)((char*)p + a + 48*sizeof(int)) = x;
			*(int *)((char*)p + a + 49*sizeof(int)) = x;
			*(int *)((char*)p + a + 50*sizeof(int)) = x;
			*(int *)((char*)p + a + 51*sizeof(int)) = x;
			*(int *)((char*)p + a + 52*sizeof(int)) = x;
			*(int *)((char*)p + a + 53*sizeof(int)) = x;
			*(int *)((char*)p + a + 54*sizeof(int)) = x;
			*(int *)((char*)p + a + 55*sizeof(int)) = x;

			*(int *)((char*)p + a + 56*sizeof(int)) = x;
			*(int *)((char*)p + a + 57*sizeof(int)) = x;
			*(int *)((char*)p + a + 58*sizeof(int)) = x;
			*(int *)((char*)p + a + 59*sizeof(int)) = x;
			*(int *)((char*)p + a + 60*sizeof(int)) = x;
			*(int *)((char*)p + a + 61*sizeof(int)) = x;
			*(int *)((char*)p + a + 62*sizeof(int)) = x;
			*(int *)((char*)p + a + 63*sizeof(int)) = x;
		}
	A_END(6)

	// вывод результатов на консоль
	Lx_OUT(" 2x",Ax_GET(0), Ax_GET(1));
	Lx_OUT(" 4x",Ax_GET(0), Ax_GET(2));
	Lx_OUT(" 8x",Ax_GET(0), Ax_GET(3));
	Lx_OUT("16x",Ax_GET(0), Ax_GET(4));
	Lx_OUT("32x",Ax_GET(0), Ax_GET(5));
	Lx_OUT("64x",Ax_GET(0), Ax_GET(6));

}

_P_S()
{
/*
	...каждый   язык  -  это  своя  философия,  свой  взгляд  на  деятельность
	программиста,  отражение  определенной  технологии  программирования. Даже
	представлений   об   Алголе,   Фортране   и   Бейсике   достаточно,  чтобы
	почувствовать,  что  имеется  в  виду.

	Скажем,    творцы   Алгола   (выдающиеся   представители    международного
	сообщества  ученых  в области информатики под руководством  Петера  Наура)
	с естественным для них академизмом придавали относительно  много  значения
	строгости   определения  и  изяществу  языковых  конструктов.   Считалось,
	что   самое   важное  в  работе  программиста  - сформулировать   алгоритм
	(и,   возможно,  опубликовать  его).  Переписать программу  в  расчете  на
	конкретные  устройства  ввода-вывода  считалось  не заслуживающей  особого
	внимания   технической   деятельностью.   Не   привлек должного   внимания
	авторов    языка    и    такой    "технический"    аспект  программистской
	деятельности,  как компоновка программ из модулей.

	Творцы  Фортрана   (сотрудники   фирмы  ИБМ  во  главе с Джоном Бэкусом) в
	значительной степени пренебрегли строгостью и изяществом и со свойственным
	им  в  ту  пору (1954-57гг.)  прагматизмом  уже  в  первых  версиях  языка
	уделили  особое  внимание  вводу-выводу  и  модульности. Но ни Фортран, ни
	Алгол  не рассчитаны на  работу  в  диалоговом  режиме. В отличии, как вам
	известно, от Бейсика (созданного в Дартмундском колледже первоначально для
	обучения студентов).

					КАУФМАН В.Ш. "Языки программирования: концепции и принципы"
*/
}
