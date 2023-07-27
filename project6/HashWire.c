#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

static const int max = 2100;//�����Ч��
static const int now = 2000;//��ǰ���
static const int born = 1978;//��ǰ���
static const int bornFake = 1980;//һ���������

void seed(unsigned char tem[])//�����������
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


void Alice(unsigned char s[],unsigned char p[],unsigned char c[])//alice�ļ��� 
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

void Carol(unsigned char p[],unsigned char c[])//Carol�ļ���
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
	unsigned char tem[16];//�����ʼ�����ӣ�128bit���������1978��
	seed(tem);
	unsigned char s[32];
	SM3_256(tem,16,s);
	printf("��%d�����Ĺ�ϣֵ��",born);
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