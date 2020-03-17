#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <windows.h> 
#define LEFT (i) //макрос, определ€ющий номер левого философа
#define RIGHT ((i + 1) % 5) //макрос, определ€ющий номер правого философа

unsigned int  max_time,//врем€ сна и еды
all_time,//длина всей программы по условию
started,
end_time;//врем€ начаа программы
int	arr[5] = { 1,2,3,4,5 };

HANDLE fork[5]; //взаимное исключение входа в критическую область (вз€ть - положить палочки)
HANDLE threads[5];// ’ранение активных потоков

DWORD WINAPI going(void* P)
{
	int i = (char*)P - (char*)0;
//	unsigned int end_time ;
	unsigned int time;
	while (1)
	{
		
		//printf("(%d)",i);
		if (GetTickCount() >= end_time) return 0;
		WaitForSingleObject(fork[LEFT], INFINITE);//take left fork
		WaitForSingleObject(fork[RIGHT], INFINITE);//take right fork
		if (GetTickCount() >= end_time) 
		{
			ReleaseSemaphore(fork[RIGHT], 1, NULL);//put left fork
			ReleaseSemaphore(fork[LEFT], 1, NULL);//put right fork
			return 0; 
		}
		printf("%4d:%d:T->E\n", GetTickCount()-started, 1 + i);//TIME:NUM:OLD->NEW
		Sleep(max_time);
		ReleaseSemaphore(fork[RIGHT], 1, NULL);//put left fork
		ReleaseSemaphore(fork[LEFT], 1, NULL);//put right fork
		printf("%4d:%d:E->T\n", GetTickCount()- started, 1 + i);
		Sleep(max_time);
	}
}

int main(int argc, char* argv[])//phil TOTAL PHIL .√де TOTAL Ч общее врем€ работы программы(в миллисекундах), PHIL Ч минимальное врем€ в мс, которое философ может проводить в каком - либо состо€нии(есть, размышл€ть)
{
	int i;
	all_time = atoi(argv[1]); //переводим из строки в целочисленную переменную
	max_time  = atoi(argv[2]);
	/*all_time = 2000;
	max_time = 200;*/
	started = GetTickCount(); // ѕолучение времени (в мс)
	end_time = started + all_time;
	printf("%d %d\n", all_time, max_time);
	
	// »нициализаци€ семафора fork: одновременно могут выполн€тьс€ 1 потоков
	for (i = 0; i < 5; i++)
		fork[i] = CreateSemaphore(0, 1, 1, 0);
	// —оздание потоков
	for (i = 0; i < 5; i++)
	{
		threads[i] = CreateThread(0, 0, going, (char*)0 + i, 0, 0);
	}
	// ќсвобождение пам€ти
	WaitForMultipleObjects(5, threads,true,INFINITE);
	for (i = 0; i < 5; i++)
		CloseHandle(fork[i]);
	return 0;
}