/* ---------------------------------------------------------------------------
 *
 *		ПРИМЕР ДЕМОНСТРИРУЮЩИЙ ЭФФЕКТ ОТ "РАЗВОРАЧИВАНИЯ" ЦИКЛА ПРЕДВЫБОРКИ
 *		===================================================================
 *
 * Build 0x001 26.08.2002
--------------------------------------------------------------------------- */

#define BLOCK_SIZE	(4*M)		// выбираем достаточно большой блок памяти,
								// чтобы исключить влиянение кэша

#define STEP_SIZE	4			// шаг чтения


#define computation(a, b, p) b += (a)*0x666; b += *(int *)((char *)(p)+a);

#include <DoCPU.h>
#include <DoCPU.cmd.h>

/* ------------------------------------------------------------------

					ТЕСТ БЕЗ ПРЕДВЫБОРКИ
										(неоптимизированный вариант)

--------------------------------------------------------------------- */
normal(char *p, int len)
{
	int a;
	int x = 0;
	for(a = 0; a < len; a += 4)
	{
		computation(a, x, p);
	}
	return x;
}


/* ------------------------------------------------------------------

					ТЕСТ БЕЗ РАЗВЕРТКИ
										(неоптимизированный вариант)
	
--------------------------------------------------------------------- */
prefetch(char *p, int len)
{
	int a;
	int x = 0;

	for(a = 0; a < len; a += 4)
	{
		__prefetchnta(p + a + 32*5);
			computation(a, x, p);
	}
	return x;
}


/* -------------------------------------------------------------------
	
					ТЕСТ C РАЗВЕРТКОЙ
										(оптимизированный вариант)

---------------------------------------------------------------------- */
unroll_prefetch(char *p, int len)
{
	int a;
	int x = 0;
	for(a = 0; a < len; a += 32)
	{
		__prefetchnta(p + a + 32*5);

		computation(a + 0,	x, p);
		computation(a + 4,	x, p);
		computation(a + 8,	x, p);
		computation(a + 12,	x, p);
		computation(a + 16,	x, p);
		computation(a + 20,	x, p);
		computation(a + 24,	x, p);
		computation(a + 28,	x, p);
	}
}

main()
{
	char *p;
	

	// выделяем память
	p= (char *) _malloc32(BLOCK_SIZE);

	// TITLE
	PRINT("= = = memory optimization using prefetch whith loop unrolling = = =\n");
	PRINT_TITLE;

	A_BEGIN(0)
		normal(p, BLOCK_SIZE);
	A_END(0)

	A_BEGIN(1)
		prefetch(p, BLOCK_SIZE);
	A_END(1)

	A_BEGIN(2)
		unroll_prefetch(p, BLOCK_SIZE);
	A_END(2)

	// вывод результатов на консоль
	Ax_OUT("Over Prefetch",0,1);
	Ax_OUT("OPTIMIZED    ",0,2);
}


