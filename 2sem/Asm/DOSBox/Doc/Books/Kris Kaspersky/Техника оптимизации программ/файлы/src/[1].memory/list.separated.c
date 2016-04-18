/* ---------------------------------------------------------------------------
 * @
 *							УТИЛИТА ДЛЯ ДЕМОНСТАРЦИИ
 *			ЭФЕКТИВНОСТИ ПАРАЛЛЕЛЬНОЙ ОБРАБОКИ РАЗДЕЛЬНЫХ СПИСКОВ
 *			-----------------------------------------------------
 *
 * Bild 0x001  14.06.2002
--------------------------------------------------------------------------- */
#define N_ELEM	4000	// количество элементов списка

struct list{			// КЛАССИЧЕСКИЙ СПИСОК
	struct list	*next;	// указатель на следующий узел
	int			val;	// значение
};

struct mylist{			// ОПТИМИЗИРОВАННЫЙ РАЗДЕЛЬНЫЙ СПИСОК
	short int *next;	// массив указателей на следующий узел
	int *val;			// массив значений
};

#include <DoCPU.h>
main()
{
	int a;
	int b = 0;
	struct list *classic_list,*tmp_list;
	struct mylist separated_list;

	// TITLE
	PRINT("= = = демонстрация эффективности параллельной обработки нескольких списков = = =\n");
	PRINT_TITLE;

	/* -----------------------------------------------------------------------
	 *
	 *						обработка классического списка
	 *
	------------------------------------------------------------------------ */
	// выделение памяти
	classic_list = (struct list*) _malloc32(N_ELEM * sizeof(struct list));

	// иницилизация списка
	for (a = 0; a < N_ELEM; a++)
	{
		classic_list[a].next= classic_list + a+1;
		classic_list[a].val = a;
	} classic_list[N_ELEM-1].next = 0;
	
	// трассировка списка
	AL_BEGIN;
		CLEAR_L2_CACHE();
		L_BEGIN(0)
			tmp_list=classic_list;
			while(tmp_list = tmp_list[0].next);
		L_END(0)
	AL_END;
	L1_OUT("CLASSIC   ");


	/* ----------------------------------------------------------------------
	 *
	 *			обработка оптимизированного раздельного списка
	 *
	----------------------------------------------------------------------- */
	// выделение памяти
	separated_list.next = (short int *) _malloc32(N_ELEM*sizeof(short int));
	separated_list.val  = (int *)       _malloc32(N_ELEM*sizeof(int));
	
	// иницилизация списка
	for (a=0;a<N_ELEM;a++)
	{
		separated_list.next[a] = a+1;
		/*                 ^^^ обратите внимание где находятся
		                       квадратные скобки */
		separated_list.val[a] = a;
	} separated_list.next[N_ELEM-1] = 0;

	// трассировка списка
	AL_BEGIN;
		CLEAR_L2_CACHE();
		L_BEGIN(1)
			while(b=separated_list.next[b]);
		L_END(1)
	AL_END;
	Lx_OUT("OPTIMIZED ",Ax_GET(0),Ax_GET(1));
}

_P_S()
{
/*
		"текст не застывшая сущность, но диалог между автором,
		читателем и исследователем"
						Тезис философской поэтики М. М. Бахтина 
*/
}