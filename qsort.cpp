#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <windows.h> 
#include <cmath>
HANDLE *threads;// Хранение активных потоков
HANDLE sem_write,sem_doing;
int   M/*thread*/, N/*arr*/, *arr,ind_bord=0;

typedef struct border {
	int r;
	int l;
} ;
struct border *bord;

void  push(int x, int y)
{
	WaitForSingleObject(sem_write,INFINITE);//заморозили массив
	bord[ind_bord].l = x;
	bord[ind_bord].r = y;
	ind_bord++;
	ReleaseSemaphore(sem_write,1, NULL);
}

void pop(int *x, int *y)
{
	WaitForSingleObject(sem_write, INFINITE);
	ind_bord--;
	*x = bord[ind_bord].l;
	*y = bord[ind_bord].r;
	ReleaseSemaphore(sem_write, 1, NULL);	
}

void my_qsort(int a,int b)
{	
	//printf("%d %d\n", a, b);
	
		int i=a,j=b,temp, p;
		p = arr[(j+i) >> 1];		// центральный элемент
	// процедура разделения
		do {
			while (arr[i] < p) i++;
			while (arr[j] > p) j--;

			if (i <= j) {
				temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
				i++; j--;
			}
		} while (i <= j);
		
		// рекурсивные вызовы, если есть, что сортировать 
		if (a<j)
		if (b-a > 1000) 
		{
			/*положить на стек, вызвать рекурсивно*/
			push(a, j);
			ReleaseSemaphore(sem_doing, 1, NULL);	
		}
		else  
			my_qsort(a, j);
		
		if (i < b) my_qsort(i, b); 
}

char is_not_sorted()
{
	int i;
	for (i = 0; i < N-1; i++)
		if (arr[i] > arr[i + 1]) { return 1; }
	return 0;
}

DWORD WINAPI first_q_sort(void* P)
{
	int i = (char*)P - (char*)0,a,b;
	while (1) {
		/*сортировочка*/
		WaitForSingleObject(sem_doing, INFINITE);
		if (ind_bord != 0)
		{			
			pop(&a, &b);
			my_qsort(a, b);
		}
	}
		return 0;
}

void main(int argc, char* argv[])
{
	FILE *f_in, *f_out, *f_time;
	int i, j, started_time = 0, long_bord;
	char char_N[500], char_M[10];


	f_in = fopen("input.txt.", "r");
	f_out = fopen("output.txt", "w");
	f_time = fopen("time.txt", "w");
	fgets(char_M, 10, f_in);//read number of threads
	fprintf(f_out, "%s", char_M);//вывели в файл

	fgets(char_N, 500, f_in);//read number of elements
	fprintf(f_out, "%s", char_N);//вывели в файл
	N = atoi(char_N);
	arr = (int*)calloc(N, sizeof(int));//выделили память под массив элементов
	M = (int)((N / 1000)+1 );
	M = min(M, atoi(char_M));
	threads = (HANDLE*)calloc(M, sizeof(HANDLE));//выделили потоков, сколько сказано  в условии но подогнанное под log 1000
	for (i = 0; i < N; i++) {
		fscanf(f_in, "%d ", &arr[i]);//прочитали элементы массива
		//arr[i] = (int)rand()%10;
	}	
	/*создаем потоки*/
	//2 семафора: записывать в масив границ, и выполняться (только если есть что брать из массива)
	sem_write=CreateSemaphore(0, 1, 1, NULL);
	sem_doing=CreateSemaphore(0, 0, 100000000, NULL);

	bord = (struct border*)calloc(1000+(N/1000), sizeof(struct border));
	started_time = GetTickCount(); // Получение времени (в мс)
	for (i = 0; i < M; i++)
	{
		threads[i] = CreateThread(0, 0, first_q_sort, (char*)0 + i, 0, 0);
	}
	my_qsort(0, N - 1);
	while (is_not_sorted());
	//	WaitForMultipleObjects(M, threads, true, INFINITE);//дождались пока все потоки закончат
	
	fprintf(f_time, "%d", GetTickCount() - started_time);//посчитали время
	for (i = 0; i < N ; i++)
	{
		fprintf(f_out, "%d ", arr[i]);//вывели в файл массив		
	}
	//system("pause");
	free(bord);
	free(threads);
	free(arr);

	fclose(f_time);
	fclose(f_in);
	fclose(f_out);
}