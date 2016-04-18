/*----------------------------------------------------------------------------
 *
 *							D	0		C	P	U
 *						S				D				K
 *						=================================
 *
 *				П р о ф и л и р о в щ и к "DoCPU CLOCK" в е р с и я 1.1
 *				=======================================================
 *
 *										для внутреннего использования
 *										к книге "Техника оптимизации программ"
 *										by Крис Касперски	kpnc@programme.ru
															kk@sendmail.ru
-------------------------------------------------------------------------- */


/*============================================================================
 *
 *						ГЛОБАЛЬНЫЕ КЛЮЧИ КОМАНДНОЙ СТРОКИ
 *
 *	$Fcpu:xxx				-	принудительное задание скорости CPU
 *	$NoSort					-	не выполнять сортировку
 *
 *	$DEBUG.print.Fcpu		-	отладочная печать скорости CPU
 *
 *
 *
============================================================================*/


/*============================================================================
 *
 *
 *	A_NITER				-	сколько раз гонять профилируемый фрагмент

_NORDTSC
 * TITLE				-	заголовок, выводимый макром PRINT_TITLE
============================================================================*/
 
// шатные библиотеки
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// библиотеки DoCPU
//#define __DOS__
#include "CPU.ini"
#include "PC.ini"


// глобальные макро определения
#undef _TEXT
#ifdef __DOS__
	#define TITLE "“вЁ«Ёв  Є Є­ЁЈҐ \"’Ґе­ЁЄ   ®ЇвЁ¬Ё§ жЁЁ  Їа®Ја ¬\"  /* ­ §ў ­ЁҐ а Ў®зҐҐ */"
#else
	#define TITLE "Утилита к книге \"Техника  оптимизации  програм\"  /* название рабочее */"
#endif

#define ERR_OEM	"-ERR translates a string into the OEM-defined character set"
#define TEXT_PIPE	_TEXT("* Вывод программы должен быть перенаправлен в файл\n")

#ifndef A_NITER					// сколько раз гонять профилируемый фрагмент
	#define A_NITER			8
#endif


#ifndef DoCPU_BUFF_N			// максимальное кол-во измеряемых потоков
	#define DoCPU_BUFF_N	0x10
#endif

// неконфигурируемые определения
#define TIME_TO_WAIT		100
#define MAX_STR_LEN			256
#define DoCPU_BUFF_SIZE		A_NITER

#define	MAX_GRAPH_LEN	80
#define	GRAPH_CHAR		":"



#define A_BREAK __asm{int 0x3}

/*----------------------------------------------------------------------------
 *
 *						ОПИСАНИЕ ПРОТОТИПОВ ФУНКЦИЙ
 *
----------------------------------------------------------------------------*/

// объявление ядерных функций профилировщика из DoCPU.obj
// ------------------------------------------------------
#ifdef __cplusplus
	extern "C" int*	__cdecl	DoCPU(void* x);
	extern "C" void	__cdecl	A1(unsigned int *t);
	extern "C" int	__cdecl	A2(unsigned int *t);
#else
	extern int*		__cdecl	DoCPU(void* x);
	extern void		__cdecl	A1(unsigned int *t);
	extern int		__cdecl	A2(unsigned int *t);
#endif

// чтобы не грузить монстроузный windows.h определяем все необходимое сами
#ifndef _WINDOWS_
	extern int __stdcall Sleep(int a);
	extern int __stdcall GetTickCount(void);
	extern int __stdcall CharToOemA(char *src, char *dst);
#endif

// ERROR - есть такой макр.
#ifdef ERROR
	#undef ERROR
#endif

// ??? 
extern void _null();


/*----------------------------------------------------------------------------
 *
 *						СТАТИЧЕСКИЕ ПЕРЕМЕННЫЕ
 *						======================
 *										для использования внутри clock.h
 *
 * Внимание:
 *			не используйте данные переменные внутри ваших программ,т.к. они
 *			могут измениться в следующих версиях!
----------------------------------------------------------------------------*/
static unsigned int	DoCPU_AL_Rool;						// цикл прогонов AL
static unsigned int	DoCPU_tmp, DoCPU_tmp0, DoCPU_tmp1;	// временные перем.

static unsigned int	DoCPU_t[DoCPU_BUFF_N];
static unsigned int	DoCPU_buff[DoCPU_BUFF_N][DoCPU_BUFF_SIZE];

static char			DoCPU_v[]="|/-\\";					// символы "пропелера"
static char			DoCPU_s[MAX_STR_LEN];				// строковой буфер

float				DoCPU_float_tmp;
static int			CPU_CLOCK = 0;

static unsigned int			DoCPU_vcp=0;
static int*			p_cache=0;



/* ---------------------------------------------------------------------------
 *
 *							"ФИЗИЧЕСКИЕ" КОНСТАНТЫ
 *
----------------------------------------------------------------------------*/
#define	K			1024
#define	M			(1024*K)
#define G			(1024*M)
#define T			(1024*G)
#define	Mega		1000000
#define	Bit			8



/* ---------------------------------------------------------------------------
 *
 *							РАЗМЕРЫ БЛОКОВ ПАМЯТИ
 *
----------------------------------------------------------------------------*/
#define _HUGE		(512*M)
#define _LARGE		(MAX_AVIAL_MEM)
#define	_NORMAL		(L2_CACHE_SIZE*10)
#define _MEDIUM		(L2_CACHE_SIZE/2)
#define _SMALL		(L1_CACHE_SIZE/2)

#define MAX_CACHE_LINE_SIZE												\
					MAX(L1_CACHE_LINE_SIZE, L2_CACHE_LINE_SIZE)

#define MAX(a,b)	(((a)>(b))?a:b)
#define MIN(a,b)	((a>b)?b:a)


/*  -----------------------------------------------------------------------

                        ОПРЕДЕЛЕНИЕ РАЗМЕРА БЛОКА

    ---------------------------------------------------------------------- */
//  Определение размера обрабатываемого блока
//  Принята следующая терминология:
//  _МАЛЕНЬКИЕ_БЛОКИ_  (SMALL)  -   блоки, полносттью умещающиеся в L1-кэше
//  _УМЕРЕННЫЕ_БЛОКИ_  (MEDIUM) -   блоки, польностью умещаюшиеся в L2-кэше
//  _БОЛЬШИЕ_БЛОКИ_    (LARGE)  -   блоки, намного превосходящие размер L2 кэша
//  _ГИГАНТСКИЕ_БЛОКИ_ (HUGE)   -   блоки, намного превосходящи кол-во RAM

// Нижеслеюущей макрос создает определение _BLOCK_SIZE соотвественно одному
// из определенных ключей [__LARGE|__MEDIUM|__SMALL] и размеру кэша соотв. CPU
// по умолчанию задется большое блок
// Определения размера блока могут задаваться как через #define непосредствен-
// но в тексте программы, так и через ключ /D командной строки компилятора VC
// Например: cl bla-bla-bla.c /D__MEDIUM


#ifdef __HUGE
	#define _BLOCK_SIZE _HUGE
#endif

#ifdef __LARGE
	#define _BLOCK_SIZE _LARGE
#endif

#ifdef __MEDIUM
	#define _BLOCK_SIZE _MEDIUM
#endif

#ifdef __SMALL
	#define _BLOCK_SIZE _SMALL
#endif

// Большие блоки по умолчанию
#ifndef _BLOCK_SIZE
	#define _BLOCK_SIZE _LARGE
#endif



/*----------------------------------------------------------------------------
 *
 *							ЗАМЕРЫ ВРЕМЕНИ ВЫПОЛНЕНИЯ
 *
----------------------------------------------------------------------------*/

// БАЗОВЫЙ МАКРО ЗАМЕР ВРЕМЕНИ ВЫПОЛНЕНИЯ
// --------------------------------------
//
//		Макросы _[BEGIN | END]_[RDTSC|CLOCK]_  являются  обрертками  ядерных
// функций замера времени  выполнения  профилируемого  фргамента  программы.
// Это неизкоуровненые функции! И поэтому не вызывайте их их своей программы
//
//		Запись замеров осуществляется в буфер DoCPU_buff в "поток", заданный
// аргументов t в позицию, опеределяемую значением переменной DoCPU_AL_Rool.
#define	_BEGIN_RDTSC(t)	A1(&DoCPU_t[t]);
#define	_END_RDTSC(t)	DoCPU_tmp = A2(&DoCPU_t[t]);\
						DoCPU_buff[t][DoCPU_AL_Rool] = DoCPU_tmp;

#define	_BEGIN_CLOCK(t)	DoCPU_t[t] = clock();
#define	_END_CLOCK(t)	DoCPU_tmp = clock() - DoCPU_t[t];\
						DoCPU_buff[t][DoCPU_AL_Rool] = DoCPU_tmp;


// КОНТРОЛЬНЫЕ ТОЧКИ ВРЕМЕНИ ВЫПОЛНЕНИЯ
// ------------------------------------


//		Макрос   L_BEGIN   задает   контрольную    точку    начала   замера,
// а L_END - контрольную точку конца замера времени выполнения.
//
//		Если   задано   определение   __NORDTSC__  то  для   замера  времени
// используется  системный таймер, в   противном  же  случае  (по умолчанию)
// команда RDTSC
//
//		Макросы L_BEGIN/L_END рекомендуется использовать только в теле цикла
// AL_BEGIN -- AL_END! К данным макросам имеет смысл  прибегать лишь  в  тех
// случаях, когда перед каждым из A_NITER прогонов профилируемого  фрагмента
// необходимо выполнить определенные действия, время выполнения  которых  не
// должно учитываться. Чаще же используются макросы A_BEGIN/A_END
#ifdef _NORDTSC
	#define L_BEGIN(t)	_BEGIN_CLOCK(t)
	#define L_END(t)	_END_CLOCK(t)
#else
	#define L_BEGIN(t)	_BEGIN_RDTSC(t)
	#define L_END(t)	_END_RDTSC(t)
#endif

//		Макросы UL_BEGIN/UL_END представляют собой то же самое, что L_BEGIN/
// L_END, но выбор стратегии замера здесь может осуществляться не только  на
// стадии компиляции, но и в ходе выполнения программы.
//
//	ARG:
//		t	-	интекс потока для записи результатов замеров
//		tt	-	стратегия замера. 
//				  0 : используется RDTSC
//				!=0 : используется системный таймер
#define	UL_BEGIN(t,tt)	if (tt) {_BEGIN_CLOCK(t);}	else	{_BEGIN_RDTSC(t);}
#define	UL_END(t,tt)	if (tt) {_END_CLOCK(t);	}	else	{_END_RDTSC(t);  }



// ЦИКЛИЧЕСКАЯ ПРОФИЛИРОВКА  ПРОГОН ПРОФИЛИРУЕМОГО ФРАГМЕНТА A_NITER раз
// ---------------------------------------------------------------------

//		Макросы  AL_BEGIN/A_END  прокручивают помеченный участок A_NITER раз
// и внимание: записывают номер текущей итерации в переменную DoCPU_AL_Rool,
// определяющую позицию для записи результатов  замеров макросов [U]L_BEGIN/
// [U]L_END
#define	AL_BEGIN		for(DoCPU_AL_Rool = 0;\
						DoCPU_AL_Rool < A_NITER; DoCPU_AL_Rool++ )\
						{
#define	AL_END			}


//		Макросы  A_BEGIN/A_END   являются   завершенным   инструментом   для
// создания  контрольных   точек   замера.  Они  прокручивают  профилируемый
// фрагмент A_NITER раз, засекая время выполнения каждой итерации
//
//		ВНИМАНИЕ: ЗАМЕРЫ НЕ МОГУТ БЫТЬ ВЛОЖЕННЫМИ!!!!
#define	A_BEGIN(t)		AL_BEGIN; L_BEGIN(t);
#define	A_END(t)		L_END(t); AL_END;


// СЧИТЫВАНИЕ РЕЗУЛЬТАТОВ ЗАМЕРОВ И ИХ ОБРАБОТКА
// ---------------------------------------------

//		Макрос Ax_GET возвращает  среднее  время  выполнения  профилируемого
// фрагмента, помеченного контрльной точкой t.
#define	Ax_GET(t)		cycle_mid(DoCPU_buff[t],0)

//		Макрос  Lx_GET  возвращает  _текущее_  значение  замера  контрольной
// точки t. ВНИМАНИЕ: не используйте этот макрос, если вы не  уверены в том,
// что именно вы делаете!
#define	Lx_GET(t)		DoCPU_buff[t][DoCPU_AL_Rool]

//		Макрос   Ax_GET_MIN   возвращает    минимальное   время   выполнения
// профилируемого фрагмента, помеченного контрльной точкой t.
#define	Ax_GET_MIN(t)	cycle_min(DoCPU_buff[t],0)

//		Макрос Ax_GET_MIN возвращает  моду время   выполнения профилируемого
// фрагмента, помеченного контрльной точкой t.
#define	Ax_GET_MOD(t)	cycle_mod(DoCPU_buff[t],0)


//		Макрос ALx_GET_MIN возвращает время замера профилируемого  фргамента
// заданного контрольной точкой t в tt итерации выполнения
#define	ALx_GET(t,tt)	DoCPU_buff[t][DoCPU_AL_Rool*0 + tt]


/*----------------------------------------------------------------------------
 *
 *								ВЫВОД НА ЭКРАН
 *
----------------------------------------------------------------------------*/

// перекодировка WIN --> MS-DOS
// ----------------------------
//
//	-	до версии 1.1 должна была осуществляться для вывода кирилицы,
//		набраннойв win-редакторе на консоль
//
//	-	с версии 1.1 осуществляется прозрачно для программиста
//		ВНИМАНИЕ: двойная перкодировка искажет тект!
#define _TEXT(a)		(CharToOemA(a,DoCPU_s)?&DoCPU_s[0]:ERR_OEM)


// МАКРОСЫ ВЫВОДА ТЕКСТА НА ЭКРАН
// ------------------------------

//		Макрос  PRINT  перекодирует строку s в DOS-кодировку и выводит ее на
// терминал. Вывод макроса PRINT не перенаправляется в файл.
//		Для перенаправляемого вывода воспользуйтесь штатной функцией printf
#define	PRINT(s)		fputs(_TEXT(s),stderr);

//		Макрос ERROR выводит строку s на терминал, пикает и завершает работу
#define	ERROR(s)		{ PRINT(s); PRINT("\x7"); return 0; }

//		Макрос PRINT_TITLE выводит служебный заголовок и (c)
#define	PRINT_TITLE		PRINT("* "); PRINT(TITLE);PRINT("\n");

//		Макрос  PRINT_PIPE  сообщает о  необходимотсти  перенаправить  вывод
// приложения в файл. Это полезно для тех приложений, который оформляют свой
// вывод в виде таблицы,  рассчитанной на последующее  ее  импортирование  в
// MS Graph
#define	PRINT_PIPE		PRINT(TEXT_PIPE);



// МАКРОСЫ ВЫВОДА РЕЗУЛЬТАТОВ ЗАМЕРОВ НА ЭКРАН
// -------------------------------------------

//		Макрос L1_OUT выводит на экран строку s и надпись 100%
// Обычно он используется в паре с Lx_OUT для вывода  относительной скорости
// выполнения различных профилируемых фрагментов программы
#define	L1_OUT(s)			printf("%s : 100%%\n",_TEXT(s));

//		Макрос Lx_OUT выводит на экран строку s и отношение base и val
#define	Lx_OUT(s,base,val)	printf("%s : %3.1f%%\n",\
								_TEXT(s),(float)val/(float)base*100);

//		Макрос A1_OUT то же самое, что и L1_OUT
#define	A1_OUT(s)			L1_OUT(s)

//		Макрос A1_OUT выводит на экран строку s и время выполнения фрагмента
// программы,   помечанного  контрольной  точкой  val  относительно  времени
// фрагмента,   помеченного  контрольной  точной  base.
#define	Ax_OUT(s,base,val)	Lx_OUT(s, Ax_GET(base), Ax_GET(val))


//		Макрос A_LIST_ITER распечатывает все результаты замеров  контрольной
// точки t
#define A_LIST_ITER(t)		printf("LIST_ITER {\n");						\
							for(DoCPU_tmp=0;DoCPU_tmp<A_NITER;DoCPU_tmp++)	\
								printf("\t%d\n",DoCPU_buff[t][DoCPU_tmp]);	\
							printf("} LIST_ITER\n");


//		Макрос  A_LIST_ITER_AS_TABLE  распечатывает  все  результаты  замеров
// фрагмента программы, помеченного контрольной  точкой t и  оформляет  их  в
// виде таблицы, пригодной для импортирования в MS Graph
#define A_LIST_ITER_AS_TABLE(s,buf)											\
							printf(s);										\
							for(DoCPU_tmp=0;DoCPU_tmp<A_NITER;DoCPU_tmp++)	\
								printf("\t%d",buf[DoCPU_tmp]);				\
								printf("\n");
/*
// #define	Lx_OUTx(s,buf_base,buf_val,x_factor)\
// printf("%s : %d%%\n",s,100*(cycle_mid(DoCPU_buff[buf_val],0)/x_factor)/
// (cycle_mid(DoCPU_buff[buf_base],0)/x_factor));
// #define	Lx_OUT_DEBUG(s,buf_base,buf_val) printf("%s : %d%%, %d%%\n",s,cycle_mid(DoCPU_buff[buf_val],0),cycle_mid(DoCPU_buff[buf_base],0));
*/


// МАКРОСЫ ОТОБРАЖЕНИЯ ТЕКУЩЕГО ПРОГРЕССА
// ======================================

//		Макрос, выводящий крутящийся "пропеллер"
#define VVV	if (DoCPU_vcp >= strlen(DoCPU_v))	DoCPU_vcp = 0;				\
			sprintf(DoCPU_s,"\rDoCPU:%c\r",DoCPU_v[DoCPU_vcp]);				\
			PRINT(DoCPU_s); DoCPU_vcp++;

//		Макрос, выводящий текущий прогесс х заданный в процентах
#define PRINT_PROGRESS(x)	sprintf(DoCPU_s,"\rProgress : %3d%%\r",(x));	\
							PRINT(DoCPU_s);


// ФУНКЦИЯ ВЫВОДА ДИАГРАММ "НАЛЕТУ"
// --------------------------------
// ARG:
//		size	-	размер столбца диаграммы для вывода
//
//		n_sym	-	количество символов в строке (NULL == 80)
//
//		max		-	максимальное значение size.
//					==  0 : max равен удвоенному значению первого size
//					== -1 : сброс предыдущего form_factor и возврат
//
//		nl		-	==  0 : пропорциональная шкала,
//					==  1 : логарифмическая
void online_graph(int size, int n_sym, int max, int nl)
{
	int		a;
	int		form_factor;
	static	graph_size = 0;

	// сброс graph_size в ноль
	if (max == -1) { graph_size = 0; return; }

	// вычисление graph_size по первому size
	if ((!max) && (!graph_size)) graph_size = size * 2;

	// вычисление длины диаграммы
	if (!n_sym) n_sym = MAX_GRAPH_LEN;

	// вычисление форм-фактора
	if (!max) form_factor = graph_size / n_sym;
		else
	form_factor = max / n_sym;

	// особая обработка логарифмической строки
	if (nl)
	{
		size = 10*log(size);
		form_factor=1;
	}

	if (!form_factor) form_factor = 1;

	size /= form_factor;
	// вывод диаграммы
	for (a = 0; a < size; a++)
	{
		if (a >= (n_sym-3))
		{									// выход за пределы диаграммы
			if (size > (n_sym-3)*3) printf(">");
			if (size > (n_sym-3)*2)	printf(">");
			if (size > (n_sym-3)*1)	printf(">");
			
			
			break;
		}
		printf(GRAPH_CHAR);
	}
	printf("\n");
}

//		Макрос для быстрого вызова online_graph c аргументами по умолчанию
#define GRAPH(a) online_graph(a,0,0,0);

//		Макрос для иницилизации online_graph
#define RE_GRAPH online_graph(0,0,-1,0);


/*----------------------------------------------------------------------------
 *
 *										ВВОД
 *
----------------------------------------------------------------------------*/

// чтение области памяти. Нужно ли?
#define A_READ(p, size_p)	for(DoCPU_tmp=0; DoCPU_tmp < size_p; DoCPU_tmp += sizeof(int))\
													DoCPU_tmp1+=((int *)(p+DoCPU_tmp))[0];




/*----------------------------------------------------------------------------

						ФУНКЦИИ РАБОТЫ С КЭШЕМ И ПАМЯТЬЮ

----------------------------------------------------------------------------*/

//		Макрос _malloc32 выделяет x байт памяти, гарантируя, что  выделенный
// блок будет начинаться с адреса, кратного 32
#define	_malloc32(x)	(( (((int) malloc((x)+0x20)) + 0x20) & 0xFFFFFFE0  ));


//		Макрос _align32 выравнивает указатель p по адресу, кратному 32 байт
#define	_align32(p)		(( (((int) (p)) + 0x20) & 0xFFFFFFE0  ));


//		Макрос CLEAR_L2_CACHE  забивает кэш второго уровня мусором, вытесняя
// ранее записанные в него данные.
int CLEAR_L2_CACHE()
{
	int a;
	int x = 0;
	
	if (!p_cache) p_cache = (int *) malloc(_NORMAL);

	for (a = 0; a<_NORMAL; a++)
		x += *(int *)((int)p_cache+a);
	
	return x;
}



int CLEAR_ALL()
{
		// !!!МАКРОС НЕ РЕАЛИЗОВАН!!!
		return 0;
}


//		Данный макр сбрасывает буфера записи (store buffers) на P-III 
void _DoCPU_a_fflush()
{
#ifdef __FLUSH__
	_DoCPU_a_flush();
#endif
}



#define RM 	for(DoCPU_tmp = 0; DoCPU_tmp < 10; DoCPU_tmp++) DoCPU_float_tmp+=0.99;



/*----------------------------------------------------------------------------

								СЕРВИСНЫЕ ФУНКЦИИ

----------------------------------------------------------------------------*/

// * FUNC: int getargv(char *arg_name, char *arg_val)
// * Функция проверки наличия и получения ключей аргумента
//
//		Если аргумент  arg_name указан  в  командной  строке,   то   функция
// возвращает неотрицательное значение;
//
//		Если аргумент arg_name отсутствует в командной строке (или его длина
// превышает MAX_STR_LEN), функция возвращает -1;
//
//		Если аргумент имеет ключ,   отделенный   от   аргумента   двоеточием
// (что-то типа "/XXX:666"), его  строковое  значение возвращается в arg_val
// при условии, что arg_val не равно нулю;
//
//		Если  arg_val==NULL мы говорим функции: нам ключ аргумента не нужен;
//
//		При наличии ключа функции  так же  пытается   вернуть  его  числовое
// значение (полученное по atol);
//
//		ЗАМЕЧАНИЕ: если функция возвращает 0, то ситуация неоднозначна: либо
// аргумент вообще не имеет ключа, либо ключ нечисловое значение, либо  ключ
// действительно равен нулю. В таком случае  вы должны проверить  содержимое
// arg_val для уточнения ситуации
int getargv(char *arg_name, char *arg_val)
{
	int				a;
	int*			p;
	unsigned int	c;
	
	char			buf_arg[MAX_STR_LEN];
	char			buf_val[MAX_STR_LEN];

	#ifndef __argv		
		char** __argv;
		int __argc=0;
	#endif

	if (!arg_name) return -1;

	if (arg_val) arg_val[0] = 0;	// Иницилизация

	// Перебор всех аргументов
	for (a = 1; a < __argc; a++)
	{
		if (strlen(__argv[a]) >= MAX_STR_LEN) return -1;

		// Синтаксический разбор аргумента и копирование его имени в buf_arg
		for (c=0;c!=(1+strlen(__argv[a]));c++)
		{
			if (__argv[a][c] == ':') { buf_arg[c] = 0; break; }
				buf_arg[c] = __argv[a][c];
		}
		
		// Копирование ключа (если он есть) в buf_val
		if (__argv[a][c] == ':') strcpy(buf_val, &__argv[a][c +1]); else buf_val[0] = 0;

		// Если это тот аргумент, который нам нужен - скопировать его значаение в arg_val
		if (!strcmp(arg_name,buf_arg))
		{
			if (arg_val) strcpy(arg_val,buf_val);
			return atol(buf_val);
		}
	}
	return -1;
}

//		Макрос IFHELP при  наличии в командной строке  ключа "/?" выводит на
// терминал строку s и завершает работу программы
#define	IFHELP(s)	if (getargv("/?",0)!=-1){ PRINT(_TEXT(s)); return 0; }

//		Макрос  GETARGV   представляет   собой   "обертку"  функции getargv.
// При наличии  ключа s  в  командной  строке  он  заносит  его  значение  в
// переменную val. Если ключа s в командной строке нет, то переменная val не
// изменяется.
#define	GETARGV(s,val)	DoCPU_tmp=getargv(s,NULL);							\
						if (DoCPU_tmp!=-1) val=DoCPU_tmp;

//#define A_FLUSH _DoCPU_a_flush();
//#define A_FFLUSH _DoCPU_a_fflush();
//#define A_WAIT  Sleep(TIME_TO_WAIT);


/*----------------------------------------------------------------------------
 *
 *						ФУНКЦИИ ЯДРА ПРОФИЛИРОВЩИКА
 *
----------------------------------------------------------------------------*/
int*	A0(void* x)
{
		Sleep(100);
		return DoCPU(x);
}


/*----------------------------------------------------------------------------
 *
 *						СЕРВИСНЫЕ ФУНКЦИИ ЯДРА ПРОФИЛИРОВЩИКА
 *
----------------------------------------------------------------------------*/
// Функция возвращает тактовую частоту процессора в мегагерцах
int GetCPUclock(void)
{
	#define to_time 1

	int				tmp;
	unsigned int	t;
	volatile		time_t timer;

	tmp = getargv("$Fcpu",0);

	if (tmp <= 0)
	{
		if (!(tmp=CPU_CLOCK))
		{
			timer = time(NULL);
			while(timer == time(NULL));
			timer = time(NULL);

			A1(&t);
				while( timer+to_time > time(NULL));
			t=A2(&t);

			tmp=((double)t) / ((double)1000000) / ((double)to_time);

			CPU_CLOCK=tmp;
		}
	}
	
	if (getargv("$DEBUG.print.Fcpu",0)!=-1) printf(">DEBUG:Fcpu=%d\n",tmp);
	return tmp;
}

// Функция преобразует такты в секунды
float cpu2timeu(unsigned int ticks, int tt)
{
	if (tt)
		return ((float )ticks)/CLOCKS_PER_SEC;

	if (!CPU_CLOCK) CPU_CLOCK=GetCPUclock();
		return ((float )ticks)/CPU_CLOCK/1000000;
}


#ifdef _NORDTSC
	float cpu2time(int ticks)
	{
		return cpu2timeu(ticks, 0);
		
	}
#else
	float cpu2time(int ticks)
	{
		return cpu2timeu(ticks, 1);
	}
#endif


/*----------------------------------------------------------------------------
 *
 *						ЯДЕРНЫЕ ФУНКЦИИ ОБРАБОТКИ РЕЗУЛЬТАТОВ
 *
----------------------------------------------------------------------------*/
int cycle_min(int *buff, int n_buff)
{
	int a, tmp;
	if (!n_buff)	n_buff = DoCPU_BUFF_SIZE;

	tmp = buff[0];
	for(a = 0; a < n_buff; a++)
		if (buff[a] < tmp) tmp = buff[a];
	return tmp;
}

int cycle_max(int *buff, int n_buff)
{
	int a, tmp;
	if (!n_buff)	n_buff = DoCPU_BUFF_SIZE;
	
	tmp = buff[0];
	for(a = 0; a < n_buff; a++)
		if (buff[a] > tmp) tmp = buff[a];

	return tmp;
}


	int _compare(unsigned int *arg1, unsigned int *arg2 )
	{
		if (*arg2 >= *arg1) return -1;
		// else 
		return +1;
	}

unsigned int cycle_mid(unsigned int *buff, int nbuff)
{
	int a, xa = 0;
	if (!nbuff) nbuff = A_NITER;

	// Исключаем первый элемент
	buff = buff + 1; nbuff--;

	if (getargv("$NoSort",0) == -1)
		qsort(buff, nbuff, sizeof(int),\
				(int (*)(const void *,const void*))(_compare));

	for (a = nbuff/3; a < (2*nbuff/3); a++)
		xa += buff[a];

	xa /= (nbuff/3);

	return xa;
}


	int _Search(int *array, int array_size, int val)
	{
		int a;

		for (a = 0; a < array_size; a++)
		{
			if (array[a] == val) return a;
		}
	return -1;
	}

int cycle_mod(int *buff, int n_buff)
{
	int a,b,tmp;
	int *val_array;
	int *cnt_array;
	int array_size = 0;

	if (!n_buff) n_buff=DoCPU_BUFF_SIZE;

	val_array = (int*) malloc(n_buff*sizeof(int));
	cnt_array = (int*) malloc(n_buff*sizeof(int));

	// Набираем статистику 
	for (a=0;a<n_buff;a++)
	{
		tmp=_Search(val_array, array_size, buff[a]);
		if (tmp==-1) {array_size++;val_array[array_size-1]=buff[a];cnt_array[array_size-1]=1;}
			else
		{ cnt_array[tmp]+=1; }
	}

	// Ищем наиболее популярный элемент
	tmp=cnt_array[0];b=0;
	for (a=1;a<array_size;a++)
	{
		if (cnt_array[a]>tmp) {tmp=cnt_array[a]; b=a;}
	}

	tmp = val_array[b];
	free(val_array);
	free(cnt_array);
	return tmp;
}


float cycle_average(int* buff, int n_buff)
{
	int a;
	float avrg = 0;

	if (!n_buff) n_buff=DoCPU_BUFF_SIZE;

	for (a = 0; a < n_buff; a++)
		avrg += buff[a];

	return avrg/n_buff;
}


