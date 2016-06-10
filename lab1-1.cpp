#include <windows.h>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

const unsigned short SIZE_OF_BUFFER = 10;   //缓冲区长度
unsigned short ProductID = 0;               //产品号
unsigned short ConsumeID = 0;               //将被消耗的产品号
unsigned short in = 0;                      //产品进缓冲区时的下标
unsigned short out = 0;                     //产品出缓冲区时的下标

int g_buffer[SIZE_OF_BUFFER];               //缓冲区是个循环队列
bool g_continue = true;                     //控制程序结束
HANDLE g_hMutex;                            //用于线程间的互斥
HANDLE g_hFullItems;                        //缓冲区中被占用的项
HANDLE g_hEmptyItems;                       //缓冲区中的空项

DWORD  WINAPI Producer(LPVOID);              //生产者线程
DWORD  WINAPI Consumer(LPVOID);              //消费者线程

int main()
{
	//创建各个互斥信号
	g_hMutex = CreateMutex(NULL, FALSE, NULL);
	g_hFullItems = CreateSemaphore(NULL, 0, SIZE_OF_BUFFER, NULL);//创建信号
	g_hEmptyItems = CreateSemaphore(NULL, SIZE_OF_BUFFER,
		SIZE_OF_BUFFER, NULL);
	//缓冲区初始化
	for (int i = 0; i< SIZE_OF_BUFFER; ++i){
		g_buffer[i] = -1;   //当值为-1时该项为空
	}

	const unsigned short PRODUCERS_COUNT=3;       //生产者的个数
	const unsigned short CONSUMERS_COUNT=5;       //消费者的个数
	
	//总的线程数
	const unsigned short THREADS_COUNT = PRODUCERS_COUNT + CONSUMERS_COUNT;
	HANDLE hThreads[THREADS_COUNT];     //各线程的handle
	DWORD producerID[PRODUCERS_COUNT];  //生产者线程的标识符
	DWORD consumerID[CONSUMERS_COUNT];  //消费者线程的标识符

	//创建生产者线程
	for (int i = 0; i<PRODUCERS_COUNT; ++i){
		hThreads[i]
			= CreateThread(NULL, 0, Producer, NULL, 0, &producerID[i]);//window提供的API函数
		if (hThreads[i] == NULL) return -1;                       //创建一个线程
	}
	//创建消费者线程
	for (int i = 0; i<CONSUMERS_COUNT; ++i){
		hThreads[PRODUCERS_COUNT + i]
			= CreateThread(NULL, 0, Consumer, NULL, 0, &consumerID[i]);
		if (hThreads[i] == NULL) return -1;
	}

	while (g_continue){
		if (getchar()){  //按回车后终止程序运行
			g_continue = false;
		}
	}
	cout << "over";
	return 0;
}

//生产一个产品
void Produce()
{
	cout << endl << "Producing " << ++ProductID << " ";
	cout << "Succeed" << endl;
}

//把新生产的产品放入缓冲区
void Append()
{
	cout << "Appending a product ... ";
	g_buffer[in] = ProductID;
	in = (in + 1) % SIZE_OF_BUFFER;
	cout << "Succeed" << endl;
	//输出缓冲区当前的状态
	for (int i = 0; i<SIZE_OF_BUFFER; ++i){
		cout << i << ": ";
		if (g_buffer[i] == -1)
			cout << "null";
		else
			cout << g_buffer[i];
		if (i == in) cout << '\t' << " <-- 生产";
		if (i == out) cout << '\t' << " <-- 消费";
		cout << endl;
	}
}

//从缓冲区中取出一个产品
void Take()
{
	cout << endl << "Taking a product ... ";
	ConsumeID = g_buffer[out];
	g_buffer[out] = -1;
	out = (out + 1) % SIZE_OF_BUFFER;
	cout << ConsumeID << "--Succeed" << endl;

	//输出缓冲区当前的状态
	for (int i = 0; i<SIZE_OF_BUFFER; ++i){
		cout << i << ": ";
		if (g_buffer[i] == -1)
			cout << "null";
		else
			cout << g_buffer[i];
		if (i == in) cout << '\t' << " <-- 生产";
		if (i == out) cout << '\t' << " <-- 消费";
		cout << endl;
	}
}

//消耗一个产品
void Consume()
{
	cout << "Consuming " << ConsumeID << " ... ";
	cout << "Succeed" << endl;
}

//生产者
DWORD  WINAPI Producer(LPVOID lpPara)
{
	while (g_continue){
		int i = rand() % 5;
		Sleep(i * 1000);
		WaitForSingleObject(g_hEmptyItems, INFINITE);//等待信号灯
		WaitForSingleObject(g_hMutex, INFINITE);
		Produce();
		Append();
		ReleaseMutex(g_hMutex);
		ReleaseSemaphore(g_hFullItems, 1, NULL);
	}
	return 0;
}

//消费者
DWORD  WINAPI Consumer(LPVOID lpPara)
{
	while (g_continue){
		int i = rand() % 5;
		Sleep(i * 1000);
		WaitForSingleObject(g_hFullItems, INFINITE);
		WaitForSingleObject(g_hMutex, INFINITE);
		Take();
		Consume();
		ReleaseMutex(g_hMutex);
		ReleaseSemaphore(g_hEmptyItems, 1, NULL);
	}
	
	return 0;
}