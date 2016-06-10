#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#define BUFFER_SIZE 10
typedef int buffer_item;


struct v
{
	int i;
};

buffer_item buffer[BUFFER_SIZE + 1];
buffer_item front = 0, rear = 0;
HANDLE mutex, empty, full;

DWORD WINAPI producer(PVOID Param)
{
	struct v data = *(struct v *)Param;
	srand((unsigned)time(0));
	while (1)
	{
		Sleep(rand() % 11);
		WaitForSingleObject(empty, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		
		printf("producer has producerd By %d\n", data.i);
		ReleaseMutex(mutex);
		ReleaseSemaphore(full, 1, NULL);
	}
}

DWORD WINAPI consumer(PVOID Param)
{
	struct v data = *(struct v *)Param;
	srand((unsigned)time(0));
	while (1)
	{
		Sleep(rand() % 11);
		WaitForSingleObject(full, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		printf("consumer consumed By %d \n", data.i);
		ReleaseMutex(mutex);
		ReleaseSemaphore(empty, 1, NULL);
	}
}

int main(int argc, char *argv[])
{
	
	int sleeptime, pnum, snum;
	DWORD *ThreadIdP, *ThreadIdS, i;
	struct v *countp, *counts;
	HANDLE *ThreadHandleP, *ThreadHandleS;
	
	sleeptime = 100;
	printf("enter number of producter:");
	scanf_s("%d",&pnum);
	printf("enter number of consumer:");
	scanf_s("%d", &snum);
	ThreadHandleP = (HANDLE *)malloc(pnum * sizeof(HANDLE));
	ThreadHandleS = (HANDLE *)malloc(snum * sizeof(HANDLE));
	ThreadIdP = (DWORD *)malloc(pnum * sizeof(DWORD));
	ThreadIdS = (DWORD *)malloc(pnum * sizeof(DWORD));
	mutex = CreateMutex(NULL, FALSE, NULL);
	empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
	full = CreateSemaphore(NULL, 0, BUFFER_SIZE + 1, NULL);
	/*生产者*/
	countp = (struct v *)malloc((pnum + 1)*sizeof(struct v));
	for (i = 0; i<pnum; i++)
	{
		countp[i + 1].i = i + 1;
		ThreadHandleP[i] = CreateThread(NULL, 0, producer, &countp[i + 1], 0, &ThreadIdP[i]);
	}
	/*消费者*/
	counts = (struct v *)malloc((snum + 1)*sizeof(struct v));
	for (i = 0; i<snum; i++)
	{
		counts[i + 1].i = i + 1;
		ThreadHandleS[i] = CreateThread(NULL, 0, consumer, &counts[i + 1], 0, &ThreadIdS[i]);
	}
	/*运行时间*/
	Sleep(sleeptime);
	
	printf("over");	
	return 0;
}

