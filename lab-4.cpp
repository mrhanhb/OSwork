#include<stdio.h>
#include<malloc.h>
#include<math.h>

int getBlockNum(const long long int addr);

int main(){

	long long int addr = 0;
	int blocknum = 0;
	int yeneinum = 0;
	printf("�����߼���ַ(ʮ����)��");
	scanf_s("%d", &addr);

	printf("inode��ṹ��\n");
	printf("ֱ��������\n\t���ݵ�ַ-->����\nһ��������\n\tһ��������ַ-->���ݵ�ַ-->����\n����������\n\tһ��������ַ-->����������ַ-->���ݵ�ַ-->����\n����������\n\tһ��������ַ-->����������ַ-->����������ַ-->���ݵ�ַ-->����\n");

	blocknum = getBlockNum(addr);
	yeneinum = addr % 1024;
	printf("�����ţ�%d",blocknum);
	printf("����ƫ�ƣ�%d",yeneinum);
	
	printf("over");
	return 0;
}

int getBlockNum(const long long int addr){
	int blocksize = 1024;
	int numofblock = 0;
	/*����*/
	int Zero_Block[8];   //0-7
	int *Ind_Block;      //8-263һ��
	int **Dou_Block;      //264-65799����
	int ***Tri_Block;     //65800-16843015����
	int sign1 = 0;        //��¼ÿ����������
	int sign2 = 0;
	int sign3 = 0;
	/*��ʼ��*/
	for (int zi = 0; zi < 8; zi++){         //0
		Zero_Block[zi] = numofblock;
		numofblock = numofblock + 1;
	}
	Ind_Block = (int *)malloc(1024);        //1
	for (int fi = 0; fi < 256; fi++){
		*(Ind_Block + fi) = numofblock;
		numofblock = numofblock + 1;
	}
	sign1 = numofblock;
	Dou_Block = (int **)malloc(1024);       //2
	for (int si = 0; si < 256; si++){
		*(Dou_Block + si) = (int *)malloc(1024);
		for (int sj = 0; sj < 256; sj++){
			*(*(Dou_Block + si) + sj) = numofblock;
			numofblock = numofblock + 1;
		}
	}
	sign2 = numofblock;
	Tri_Block = (int ***)malloc(1024);       //3
	for (int ti = 0; ti < 256; ti++){
		*(Tri_Block + ti) = (int **)malloc(1024);
		for (int tj = 0; tj < 256; tj++){
			*(*(Tri_Block + ti) + tj) = (int *)malloc(1024);
			for (int tk = 0; tk < 256; tk++){
				*(*(*(Tri_Block + ti) + tj) + tk) = numofblock;
				numofblock = numofblock + 1;
			}
		}
	}
	sign3 = numofblock;

	int reblock = 0;
	int blocknum = 0;
	blocknum = addr / 1024;
	/*�жϿ��*/
	if (blocknum < 8 && blocknum >= 0)
		reblock = Zero_Block[blocknum];
	else if (blocknum >= 8 && blocknum < sign1)
		reblock = *(Ind_Block + blocknum - 8);
	else if (blocknum >= sign1&&blocknum < sign2)
		reblock = *(*(Dou_Block + (blocknum - sign1) / 256) + (blocknum - sign1) % 256);
	else if (blocknum >= sign2&&blocknum < sign3)
		reblock = *(*(*(Tri_Block + (blocknum - sign2) / (256 * 256)) + (blocknum - sign2) / 256) + (blocknum - sign2) % 256);
	else{
		printf("��ַԽ��");
		reblock = -1;
	}

	return reblock;
}