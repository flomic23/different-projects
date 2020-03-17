#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <windows.h> 
#define LEFT (i) //������, ������������ ����� ������ ��������
#define RIGHT ((i + 1) % 5) //������, ������������ ����� ������� ��������

unsigned int  max_time,//����� ��� � ���
all_time,//����� ���� ��������� �� �������
started,
end_time;//����� ����� ���������
int	arr[5] = { 1,2,3,4,5 };

HANDLE fork[5]; //�������� ���������� ����� � ����������� ������� (����� - �������� �������)
HANDLE threads[5];// �������� �������� �������

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

int main(int argc, char* argv[])//phil TOTAL PHIL .��� TOTAL � ����� ����� ������ ���������(� �������������), PHIL � ����������� ����� � ��, ������� ������� ����� ��������� � ����� - ���� ���������(����, ����������)
{
	int i;
	all_time = atoi(argv[1]); //��������� �� ������ � ������������� ����������
	max_time  = atoi(argv[2]);
	/*all_time = 2000;
	max_time = 200;*/
	started = GetTickCount(); // ��������� ������� (� ��)
	end_time = started + all_time;
	printf("%d %d\n", all_time, max_time);
	
	// ������������� �������� fork: ������������ ����� ����������� 1 �������
	for (i = 0; i < 5; i++)
		fork[i] = CreateSemaphore(0, 1, 1, 0);
	// �������� �������
	for (i = 0; i < 5; i++)
	{
		threads[i] = CreateThread(0, 0, going, (char*)0 + i, 0, 0);
	}
	// ������������ ������
	WaitForMultipleObjects(5, threads,true,INFINITE);
	for (i = 0; i < 5; i++)
		CloseHandle(fork[i]);
	return 0;
}