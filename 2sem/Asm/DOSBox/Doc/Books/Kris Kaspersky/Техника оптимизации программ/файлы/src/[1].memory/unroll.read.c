/* ---------------------------------------------------------------------------
 * @
 *							УТИЛИТА ДЛЯ ОЦЕНКИ
 *					ЭФФЕКТИВНОСТИ РАЗВОРАЧИВАНИЯ ЦИКЛОВ
 *					====================================
 *
 * Build 0x001 29.06.2002
--------------------------------------------------------------------------- */
// конфигурация
#define BLOCK_SIZE	(8*M)			// размер блока
/* конец конфигурации */

#include <DoCPU.h>

main()
{
	int a;
	int *p;
	int x = 0;

	// TITLE
	PRINT("= = = демонстрация эффективности развертки циклов на операциях чтения = = =\n");
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
			x += *(int *)((char *)p + a);
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
			x += *(int *)((char *)p + a );
			x += *(int *)((char *)p + a + sizeof(int));
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
			x += *(int *)((char *)p + a );
			x += *(int *)((char *)p + a + 1*sizeof(int));
			x += *(int *)((char *)p + a + 2*sizeof(int));
			x += *(int *)((char *)p + a + 3*sizeof(int));
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
			x += *(int *)((char *)p + a );
			x += *(int *)((char *)p + a + 1*sizeof(int));
			x += *(int *)((char *)p + a + 2*sizeof(int));
			x += *(int *)((char *)p + a + 3*sizeof(int));
			x += *(int *)((char *)p + a + 4*sizeof(int));
			x += *(int *)((char *)p + a + 5*sizeof(int));
			x += *(int *)((char *)p + a + 6*sizeof(int));
			x += *(int *)((char *)p + a + 7*sizeof(int));
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
			x += *(int *)((char *)p + a );
			x += *(int *)((char *)p + a + 1*sizeof(int));
			x += *(int *)((char *)p + a + 2*sizeof(int));
			x += *(int *)((char *)p + a + 3*sizeof(int));
			x += *(int *)((char *)p + a + 4*sizeof(int));
			x += *(int *)((char *)p + a + 5*sizeof(int));
			x += *(int *)((char *)p + a + 6*sizeof(int));
			x += *(int *)((char *)p + a + 7*sizeof(int));

			x += *(int *)((char *)p + a + 8*sizeof(int));
			x += *(int *)((char *)p + a + 9*sizeof(int));
			x += *(int *)((char *)p + a + 10*sizeof(int));
			x += *(int *)((char *)p + a + 11*sizeof(int));
			x += *(int *)((char *)p + a + 12*sizeof(int));
			x += *(int *)((char *)p + a + 13*sizeof(int));
			x += *(int *)((char *)p + a + 14*sizeof(int));
			x += *(int *)((char *)p + a + 15*sizeof(int));
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
			x += *(int *)((char *)p + a );
			x += *(int *)((char *)p + a + 1*sizeof(int));
			x += *(int *)((char *)p + a + 2*sizeof(int));
			x += *(int *)((char *)p + a + 3*sizeof(int));
			x += *(int *)((char *)p + a + 4*sizeof(int));
			x += *(int *)((char *)p + a + 5*sizeof(int));
			x += *(int *)((char *)p + a + 6*sizeof(int));
			x += *(int *)((char *)p + a + 7*sizeof(int));

			x += *(int *)((char *)p + a + 8*sizeof(int));
			x += *(int *)((char *)p + a + 9*sizeof(int));
			x += *(int *)((char *)p + a + 10*sizeof(int));
			x += *(int *)((char *)p + a + 11*sizeof(int));
			x += *(int *)((char *)p + a + 12*sizeof(int));
			x += *(int *)((char *)p + a + 13*sizeof(int));
			x += *(int *)((char *)p + a + 14*sizeof(int));
			x += *(int *)((char *)p + a + 15*sizeof(int));

			x += *(int *)((char *)p + a + 16*sizeof(int));
			x += *(int *)((char *)p + a + 17*sizeof(int));
			x += *(int *)((char *)p + a + 18*sizeof(int));
			x += *(int *)((char *)p + a + 19*sizeof(int));
			x += *(int *)((char *)p + a + 20*sizeof(int));
			x += *(int *)((char *)p + a + 21*sizeof(int));
			x += *(int *)((char *)p + a + 22*sizeof(int));
			x += *(int *)((char *)p + a + 23*sizeof(int));

			x += *(int *)((char *)p + a + 24*sizeof(int));
			x += *(int *)((char *)p + a + 25*sizeof(int));
			x += *(int *)((char *)p + a + 26*sizeof(int));
			x += *(int *)((char *)p + a + 27*sizeof(int));
			x += *(int *)((char *)p + a + 28*sizeof(int));
			x += *(int *)((char *)p + a + 29*sizeof(int));
			x += *(int *)((char *)p + a + 30*sizeof(int));
			x += *(int *)((char *)p + a + 31*sizeof(int));
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
			x += *(int *)((char *)p + a );
			x += *(int *)((char *)p + a + 1*sizeof(int));
			x += *(int *)((char *)p + a + 2*sizeof(int));
			x += *(int *)((char *)p + a + 3*sizeof(int));
			x += *(int *)((char *)p + a + 4*sizeof(int));
			x += *(int *)((char *)p + a + 5*sizeof(int));
			x += *(int *)((char *)p + a + 6*sizeof(int));
			x += *(int *)((char *)p + a + 7*sizeof(int));

			x += *(int *)((char *)p + a + 8*sizeof(int));
			x += *(int *)((char *)p + a + 9*sizeof(int));
			x += *(int *)((char *)p + a + 10*sizeof(int));
			x += *(int *)((char *)p + a + 11*sizeof(int));
			x += *(int *)((char *)p + a + 12*sizeof(int));
			x += *(int *)((char *)p + a + 13*sizeof(int));
			x += *(int *)((char *)p + a + 14*sizeof(int));
			x += *(int *)((char *)p + a + 15*sizeof(int));

			x += *(int *)((char *)p + a + 16*sizeof(int));
			x += *(int *)((char *)p + a + 17*sizeof(int));
			x += *(int *)((char *)p + a + 18*sizeof(int));
			x += *(int *)((char *)p + a + 19*sizeof(int));
			x += *(int *)((char *)p + a + 20*sizeof(int));
			x += *(int *)((char *)p + a + 21*sizeof(int));
			x += *(int *)((char *)p + a + 22*sizeof(int));
			x += *(int *)((char *)p + a + 23*sizeof(int));

			x += *(int *)((char *)p + a + 24*sizeof(int));
			x += *(int *)((char *)p + a + 25*sizeof(int));
			x += *(int *)((char *)p + a + 26*sizeof(int));
			x += *(int *)((char *)p + a + 27*sizeof(int));
			x += *(int *)((char *)p + a + 28*sizeof(int));
			x += *(int *)((char *)p + a + 29*sizeof(int));
			x += *(int *)((char *)p + a + 30*sizeof(int));
			x += *(int *)((char *)p + a + 31*sizeof(int));

			x += *(int *)((char *)p + a + 32*sizeof(int));
			x += *(int *)((char *)p + a + 33*sizeof(int));
			x += *(int *)((char *)p + a + 34*sizeof(int));
			x += *(int *)((char *)p + a + 35*sizeof(int));
			x += *(int *)((char *)p + a + 36*sizeof(int));
			x += *(int *)((char *)p + a + 37*sizeof(int));
			x += *(int *)((char *)p + a + 38*sizeof(int));
			x += *(int *)((char *)p + a + 39*sizeof(int));

			x += *(int *)((char *)p + a + 40*sizeof(int));
			x += *(int *)((char *)p + a + 41*sizeof(int));
			x += *(int *)((char *)p + a + 42*sizeof(int));
			x += *(int *)((char *)p + a + 43*sizeof(int));
			x += *(int *)((char *)p + a + 44*sizeof(int));
			x += *(int *)((char *)p + a + 45*sizeof(int));
			x += *(int *)((char *)p + a + 46*sizeof(int));
			x += *(int *)((char *)p + a + 47*sizeof(int));

			x += *(int *)((char *)p + a + 48*sizeof(int));
			x += *(int *)((char *)p + a + 49*sizeof(int));
			x += *(int *)((char *)p + a + 50*sizeof(int));
			x += *(int *)((char *)p + a + 51*sizeof(int));
			x += *(int *)((char *)p + a + 52*sizeof(int));
			x += *(int *)((char *)p + a + 53*sizeof(int));
			x += *(int *)((char *)p + a + 54*sizeof(int));
			x += *(int *)((char *)p + a + 55*sizeof(int));

			x += *(int *)((char *)p + a + 56*sizeof(int));
			x += *(int *)((char *)p + a + 57*sizeof(int));
			x += *(int *)((char *)p + a + 58*sizeof(int));
			x += *(int *)((char *)p + a + 59*sizeof(int));
			x += *(int *)((char *)p + a + 60*sizeof(int));
			x += *(int *)((char *)p + a + 61*sizeof(int));
			x += *(int *)((char *)p + a + 62*sizeof(int));
			x += *(int *)((char *)p + a + 63*sizeof(int));
		}
	A_END(6)

	// вывод результатов на консоль
	Lx_OUT(" 2x",Ax_GET(0),Ax_GET(1));
	Lx_OUT(" 4x",Ax_GET(0),Ax_GET(2));
	Lx_OUT(" 8x",Ax_GET(0),Ax_GET(3));
	Lx_OUT("16x",Ax_GET(0),Ax_GET(4));
	Lx_OUT("32x",Ax_GET(0),Ax_GET(5));
	Lx_OUT("64x",Ax_GET(0),Ax_GET(6));
}

_P_S()
{
/*
	"...конечная  цель  программирования - не создание программ самих по себе,
	а предоставление  программных  услуг.  Другими  словами,  программирование
	в   конечном     итоге   нацелено   на   обслуживание   пользователей.   А
	настоящее  обслуживание   должно  руководствоваться  известным  принципом:
	"клиент  всегда прав".   В  применении  к  программированию  этот  принцип
	означает,  что программы   должны  быть  надежными  и  робастными"
				КАУФМАН В.Ш.  "Языки программирования: концепции и принципы"
*/
}