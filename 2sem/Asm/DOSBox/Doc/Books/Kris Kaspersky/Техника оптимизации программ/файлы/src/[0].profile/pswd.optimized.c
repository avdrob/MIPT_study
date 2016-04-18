/*----------------------------------------------------------------------------
 *
 *				ПРИМЕР, ДЕМОНСТРИРУЮЩИЙ ТЕХНИКУ ОПТИМИЗАЦИИ
 *			и профилировку с помощью профилировщика Intel VTune
 *			====================================================
 *
 * Build 0x001 12.08.2002
----------------------------------------------------------------------------*/
// STEP 1
//			Вызов printf для отображения прогесса идет через 6666 итераций

// STEP 2
//			Выносим strlen за тело цикла в функции CalculateCRC

//STEP 3
//			Выравнивание pswd

// STEP 4
//			Вычисление длины внутри gen_pswd

// STEP 5
//			Удаляем вычисление прогресса из gen_pswd

// STEP 6
//			Удалаем отображение текущего состояния

// STEP 7
//			Инлайним функции

// КОНФИГУРАЦИЯ
#define ITER 40000000						// макс. итераций
#define MAX_CRYPT_LEN	200					// макс. длина шифротекста

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>

int length;
/*----------------------------------------------------------------------------
 *
 *				процедура расшифровки шифротекста найденным паролем
 *				---------------------------------------------------
 *	ARG:
 *		pswd		:	пароль
 *		crypteddata	:	шифротекст
 *
 *	RET:
 *		*crypreddata:	расшифрованный шифротест
 *
 *	NOTE:
 *		none
-----------------------------------------------------------------------------*/
DeCrypt(char *pswd, char *crypteddata)
{

	unsigned int p = 0;		// указатель текущей позиции расшифровываемых данных

	// * * * ОСНОВНОЙ ЦИКЛ РАСШИФРОВКИ * * *
	do {
		// расшифровываем текущий символ
		crypteddata[p] ^= pswd[p % strlen(pswd)];
		// внимание: взятие остатка ^^^ _очень_ медленная операция
		// тем не менее здесь ей можно пользоваться  безбоязненно,
		// т.к. эта функция вызывается  крайне редко (если  вообще
		// вызывается

		// переходим к расшифровке следующего символа
	} while(++p < strlen(crypteddata));
}

int gen_pswd(char *crypteddata, char *pswd, int max_iter, int validCRC)
{
	int a, b, x;
	int p = 0;
	char *buff;
	int y=0;
	int k;
	int length = strlen(pswd);
	
	x = -1;
	for (b = 0; b <= length;  b++)
		x += *(int *)((int)pswd + b);

	// генерировать пароли
	for(a = 0; a <  max_iter ; a++)
	{

		if (x==validCRC)
		{
		// копируем шифроданные во временный буфер
		buff = (char *) malloc(strlen(crypteddata));
		strcpy(buff, crypteddata);

		// расшифровываем
		DeCrypt(pswd, buff);
	
			// выводим результат расшифровки на экран
			printf("CRC %8X: try to decrypt: \"%s\"\n", validCRC,buff);
		}
		y = 1;
		k = 0x59;
		while((++pswd[p])>'z')
		{
			pswd[p] = '!';
			// следующий символ
			y = y | y << 8;
			x -= k;
			k = k << 8;
			k += 0x59;
			
			p++;
			if (!pswd[p])
			{
					pswd[p]='!';
					pswd[p+1]=0;
					length++;
						x = -1;
						for (b = 0; b <= length;  b++)
						x += *(int *)((int)pswd + b);
					pswd[p]=' ';
						y = 0;
			}
			//printf("%x\n",y);
		} // end while(pswd)
		p = 0;
		x+=y;
	} // end for(a)

	return 0;
}

/*----------------------------------------------------------------------------
 *
 *				Функциия выводит число, разделяя разряды точками
 *				------------------------------------------------
 *	ARG:
 *		per			:	число для вывода
 *
 *	RET:
 *		none
 *
 *	NOTE:
 *		функция выводит на экран число, усекая его до целой части
 *
-----------------------------------------------------------------------------*/
print_dot(float per)
{
	// * * * КОНФИГУРАЦИЯ * * *
	#define N			3		// отделять по три разряда
								// при выводе HEX надо отделть два

	#define DOT_SIZE	1		// размер точки-разделителя

	#define	DOT			"."		// разделитель
	
	int		a;
	char	buff[666];

	sprintf(buff,"%0.0f", per);
	/* ^^^^^^^^^^^^^^^^ формат вывода */

	// * * * цикл рабора числа по рязрядам * * *
	for(a = strlen(buff) - N; a > 0; a -= N)		// <-- смещаем
	{ /* ^^^^^^^^^^^^^^^^ - это глупый код, - не вызывайте функцию часто */

			memmove(buff + a + DOT_SIZE, buff + a, 66);
			/* внимание!						^^^^^^^^^ */

			if(buff[a]==' ') break;	// достигнут пробел - конец работы
				else
			// копируем разделитель
			memcpy(buff + a, DOT, DOT_SIZE);
	}
	// выводиим на экран
	printf("%s\n",buff);
}


main(int argc, char **argv)
{
	// переменные
	FILE *f;				// для чтения исходного файла (если есть)
	char *buff;				// для чтения данных исходного файла
	char *pswd;				// текущий тестируемый пароль (need by gen_pswd)
	int validCRC;			// для хранения оригинального CRC пароля
	unsigned int t;			// для замера времени исполнения перебора
	int iter = ITER;		// макс. кол-во перебираемых паролей
	char *crypteddata;		// для хранения шифрованных

	//	build-in default crypt
	//	кто прочтет, что здесь  зашифровано, тот  постигет  Великую  Тайну
	//	Крис Касперски ;)
	char _DATA_[] = "\x4B\x72\x69\x73\x20\x4B\x61\x73\x70\x65\x72\x73\x6B"\
					"\x79\x20\x44\x65\x6D\x6F\x20\x43\x72\x79\x70\x74\x3A"\
					"\xB9\x50\xE7\x73\x20\x39\x3D\x30\x4B\x42\x53\x3E\x22"\
					"\x27\x32\x53\x56\x49\x3F\x3C\x3D\x2C\x73\x73\x0D\x0A";

	// TITLE
	printf("= = = VTune profiling demo = = =\n==================================\n");

	// HELP
	if (argc==2)
	{
			printf("USAGE:\n\tpswd.exe [StartPassword MAX_ITER]\n");
			return 0;
	}
	
	// выделение памяти
	printf("memory malloc\t\t");
	buff = (char *) malloc(MAX_CRYPT_LEN);
	if (buff) printf("+OK\n"); else {printf("-ERR\n"); return -1;}

	// получение шифротекста для расшифровки
	printf("get source from\t\t");
	if ((f=fopen("crypted.dat","r"))!=0)
	{
		printf("crypted.dat\n");
		fgets(buff,MAX_CRYPT_LEN, f);
	}
	else
	{
		printf("build-in data\n");
		buff=_DATA_;
	}

	// выделение CRC
	validCRC=*(int *)((int) strstr(buff,":")+1);
	printf("calculate CRC\t\t%X\n",validCRC);
	if (!validCRC)
	{
		printf("-ERR: CRC is invalid\n");
		return -1;
	}

	// выделение шифрованных данных
	crypteddata=strstr(buff,":") + 5;
	//printf("cryptodata\t\t%s\n",crypteddata);

	// выделение памяти для парольного буфера 
	printf("memory malloc\t\t");
	pswd = (char *) malloc(512*1024);
	//pswd+=62;

	memset(pswd,0,666);		// <-- иницилизация 

	if (pswd) printf("+OK\n"); else {printf("-ERR\n"); return -1;}
	
	// разбор аргмуентов командной строки
	// получение стартового пароля и макс. кол-ва итераций
	printf("get arg from\t\t");
	if (argc>2)
	{
		printf("command line\n");
		if(atol(argv[2])>0) iter=atol(argv[2]);
		strcpy(pswd,argv[1]);
	}
		else
	{
		printf("build-in default\n");
		strcpy(pswd,"!");
	}
	printf("start password\t\t%s\nmax iter\t\t%d\n",pswd,iter);
	

	// начало перебора паролей
	printf("==================================\ntry search... wait!\n");
	t=clock();
		gen_pswd(crypteddata,pswd,iter,validCRC);
	t=clock()-t;

	// вывод кол-ва перебираемых паролей за сек
	printf("                                       \rPassword per sec:\t");
	print_dot(iter/(float)t*CLOCKS_PER_SEC);
	//printf("%d\n",t);

	return 0;
}