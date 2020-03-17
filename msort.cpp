#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <windows.h> 
HANDLE *threads;// Хранение активных потоков
int  *bord, M/*thread*/,N/*arr*/,*arr;

void mergeSort(int *a, int l, int r)
{

	if (l >= r) return; // границы сомкнулись
	int mid = (l + r) / 2; // определяем середину последовательности
						   // и рекурсивно вызываем функцию сортировки для каждой половины

	mergeSort(a, l, mid);
	mergeSort(a, mid + 1, r);
	int i = l;  // начало первого пути
	int j = mid + 1; // начало второго пути
	int *tmp = (int*)malloc((r - l + 1) * sizeof(int)); // дополнительный массив
	for (int step = 0; step < r - l + 1; step++) // для всех элементов дополнительного массива
	{
		// записываем в формируемую последовательность меньший из элементов двух путей
		// или остаток первого пути если j > r
		if ((j > r) || ((i <= mid) && (a[i] < a[j])))
		{
			tmp[step] = a[i];
			i++;
		}
		else
		{
			tmp[step] = a[j];
			j++;
		}
	}
	// переписываем сформированную последовательность в исходный массив
	for (int step = 0; step < r - l + 1; step++)
		a[l + step] = tmp[step];
	//free(*tmp);

}

DWORD WINAPI first_sort(void* P)
{
	int i = (char*)P - (char*)0, a, b;
	a = b = bord[i];
	if (a != N)
	{
		b = bord[i + 1] - 1;
		/*сортировка*/
		mergeSort(arr, a, b);
	}
	//printf("%d %d:\n", a, b);
	return 0;
}

DWORD WINAPI second_sort(void* P) 
{
	int i = (char*)P - (char*)0,a, b, c, d, j;
	printf("%d\n", i);
	a = b = c = d = bord[i];
	if (i>M) 		printf("\n%d)", i);
	if (a != N)
		b = bord[i + 1]-1;
	if (N-1 == b) return 0; //у нас только 1 кусок, а не 2, возвращaемся.
	c = bord[i + 1];//тут не будет  N, т.к. мы это исключи на предыдущей строчке
	d = bord[i + 2]-1;
	if (d < c) 
		printf("WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING WHATAR YOU DOING ");
	int *tmp = (int*)malloc((d-a+1) * sizeof(int)); // дополнительный массив
	 i = a;  // начало первого пути
	 j = c; // начало второго пути
	for (int step = 0; step <= d - a ; step++) // для всех элементов дополнительного массива
	{
		// записываем в формируемую последовательность меньший из элементов двух путей
		// или остаток первого пути если j > d
		if ((j > d) || ((i <= b) && (arr[i] < arr[j])))
		{
			tmp[step] = arr[i];
			i++;
		}
		else
		{
			tmp[step] = arr[j];
			j++;
		}
	}
	// переписываем сформированную последовательность в исходный массив
	for (int step = 0; step < d - a +1; step++)
		arr[a + step] = tmp[step];
	return 0;
}

void main(int argc, char* argv[])
{
	FILE *f_in, *f_out, *f_time;
	int i, j, started_time = 0, long_bord;
	char char_N[500], char_M[50];


	f_in = fopen("input.txt.", "r");
	f_out = fopen("output.txt", "w");
	f_time = fopen("time.txt", "w");
	fgets(char_M, 50, f_in);//read number of threads
	M = atoi(char_M);
	fprintf(f_out, "%d\n", M);//вывели в файл

	fgets(char_N, 500, f_in);//read number of elements
	fprintf(f_out, "%s", char_N);//вывели в файл
	N = atoi(char_N);
	arr = (int*)calloc(N, sizeof(int));//выделили память под массив элементов

	M = min(M, ((N - 1) / 1000 + 1));
	threads = (HANDLE*)calloc(M, sizeof(HANDLE));//выделили потоков, сколько сказано  в условии но подогнанное под 1000
	for (i = 0; i < N; i++) {
		//fscanf(f_in, "%d ", &arr[i]);//прочитали элементы массива
		arr[i] = rand();
	}
	/*считаем границы:*/
	bord = (int*)calloc(2*M+1, sizeof(int));
	long_bord = (N / M) + !!(N % M);
	bord[0] = 0;
	for (i = 1; i <= M; i++) {
		bord[i] = min(bord[i - 1] + long_bord,N);//считаем границы(последний промежуток будет меньше
	}
		for (i = 0; i < M; i++)
			printf("\n%d)%d",i, bord[i]);
	//for (i = 0; i <= M; i++) {
	//	printf("%d ", bord[i]);//просто позырить границы
	//}
	//printf("\n ");
	/*разбить на потоки и отсортировать первый раз*/
	started_time = GetTickCount(); // Получение времени (в мс)
	for (i = 0; i < M; i++)
	{
		threads[i] = CreateThread(0, 0, first_sort, (char*)0 + i, 0, 0);
	}
	WaitForMultipleObjects(M, threads, true, INFINITE);//дождались пока все потоки закончат
	/*пересоздать потоки*/
	int tmp_M;
	while (bord[1] != N)
	{
		for(i=0;i<M;i++)
		{			
			printf("%d ", i);
			threads[i] = CreateThread(0, 0, second_sort, (char*)0 + i, 0, 0);//из тех границ что были отсортировали еще раз.
		}
		WaitForMultipleObjects(M, threads, true, INFINITE);//дождались пока все потоки закончат
		for (j = 0; j * 2 <= M; j++) {

			bord[j] = bord[j * 2];
		}
		for (j; j < M; j++)
			bord[j] = 0;//не знаю нужно ли это
		M = (M + 1) / 2; 
		bord[M] = N;
		//for (i = 0; i <= M; i++) {
		//	printf("_%d ", bord[i]);//просто позырить границы
		//}
		//printf("\n ");
	}

	fprintf(f_time, "%d", GetTickCount() - started_time);//посчитали время
	for (i = 0; i < N; i++)
	{
		//fprintf(f_out, "%d ", arr[i]);//вывели в файл массив
		if (arr[i] > arr[i + 1]) {	//проверяем отсортировано ли по возрастанию
			fprintf(f_out, "false");
			break;
		}
		
	}

	system("pause");
	free(bord);
	free(threads);
	free(arr);

	fclose(f_time);
	fclose(f_in);
	fclose(f_out);
}