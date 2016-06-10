#include<stdio.h>
#include<malloc.h>
#include<math.h>

int main(){
	int p = 0, r = 0;
	int** max = NULL;     //最大需求
	int** all = NULL;     //已分配
	int* ava = NULL;      //剩余资源
	int* sign = NULL;     //标志是否已分配
	int** need = NULL;
	int* que = NULL;

	printf("enter number of process:");
	scanf_s("%d", &p);
	printf("enter number of resources:");
	scanf_s("%d", &r);

	max = (int **)malloc(sizeof(int *)*p);
	all = (int **)malloc(sizeof(int *)*p);
	need = (int **)malloc(sizeof(int *)*p);
	ava = (int *)malloc(sizeof(int)*r);
	sign = (int *)malloc(sizeof(int)*(p+1));
	que = (int *)malloc(sizeof(int)*(p + 1));

	for (int i = 0; i < p; i++){
		*(max + i) = (int *)malloc(sizeof(int)*r);
		*(all + i) = (int *)malloc(sizeof(int)*r);
		*(need + i) = (int *)malloc(sizeof(int)*r);
	}
	for (int jp = 0; jp < p; jp++){
		for (int jr = 0; jr < r; jr++){
			printf("enter max[%d][%d]:", jp, jr);
			scanf_s("%d", &max[jp][jr]);
		}
	}
	for (int jp = 0; jp < p; jp++){
		for (int jr = 0; jr < r; jr++){
			printf("enter all[%d][%d]:", jp, jr);
			scanf_s("%d", &all[jp][jr]);
		}
	}
	for (int jp = 0; jp < p; jp++){
		for (int jr = 0; jr < r; jr++){
			need[jp][jr] = max[jp][jr] - all[jp][jr];
		}
	}
	for (int jp = 0; jp < p; jp++){
		sign[jp] = 0;
	}
	for (int jr = 0; jr < r; jr++){
		printf("enter ava[%d]", jr);
		scanf_s("%d", &ava[jr]);
	}
	

	int mnum = p;
	int si = 0;
	int pn = 0;
	int ds = 0;
	int pss = 1;
	while (mnum != 0){
		
		for (int kp = 0; kp < p;kp++){
			si = 0;
			ds = ds + 1;
			if (sign[kp] == 0){
				for (int kr = 0; kr < r; kr++){
					if (need[kp][kr]>ava[kr]){
						si = 1;

			         }
			     }
				if (si == 0){
					ds = 0;
					mnum = mnum - 1;
					sign[kp] = 1;
					que[pn] = kp+1;
					pn = pn + 1;
					for (int kk = 0; kk < r; kk++){
						ava[kk] = ava[kk] + all[kp][kk];
					}
				}
			}

		}
		if (ds == p){
			pss = 0;
			printf("死锁状态");
			break;
		}
	}
	if (pss == 1){
		for (int pp = 0; pp < p; pp++){
			printf("%d\t", que[pp]);
		}
	}

	printf("over");
	return 0;
}