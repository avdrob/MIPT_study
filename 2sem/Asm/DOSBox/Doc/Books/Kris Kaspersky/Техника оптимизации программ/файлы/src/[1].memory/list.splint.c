/* ---------------------------------------------------------------------------
 * @
 *							УТИЛИТА ДЛЯ ДЕМОНСТАРЦИИ
 *			ЭФЕКТИВНОСТИ ПАРАЛЛЕЛЬНОЙ ОБРАБОКИ ССЫЛОЧНЫХ ДАННЫХ
 *			Т.Е.	ТАК		НАЗЫВАЕМОГО		РАСШЕПЛЕНИЯ СПИСКОВ
 *			---------------------------------------------------
 *
 * Bild 0x001  13.06.2002
--------------------------------------------------------------------------- */

// КОНФИГУРАЦИЯ
#define	BLOCK_SIZE	(8*M)					// размер обрабатываемого блока



struct MYLIST{								// элемент списка
		struct MYLIST *next;
		int	val;
};

// вычисление кол-ва элементов
#define N_ELEM		(BLOCK_SIZE/sizeof(struct MYLIST))


#include <DoCPU.h>
main()
{
	int a;
	struct MYLIST *p, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8;

	// выделение памяти
	struct MYLIST *one_list   = (struct MYLIST*) malloc(BLOCK_SIZE);
	struct MYLIST *spl_list_1 = (struct MYLIST*) malloc(BLOCK_SIZE/2);
	struct MYLIST *spl_list_2 = (struct MYLIST*) malloc(BLOCK_SIZE/2);
	struct MYLIST *spl_list_3 = (struct MYLIST*) malloc(BLOCK_SIZE/4);
	struct MYLIST *spl_list_4 = (struct MYLIST*) malloc(BLOCK_SIZE/4);
	struct MYLIST *spl_list_5 = (struct MYLIST*) malloc(BLOCK_SIZE/6);
	struct MYLIST *spl_list_6 = (struct MYLIST*) malloc(BLOCK_SIZE/6);
	struct MYLIST *spl_list_7 = (struct MYLIST*) malloc(BLOCK_SIZE/8);
	struct MYLIST *spl_list_8 = (struct MYLIST*) malloc(BLOCK_SIZE/8);

	// TITLE
	PRINT("=== демонстрация эффективности расшепления списков ===\n");
	PRINT_TITLE; VVV;


	/* -----------------------------------------------------------------------
	 *
	 *						обработка одного списка
	 *
	----------------------------------------------------------------------- */
	// иницилизация
	for (a = 0; a < N_ELEM; a++)
	{
		one_list[a].next = one_list + a + 1;
		one_list[a].val  = a;
	} one_list[N_ELEM-1].next = 0;
	// трассировка
	A_BEGIN(1)
		p = one_list;
		while(p = p[0].next);
	A_END(1)
	printf("%s %d\n",_TEXT("один список................"), Ax_GET(1));


	/* -----------------------------------------------------------------------
	 *
	 *					обработка двух расщепленных списков
	 *
	----------------------------------------------------------------------- */
	CLEAR_L2_CACHE(); VVV;
	// иницилизация
	for (a = 0; a < N_ELEM/2; a++)
	{
		spl_list_1[a].next = spl_list_1 + a + 1;
		spl_list_1[a].val  = a;

		spl_list_2[a].next = spl_list_2 + a + 1;
		spl_list_2[a].val  = a;

	} spl_list_1[N_ELEM/2-1].next = 0;
	  spl_list_2[N_ELEM/2-1].next = 0;

	// трассировка
	A_BEGIN(2)
		p1 = spl_list_1; p2 = spl_list_2;
		while((p1 = p1[0].next) && (p2 = p2[0].next));
		// внимание! данный способ трассировки предполагает, что оба списки
		// равны по количеству элементов, в  противном  случае  потребуется
		// слегда доработать код, например, так:
		// while(p1 || p2)
		// {
		//		if (p1) p1 = p1[0].next;
		//		if (p2) p2 = p2[0].next;
		// }
		// однако это сделает его менее наглядным, поэтому в книге приводится
		// первый вариант
	A_END(2)
	printf("%s %d\n",_TEXT("два расшепленных списка...."),Ax_GET(2));


	/* -----------------------------------------------------------------------
	 *
	 *					обработка четырех расщепленных списков
	 *
	----------------------------------------------------------------------- */
	CLEAR_L2_CACHE(); VVV;
	// иницилизация
	for (a = 0; a < N_ELEM/4; a++)
	{
		spl_list_1[a].next = spl_list_1 + a + 1;
		spl_list_1[a].val  = a;

		spl_list_2[a].next = spl_list_2 + a + 1;
		spl_list_2[a].val  = a;

		spl_list_3[a].next = spl_list_3 + a + 1;
		spl_list_3[a].val  = a;

		spl_list_4[a].next = spl_list_4 + a + 1;
		spl_list_4[a].val  = a;
	} spl_list_1[N_ELEM/4-1].next = 0; spl_list_2[N_ELEM/4-1].next = 0;
	  spl_list_3[N_ELEM/4-1].next = 0; spl_list_4[N_ELEM/4-1].next = 0;

	// трассировка
	A_BEGIN(3)
		p1 = spl_list_1; p2 = spl_list_2; p3 = spl_list_3; p4 = spl_list_4;
		while( (p1 = p1[0].next) && (p2 = p2[0].next)
			&& (p3 = p3[0].next) && (p4 = p4[0].next));
	A_END(3)
	printf("%s %d\n",_TEXT("четыре  расшепленных списка"), Ax_GET(3));


	/* -----------------------------------------------------------------------
	 *
	 *					обработка шести расщепленных списков
	 *
	----------------------------------------------------------------------- */
	CLEAR_L2_CACHE(); VVV;
	// иницилизация
	for (a=0;a < N_ELEM/6;a++)
	{
		spl_list_1[a].next = spl_list_1 + a + 1;
		spl_list_1[a].val  = a;

		spl_list_2[a].next = spl_list_2 + a + 1;
		spl_list_2[a].val  = a;

		spl_list_3[a].next = spl_list_3 + a + 1;
		spl_list_3[a].val  = a;

		spl_list_4[a].next = spl_list_4 + a + 1;
		spl_list_4[a].val  = a;

		spl_list_5[a].next = spl_list_5 + a + 1;
		spl_list_5[a].val  = a;

		spl_list_6[a].next = spl_list_6 + a + 1;
		spl_list_6[a].val  = a;
	} spl_list_1[N_ELEM/6-1].next = 0; spl_list_2[N_ELEM/6-1].next = 0;
	  spl_list_3[N_ELEM/6-1].next = 0; spl_list_4[N_ELEM/6-1].next = 0;
	  spl_list_5[N_ELEM/6-1].next = 0; spl_list_6[N_ELEM/6-1].next = 0;

	// трассировка
	A_BEGIN(4)
		p1 = spl_list_1; p2 = spl_list_2; p3 = spl_list_3; p4 = spl_list_4;
		p5 = spl_list_5; p6 = spl_list_6;
		while( (p1 = p1[0].next) && (p2 = p2[0].next)
			&& (p3 = p3[0].next) && (p4 = p4[0].next)
			&& (p5 = p5[0].next) && (p6 = p6[0].next));
	A_END(4)
	printf("%s %d\n",_TEXT("шесть  расшепленных списков"), Ax_GET(4));


	/* -----------------------------------------------------------------------
	 *
	 *					обработка восьми расщепленных списков
	 *
	----------------------------------------------------------------------- */
	CLEAR_L2_CACHE();
	// иницилизация
	for (a=0;a < N_ELEM/8;a++)
	{
		spl_list_1[a].next = spl_list_1 + a + 1;
		spl_list_1[a].val  = a;

		spl_list_2[a].next = spl_list_2 + a + 1;
		spl_list_2[a].val  = a;

		spl_list_3[a].next = spl_list_3 + a + 1;
		spl_list_3[a].val  = a;

		spl_list_4[a].next = spl_list_4 + a + 1;
		spl_list_4[a].val  = a;

		spl_list_5[a].next = spl_list_5 + a + 1;
		spl_list_5[a].val  = a;

		spl_list_6[a].next = spl_list_6 + a + 1;
		spl_list_6[a].val  = a;

		spl_list_7[a].next = spl_list_7 + a + 1;
		spl_list_7[a].val  = a;

		spl_list_8[a].next = spl_list_8 + a + 1;
		spl_list_8[a].val  = a;

	} spl_list_1[N_ELEM/8-1].next = 0; spl_list_2[N_ELEM/8-1].next = 0;
	  spl_list_3[N_ELEM/8-1].next = 0; spl_list_4[N_ELEM/8-1].next = 0;
	  spl_list_5[N_ELEM/8-1].next = 0; spl_list_6[N_ELEM/8-1].next = 0;
	  spl_list_7[N_ELEM/8-1].next = 0; spl_list_8[N_ELEM/8-1].next = 0;

	// трассировка
	A_BEGIN(5)
		p1 = spl_list_1; p2 = spl_list_2;p3 = spl_list_3; p4 = spl_list_4;
		p5 = spl_list_5; p6 = spl_list_6;p5 = spl_list_5; p6 = spl_list_6;
		p7 = spl_list_7; p8 = spl_list_8;
		while( (p1 = p1[0].next) && (p2 = p2[0].next)
			&& (p3 = p3[0].next) && (p4 = p4[0].next)
			&& (p5 = p5[0].next) && (p6 = p6[0].next)
			&& (p7 = p7[0].next) && (p8 = p8[0].next));
	A_END(5)
	printf("%s %d\n",_TEXT("восемь расшепленных списков"), Ax_GET(5));
}

_P_S()
{
/*
		Реальность такова, какой ее описывает язык
				тезис лингвистической относительности Б.Л. Уорфа
*/
}
