/********************************************************************************
*File name:main.c
*Author:陈明曦
*Version:1.0
*Date:2020.12.30
*Debug Environment:codeblocks
*Description:一个矩阵计算器，能实现矩阵加减，矩阵相乘，矩阵转置，求矩阵行列式，求矩
阵伴随矩阵，求矩阵逆矩阵，求矩阵秩的功能。
********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<Windows.h>												//以下三行代码皆是实现音乐播放功能。
//#include<mmsystem.h>
//#pragma comment(lib, "winmm.lib")

typedef struct matrix											//定义结构体用以保存矩阵
{
	float* a;
	int m, n;
}MATRIX;

MATRIX InputMatrix(char op, int m, int n);
MATRIX MatrixAdjoint(MATRIX A, int judge);
MATRIX Transpose(MATRIX A, int judge);
void PrintMatrix(MATRIX x);
void MatrixPlusOrMinus(int opJudge);
void MatrixMutiply(void);
void MatrixInverse(void);
void MatixRank(void);
void AllocFail(void);
float Determinant(MATRIX A, int judge);

int main(void)
{
//	PlaySound(L"Alone（cut）.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
//	//播放美妙的音乐φ(゜▽゜*)♪

	char op, choose;                                            //定义加减符号，结束字符，菜单选择字符。
	MATRIX X = { NULL, 0, 0 };                                  //定义一个无具体意义的矩阵作为实参。
	printf("------------以下为本矩阵计算器的功能菜单：------------\n\n\n");
	printf("\t\t1——实现m*n阶矩阵加减法\n");
	printf("\t\t2——实现矩阵相乘\n");
	printf("\t\t3——实现矩阵转置\n");
	printf("\t\t4——求n阶矩阵的行列式\n");
	printf("\t\t5——求n阶矩阵的伴随矩阵\n");
	printf("\t\t6——求n阶矩阵的逆矩阵\n");
	printf("\t\t7——求矩阵的秩\n");
	printf("\t\t0——退出\n");
ChooseMenu:printf("\n请输入您想使用的功能序号（0 - 7)\a");
InputChoose:scanf("%c", &choose);
	rewind(stdin);												//清空缓冲区，防止空格对其后输入产生影响。
	switch (choose)
	{
	case '0':
		printf("感谢您的使用！\n\a");
		return 0;
	case '1':
		printf("请选择加法或减法（+或-）");
	InputOP:scanf("%c", &op);
		rewind(stdin);
		if (op == '+')
		{
			MatrixPlusOrMinus(1);								//向函数传递正数，代表执行加操作。
		}
		else if (op == '-')
		{
			MatrixPlusOrMinus(-1);								//向函数传递负数，代表执行减操作。
		}
		else
		{
			printf("您的加减符号输入错误，请重新输入：");
			goto InputOP;										//输入有误则重新输入。
		}
		break;
	case '2':
		MatrixMutiply();
		break;
	case '3':
		Transpose(X, 0);
		break;
	case '4':
		Determinant(X, 0);
		break;
	case '5':
		MatrixAdjoint(X, 0);
		break;
	case '6':
		MatrixInverse();
		break;
	case '7':
		MatixRank();
		break;
	default:
		printf("您并没有输入正确的功能菜单序号，请重新输入：");
		goto InputChoose;										//输入有误则重新输入。
	}
	rewind(stdin);
	goto ChooseMenu;
}

/********************************************************************************
*函数功能：输入一任意矩阵。
*入口参数：一用来判断进行的运算种类的字符，两整形数储存上次输入矩阵的行列数。
*返回值：一代表该输入矩阵的结构体。
********************************************************************************/
MATRIX InputMatrix(char op, int m, int n)
{
	MATRIX x = { NULL, 0, 0 };
	int i, j, judge;
	if (op == ' ')												//无需对行列数做任何限制的情况。
	{
		printf("请输入该矩阵的行数:");
	INPUTM:scanf("%d", &x.m);
		if (x.m <= 0)
		{
			rewind(stdin);
			printf("请重新输入大于0的行数：");
			goto INPUTM;
		}
		printf("请输入该矩阵的列数:");
	INPUTN:scanf("%d", &x.n);
		if (x.n <= 0)
		{
			rewind(stdin);
			printf("请重新输入大于0的列数：");
			goto INPUTN;
		}
	}
	else if (op == '*')											//乘法限定第二个矩阵的行数等于第一个的列数。
	{
		printf("将为您自动分配待乘矩阵的行数，请输入其列数：\n");
	INPUTN1:scanf("%d", &x.n);
		if (x.n <= 0)
		{
			rewind(stdin);
			printf("请重新输入大于0的列数：");
			goto INPUTN1;
		}
		x.m = n;
	}
	else if (op == '+')											//加减法限定第二个矩阵的行列数与第一个矩阵相同。
	{
		printf("将为您自动分配待加减矩阵行列数。\n");
		x.m = m;
		x.n = n;
	}
	else if (op == 'D')											//输入方阵的情况。
	{
		printf("请输入该方阵的阶数：");
	INPUTM1:scanf("%d", &x.n);
		if (x.n <= 0)
		{
			rewind(stdin);
			printf("请重新输入大于0的阶数：");
			goto INPUTM1;
		}
		x.m = x.n;
	}
	x.a = (float*)calloc(x.m * x.n, sizeof(float));				//申请x.m*x.n个空间并以x.a为首地址。
	if (x.a == NULL)											//申请失败调用退出函数。
	{
		AllocFail();
	}
	printf("请您按照标准格式输入%d * %d矩阵（元素以空格隔开，每行以回车分界）\n", x.m, x.n);
	for (i = 0; i < x.m; i++)
	{
		for (j = 0; j < x.n; j++)
		{
		INPUTELEMENTS:judge = scanf("%f", x.a + i * x.n + j);	//输入矩阵元素值。
			if (judge == 0)
			{
				rewind(stdin);
				printf("请从您输入错误的元素开始重新输入合法的数字：\n");
				goto INPUTELEMENTS;
			}
		}
	}
	return x;													//返回矩阵结构体。
}

/********************************************************************************
*函数功能：打印一矩阵。
*入口参数：一代表该矩阵的结构体
*返回值：无
********************************************************************************/
void PrintMatrix(MATRIX x)
{
	int i, j;
	for (i = 0; i < x.m; i++)
	{
		for (j = 0; j < x.n; j++)
		{
			printf("%-6.2f ", x.a[i * x.n + j]);				//与*(x.a + i * x.n + j)相同。
		}
		printf("\n");
	}
	return;
}

/********************************************************************************
*函数功能：输入两矩阵，进行相加或相减操作并输出结果。
*入口参数：一正或负整型变量，用以判断函数执行加还是减操作。
*返回值：无。
********************************************************************************/
void MatrixPlusOrMinus(int opJudge)
{
	int i, j;
	MATRIX A, B, C;
	printf("请输入矩阵A\n");
	A = InputMatrix(' ', 0, 0);
	printf("请输入矩阵B\n");
	B = InputMatrix('+', A.m, A.n);
	C = A;                                                       //将输出矩阵初始化为A。
	for (i = 0; i < A.m; i++)
	{
		for (j = 0; j < A.n; j++)
		{
			if (opJudge == 1)
			{
				*(C.a + i * C.n + j) += *(B.a + i * B.n + j);    //进行加操作。
			}
			else
			{
				*(C.a + i * C.n + j) -= *(B.a + i * B.n + j);    //进行减操作。
			}
		}
	}
	if (opJudge == 1)
	{
		printf("A + B = \n");
	}
	else
	{
		printf("A - B = \n");
	}
	PrintMatrix(C);
	free(A.a);                                                    //释放内存。
	free(B.a);
	return;
}

/********************************************************************************
*函数功能：输入两矩阵，进行相乘操作并输出结果。
*入口参数：无。
*返回值：无。
********************************************************************************/
void MatrixMutiply(void)
{
	int i, j, k;
	MATRIX A, B, C;
	printf("请输入矩阵A\n");
	A = InputMatrix(' ', 0, 0);
	printf("请输入矩阵B\n");
	B = InputMatrix('*', A.m, A.n);
	C.m = A.m;
	C.n = B.n;
	C.a = (float*)calloc(A.m * B.n, sizeof(float));
	if (C.a == NULL)
	{
		AllocFail();
	}
	for (i = 0; i < C.m; i++)
	{
		for (j = 0; j < C.n; j++)
		{
			*(C.a + i * C.n + j) = 0;
			for (k = 0; k < A.n; k++)                            //A.n = B.m
			{
				*(C.a + i * C.n + j) += (*(A.a + i * A.n + k)) * (*(B.a + k * B.n + j));
			}                                                   //与人工计算一致的累加算法。
		}
	}
	printf("A * B =\n");
	PrintMatrix(C);
	free(A.a);
	free(B.a);
	free(C.a);
	return;
}

/********************************************************************************
*函数功能：求一矩阵的转置矩阵并打印。
*入口参数：无。
*返回值：无。
********************************************************************************/
MATRIX Transpose(MATRIX A, int judge)
{
	int i, j;
	MATRIX B;
	if (judge == 0)
	{
		printf("请输入待转置矩阵\n");
		A = InputMatrix(' ', 0, 0);
	}
	B.a = (float*)calloc(A.n * A.m, sizeof(float));             //分配A.n*A.m个空间，首地址为B.a。
	if (B.a == NULL)
	{
		AllocFail();
	}
	B.m = A.n;													//初始化B.m,B.n。
	B.n = A.m;
	for (i = 0; i < A.m; i++)
	{
		for (j = 0; j < A.n; j++)
		{
			*(B.a + j * B.n + i) = *(A.a + i * A.n + j);		//为B赋值。
		}
	}
	free(A.a);
	if (judge == 0)
	{
		printf("A矩阵的转置为\n");
		PrintMatrix(B);
		free(B.a);
	}
	return B;
}

/********************************************************************************
*函数功能：计算一矩阵的行列式值。
*入口参数：一个整形变量用来判断，若为0则为用户调用，为1则为其他函数调用，为-1则
		   是自身调用。
*返回值：浮点数为行列式值。
********************************************************************************/
float Determinant(MATRIX A, int judge)
{
	if (judge == 0)
	{
		A = InputMatrix('D', 0, 0);
	}
	int i, j, k;
	float result = 0;
	if (A.m == 1)
	{
		result = A.a[0];
	}
	else
	{
		MATRIX* p = (MATRIX*)calloc(A.m, sizeof(MATRIX));							//建立动态数组，储存A的阶数个余子式。
		if (p == NULL)
		{
			AllocFail();
		}
		for (i = 0; i < A.m; i++)													//i是计算余子式那列。
		{
			(p + i)->m = A.m - 1;
			(p + i)->n = (p + i)->m;
			(p + i)->a = (float*)calloc((A.n - 1) * (A.m - 1), sizeof(float));		//为这些余子式分配空间。
			if ((p + i)->a == NULL)
			{
				AllocFail();
			}
			for (j = 0; j < A.m - 1; j++)											//写入对应余子式的元素，j遍历列。
			{
				for (k = 0; k < A.m - 1; k++)										//遍历每个余子式的行。
				{
					if (j < i)
					{
						(p + i)->a[k * (A.m - 1) + j] = A.a[(k + 1) * A.m + j];		//小于那列则直接原列赋值。
					}
					else
					{
						(p + i)->a[k * (A.m - 1) + j] = A.a[(k + 1) * A.m + j + 1];	//大于等于那列则加一列赋值。
					}
				}
			}
			if (i % 2 == 0)
			{
				result += A.a[i] * Determinant(p[i], -1);
			}
			else
			{
				result -= A.a[i] * Determinant(p[i], -1);
			}
		}
	}
	if (judge != 1)
	{
		if (judge == 0)
		{
			printf("行列式的值为%.2f\n", result);
		}
		free(A.a);
	}
	return result;
}

/********************************************************************************
*函数功能：求一矩阵的秩并打印。
*入口参数：无。
*返回值：无。
********************************************************************************/
void MatixRank(void)
{
	MATRIX A;
	int i, j, k, l, rank;
	float times;
	A = InputMatrix(' ', 0, 0);
	if (A.a == NULL)
	{
		AllocFail();
	}
	rank = A.m;
	for (i = 0; i < A.m; i++)
	{
		for (j = 0; fabs(A.a[i * A.n + j]) < 1e-6; j++);         //找到每一行第一个不为零的元素的列数j。
		for (k = i + 1; k < A.m; k++)                            //做将该行下每一行的j阶元素都化为0的消法变换。
		{
			times = -A.a[k * A.n + j] / A.a[i * A.n + j];		 //计算消法变换的倍数。
			for (l = j; l < A.n; l++)
			{
				A.a[k * A.n + l] += times * A.a[i * A.n + l];
			}
		}
	}
	for (i = 0; i < A.m; i++)                                    //若有一行全为0，则秩减少1。
	{
		rank--;
		for (j = 0; j < A.n; j++)
		{
			if (fabs(A.a[i * A.n + j]) > 1e-6)
			{
				rank++;
				break;
			}
		}
	}
	printf("该矩阵的秩为%d\n", rank);
	free(A.a);
	return;
}

/********************************************************************************
*函数功能：求一矩阵的伴随矩阵并打印。
*入口参数：一个整形变量用来判断，若为0则自行输入矩阵运行计算，否则计算入口实参矩阵。
*返回值：一代表伴随矩阵的结构体。
********************************************************************************/
MATRIX MatrixAdjoint(MATRIX A, int judge)
{
	MATRIX B, C;
	int i, j, k, l;
	if (judge == 0)
	{
		A = InputMatrix('D', 0, 0);
	}
	B.m = A.m;
	B.n = A.n;
	B.a = (float*)calloc(B.m * B.n, sizeof(float));
	if (B.a == NULL)
	{
		AllocFail();
	}
	C.m = B.m - 1;
	C.n = B.n - 1;
	C.a = (float*)calloc(C.m * C.n, sizeof(float));
	if (C.a == NULL)
	{
		AllocFail();
	}
	for (i = 0; i < B.m; i++)                                    //i遍历A的行。
	{
		for (j = 0; j < B.n; j++)                                //j遍历A的列。通过ij循环先做出一个遍历A所有元素的前提情况。
		{
			for (k = 0; k < i; k++)
			{
				for (l = 0; l < j; l++)                          //A中行和列都小于划去行列的元素原位赋值给C。
				{
					C.a[k * C.n + l] = A.a[k * A.n + l];
				}
				for (l = j + 1; l < A.n; l++)                    //A中行小于，列大于划去元素的元素列减1后赋值给C。
				{
					C.a[k * C.n + l - 1] = A.a[k * A.n + l];
				}
			}
			for (k = i + 1; k < A.m; k++)
			{
				for (l = 0; l < j; l++)                          //A中行大于，列小于划去元素的元素行减1后赋值给C。
				{
					C.a[(k - 1) * C.n + l] = A.a[k * A.n + l];
				}
				for (l = j + 1; l < A.n; l++)                    //A中行列都大于划去元素的元素行列都减1后赋值给C。
				{
					C.a[(k - 1) * C.n + l - 1] = A.a[k * A.n + l];
				}

			}
			if ((i + j) % 2 == 0)
			{
				B.a[i * B.n + j] = Determinant(C, 1);           //此处及以下都是计算A[i][j]，也就是对应的正负C行列式值。
			}
			else
			{
				B.a[i * B.n + j] = -Determinant(C, 1);
			}
		}
	}
	B = Transpose(B, 1);
	free(C.a);
	if (judge == 0)
	{
		printf("该矩阵的伴随矩阵为：\n");
		PrintMatrix(B);
		free(A.a);
		free(B.a);
	}
	return B;
}

/********************************************************************************
*函数功能：求一矩阵的逆矩阵并打印。
*入口参数：无。
*返回值：无。
********************************************************************************/
void MatrixInverse(void)
{
	int i, j;
	MATRIX A, B, C;
	float divisor;
	A = InputMatrix('D', 0, 0);
	if (A.m == 1)
	{
		printf("该矩阵的逆矩阵为：\n%-6.2f", 1.0 / A.a[0]);
		return;
	}
	C = A;                                                      //因之后求行列式函数会改变参数，故新建一个C矩阵。
	C.a = (float*)calloc(C.m * C.n, sizeof(float));
	if (C.a == NULL)
	{
		AllocFail();
	}
	for (i = 0; i < C.m; i++)
	{
		for (j = 0; j < C.n; j++)
		{
			C.a[i * C.n + j] = A.a[i * A.n + j];
		}
	}
	divisor = Determinant(C, 1);                                //求A的行列式值。
	if (divisor == 0)
	{
		printf("该方阵不可逆！");
		return;
	}
	B = MatrixAdjoint(A, 1);                                    //求A的伴随矩阵。
	printf("该矩阵的逆矩阵为：\n");
	for (i = 0; i < B.m; i++)
	{
		for (j = 0; j < B.n; j++)
		{
			printf("%-6.2f ", B.a[i * B.n + j] / divisor);
		}
		printf("\n");
	}
	free(A.a);
	free(B.a);
	free(C.a);
	return;
}

/********************************************************************************
*函数功能：当动态内存申请失败时退出程序。
*入口参数：无。
*返回值：无。
********************************************************************************/
void AllocFail(void)
{
	printf("未能成功为您分配内存，请重试。");
	exit(1);
}
