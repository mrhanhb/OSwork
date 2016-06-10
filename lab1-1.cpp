#include <windows.h>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

const unsigned short SIZE_OF_BUFFER = 10;   //����������
unsigned short ProductID = 0;               //��Ʒ��
unsigned short ConsumeID = 0;               //�������ĵĲ�Ʒ��
unsigned short in = 0;                      //��Ʒ��������ʱ���±�
unsigned short out = 0;                     //��Ʒ��������ʱ���±�

int g_buffer[SIZE_OF_BUFFER];               //�������Ǹ�ѭ������
bool g_continue = true;                     //���Ƴ������
HANDLE g_hMutex;                            //�����̼߳�Ļ���
HANDLE g_hFullItems;                        //�������б�ռ�õ���
HANDLE g_hEmptyItems;                       //�������еĿ���

DWORD  WINAPI Producer(LPVOID);              //�������߳�
DWORD  WINAPI Consumer(LPVOID);              //�������߳�

int main()
{
	//�������������ź�
	g_hMutex = CreateMutex(NULL, FALSE, NULL);
	g_hFullItems = CreateSemaphore(NULL, 0, SIZE_OF_BUFFER, NULL);//�����ź�
	g_hEmptyItems = CreateSemaphore(NULL, SIZE_OF_BUFFER,
		SIZE_OF_BUFFER, NULL);
	//��������ʼ��
	for (int i = 0; i< SIZE_OF_BUFFER; ++i){
		g_buffer[i] = -1;   //��ֵΪ-1ʱ����Ϊ��
	}

	const unsigned short PRODUCERS_COUNT=3;       //�����ߵĸ���
	const unsigned short CONSUMERS_COUNT=5;       //�����ߵĸ���
	
	//�ܵ��߳���
	const unsigned short THREADS_COUNT = PRODUCERS_COUNT + CONSUMERS_COUNT;
	HANDLE hThreads[THREADS_COUNT];     //���̵߳�handle
	DWORD producerID[PRODUCERS_COUNT];  //�������̵߳ı�ʶ��
	DWORD consumerID[CONSUMERS_COUNT];  //�������̵߳ı�ʶ��

	//�����������߳�
	for (int i = 0; i<PRODUCERS_COUNT; ++i){
		hThreads[i]
			= CreateThread(NULL, 0, Producer, NULL, 0, &producerID[i]);//window�ṩ��API����
		if (hThreads[i] == NULL) return -1;                       //����һ���߳�
	}
	//�����������߳�
	for (int i = 0; i<CONSUMERS_COUNT; ++i){
		hThreads[PRODUCERS_COUNT + i]
			= CreateThread(NULL, 0, Consumer, NULL, 0, &consumerID[i]);
		if (hThreads[i] == NULL) return -1;
	}

	while (g_continue){
		if (getchar()){  //���س�����ֹ��������
			g_continue = false;
		}
	}
	cout << "over";
	return 0;
}

//����һ����Ʒ
void Produce()
{
	cout << endl << "Producing " << ++ProductID << " ";
	cout << "Succeed" << endl;
}

//���������Ĳ�Ʒ���뻺����
void Append()
{
	cout << "Appending a product ... ";
	g_buffer[in] = ProductID;
	in = (in + 1) % SIZE_OF_BUFFER;
	cout << "Succeed" << endl;
	//�����������ǰ��״̬
	for (int i = 0; i<SIZE_OF_BUFFER; ++i){
		cout << i << ": ";
		if (g_buffer[i] == -1)
			cout << "null";
		else
			cout << g_buffer[i];
		if (i == in) cout << '\t' << " <-- ����";
		if (i == out) cout << '\t' << " <-- ����";
		cout << endl;
	}
}

//�ӻ�������ȡ��һ����Ʒ
void Take()
{
	cout << endl << "Taking a product ... ";
	ConsumeID = g_buffer[out];
	g_buffer[out] = -1;
	out = (out + 1) % SIZE_OF_BUFFER;
	cout << ConsumeID << "--Succeed" << endl;

	//�����������ǰ��״̬
	for (int i = 0; i<SIZE_OF_BUFFER; ++i){
		cout << i << ": ";
		if (g_buffer[i] == -1)
			cout << "null";
		else
			cout << g_buffer[i];
		if (i == in) cout << '\t' << " <-- ����";
		if (i == out) cout << '\t' << " <-- ����";
		cout << endl;
	}
}

//����һ����Ʒ
void Consume()
{
	cout << "Consuming " << ConsumeID << " ... ";
	cout << "Succeed" << endl;
}

//������
DWORD  WINAPI Producer(LPVOID lpPara)
{
	while (g_continue){
		int i = rand() % 5;
		Sleep(i * 1000);
		WaitForSingleObject(g_hEmptyItems, INFINITE);//�ȴ��źŵ�
		WaitForSingleObject(g_hMutex, INFINITE);
		Produce();
		Append();
		ReleaseMutex(g_hMutex);
		ReleaseSemaphore(g_hFullItems, 1, NULL);
	}
	return 0;
}

//������
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