#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

static const int max = 2100;//最大有效期
static const int now = 2000;//当前年份
static const int born = 1978;//当前年份
static const int bornFake = 1980;//一个测试年份

void seed(unsigned char tem[])//生成随机种子
{
	for (int i = 0; i < 8; i++)
	{
		unsigned int temp = rand();
		for (int j = 0; j < 2; j++)
		{
			tem[i * 2+ j] = temp & 0xff;
			temp = temp >> 8;
		}
	}
}


void Alice(unsigned char s[],unsigned char p[],unsigned char c[])//alice的计算 
{
	unsigned char temp[32];
	memcpy(temp, s,32);
	for (int i = 0; i < now - born; i++)
	{
		SM3_256(temp, 32, p);
		memcpy(temp, p, 32);
	}
	printf("Alice calculate finished!\n");
	memcpy(temp, s, 32);
	for (int i = 0; i < max - born; i++)
	{
		SM3_256(temp, 32, c);
		memcpy(temp, c, 32);
	}
}

void Carol(unsigned char p[],unsigned char c[])//Carol的计算
{
	for (int i = 0; i < max - now; i++)
	{
		SM3_256(p, 32, p);
	}
	if (!memcmp(p, c, 32))
		printf("okey!\n");
	else
		printf("no!\n");
}

int main()
{
	srand(time(NULL));
	unsigned char tem[16];//随机初始化种子（128bit长）分配给1978年
	seed(tem);
	unsigned char s[32];
	SM3_256(tem,16,s);
	printf("给%d年分配的哈希值：",born);
	show(s,32);
	unsigned char p[32];
	unsigned char c[32];
	Alice(s,p,c);
	printf("Alice give P:");
	show(p, 32);
	printf("Alice give C:");
	show(c, 32);
	Carol(p,c);
	printf("Carol calculate:");
	show(p,32);
}