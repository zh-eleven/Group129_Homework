#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

//定义常量,这样定义默认看作无符号整型
#define IV0 0x7380166f
#define IV1 0x4914b2b9
#define IV2 0x172442d7
#define IV3 0xda8a0600
#define IV4 0xa96f30bc
#define IV5 0x163138aa
#define IV6 0xe38dee4d
#define IV7 0xb0fb0e4e
#define T1 0x79cc4519
#define T2 0x7a879d8a

typedef struct
{
	unsigned int len;
	unsigned int curlen;
	unsigned char buf[64];
	unsigned  int state[8];
}SM3;

unsigned int FF1(X, Y, Z)
{
	return (X ^ Y ^ Z);
}

unsigned int FF2(X, Y, Z)
{
	return ((X & Y) | (X & Z) | (Y & Z));
}

unsigned int GG1(X, Y, Z)
{
	return X ^ Y ^ Z;
}

unsigned int GG2(X, Y, Z)
{
	return (X & Y) | (~X & Z);
}

unsigned int Rleft(x, n)
{
	return ((unsigned int)x) << n | ((unsigned int)x) >> (32 - n);
}

unsigned int P1(X)
{
	return X ^ Rleft(X, 9) ^ Rleft(X, 17);
}

unsigned int P2(X)
{
	return X ^ Rleft(X, 15) ^ Rleft(X, 23);
}

void BigEndian(unsigned char src[], int bytelen, unsigned char des[])//src和des指向同一个数组
{
	unsigned char temp = 0;
	for (int i = 0; i < bytelen / 4; i++)
	{
		temp = src[i * 4];
		des[i * 4] = src[i * 4 + 3];
		des[i * 4 + 3] = temp;
		temp = src[i * 4 + 1];
		des[i * 4 + 1] = src[i * 4 + 2];
		des[i * 4 + 2] = temp;
	}

}

void init(SM3* md)
{
	md->state[0] = IV0;
	md->state[1] = IV1;
	md->state[2] = IV2;
	md->state[3] = IV3;
	md->state[4] = IV4;
	md->state[5] = IV5;
	md->state[6] = IV6;
	md->state[7] = IV7;
	md->len = 0;
	md->curlen = 0;
}

void ME(unsigned int B[], unsigned int W[], unsigned  int W1[])
{
	for (int i = 0; i < 16; i++)
		W[i] = B[i];
	for (int n = 16; n < 68; n++)
		W[n] = P2(W[n - 16] ^ W[n - 9] ^ Rleft(W[n - 3], 15)) ^ Rleft(W[n - 13], 7) ^ W[n - 6];
	for (int j = 0; j < 64; j++)
		W1[j] = W[j] ^ W[j + 4];
}

void CF(unsigned int W[], unsigned int W1[], unsigned V[])
{
	unsigned int A = V[0];
	unsigned int B = V[1];
	unsigned int C = V[2];
	unsigned int D = V[3];
	unsigned int E = V[4];
	unsigned int F = V[5];
	unsigned int G = V[6];
	unsigned int H = V[7];
	unsigned int SS1;
	unsigned int SS2;
	unsigned int TT1;
	unsigned int TT2;
	for (int i = 0; i < 64; i++)
	{
		if (i < 16)
		{
			SS1 = Rleft(Rleft(A, 12) + E + Rleft(T1, i), 7);
		}
		else
		{
			SS1 = Rleft(Rleft(A, 12) + E + Rleft(T2, i), 7);
		}
		SS2 = SS1 ^ Rleft(A, 12);
		if (i < 16)
		{
			TT1 = FF1(A, B, C) + D + SS2 + W1[i];
			TT2 = GG1(E, F, G) + H + SS1 + W[i];
		}
		else
		{
			TT1 = FF2(A, B, C) + D + SS2 + W1[i];
			TT2 = GG2(E, F, G) + H + SS1 + W[i];
		}
		D = C;
		C = Rleft(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = Rleft(F, 19);
		F = E;
		E = P1(TT2);
	}
	V[0] = A ^ V[0];
	V[1] = B ^ V[1];
	V[2] = C ^ V[2];
	V[3] = D ^ V[3];
	V[4] = E ^ V[4];
	V[5] = F ^ V[5];
	V[6] = G ^ V[6];
	V[7] = H ^ V[7];
}

void compress(SM3* md)
{
	unsigned int W[68];
	unsigned int W1[64];
	BigEndian(md->buf, 64, md->buf);
	ME((unsigned int*)md->buf, W, W1);
	CF(W, W1, md->state);
}

void SM3_first(SM3* md, unsigned char* buf, int len)
{
	while (len--)
	{
		md->buf[md->curlen] = *buf++;
		md->curlen++;
		if (md->curlen == 64)
		{
			compress(md);
			md->len += 512;
			md->curlen = 0;
		}
	}
}

void SM3_last(SM3* md, unsigned char hash[])
{
	int i;
	unsigned char tmp = 0;
	md->len += md->curlen << 3;
	md->buf[md->curlen] = 0x80;
	md->curlen++;
	if (md->curlen > 56)
	{
		for (; md->curlen < 64;)
		{
			md->buf[md->curlen] = 0;
			md->curlen++;
		}
		compress(md);
		md->curlen = 0;
	}
	for (; md->curlen < 56;)
	{
		md->buf[md->curlen] = 0;
		md->curlen++;
	}
	for (i = 56; i < 60; i++)
	{
		md->buf[i] = 0;
	}
	md->buf[63] = md->len & 0xff;
	md->buf[62] = (md->len >> 8) & 0xff;
	md->buf[61] = (md->len >> 16) & 0xff;
	md->buf[60] = (md->len >> 24) & 0xff;
	compress(md);
	memcpy(hash, md->state, 256 / 8);
	BigEndian(hash, 256 / 8, hash);
}

void show(unsigned char hash[])
{
	for (int i = 0; i < 32; i++)
		printf("%02x ", hash[i]);
	printf("\n");
}

void SM3_256(unsigned char buf[], int len, unsigned char hash[])
{
	SM3 md;
	init(&md);
	SM3_first(&md, buf, len);
	SM3_last(&md, hash);
}

int SM3_SelfTest()
{
	unsigned int i = 0, a = 1, b = 1;
	unsigned char Msg1[3] = { 0x61,0x62,0x63 };
	int MsgLen1 = 3;
	unsigned char MsgHash1[32] = { 0 };
	unsigned char
		StdHash1[32] = { 0x66,0xC7,0xF0,0xF4,0x62,0xEE,0xED,0xD9,0xD1,0xF2,0xD4,0x6B,0xDC,0x10,0xE4,0xE2,
		0x41,0x67,0xC4,0x87,0x5C,0xF2,0xF7,0xA2,0x29,0x7D,0xA0,0x2B,0x8F,0x4B,0xA8,0xE0 };
	unsigned char
		Msg2[64] = { 0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

		0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

		0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

		0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64 };
	int MsgLen2 = 64;
	unsigned char MsgHash2[32] = { 0 };
	unsigned char
		StdHash2[32] = { 0xde,0xbe,0x9f,0xf9,0x22,0x75,0xb8,0xa1,0x38,0x60,0x48,0x89,0xc1,0x8e,0x5a,0x4d,		0x6f,0xdb,0x70,0xe5,0x38,0x7e,0x57,0x65,0x29,0x3d,0xcb,0xa3,0x9c,0x0c,0x57,0x32 };
	SM3_256(Msg1, MsgLen1, MsgHash1);
	SM3_256(Msg2, MsgLen2, MsgHash2);
	a = memcmp(MsgHash1, StdHash1, 32);
	b = memcmp(MsgHash2, StdHash2, 32);
	show(MsgHash1);
	show(MsgHash2);
	if ((a == 0) && (b == 0))
	{
		printf("success!\n");
		return 0;
	}
	else
	{
		printf("failure!\n");
		return 1;
	}
}

int main()
{
	unsigned int  a = 12345;
	unsigned char* b = &a;
	//for (int i = 0; i < sizeof(int); i++)
	//{
	//	printf("%.2x ", b[i]);
	//}
	//printf("\n");
	unsigned char
		StdHash2[32] = { 0xde,0xbe,0x9f,0xf9,0x22,0x75,0xb8,0xa1,0x38,0x60,0x48,0x89,0xc1,0x8e,0x5a,0x4d,		0x6f,0xdb,0x70,0xe5,0x38,0x7e,0x57,0x65,0x29,0x3d,0xcb,0xa3,0x9c,0x0c,0x57,0x32 };
	unsigned char
		StdHash1[32] = { 0x66,0xC7,0xF0,0xF4,0x62,0xEE,0xED,0xD9,0xD1,0xF2,0xD4,0x6B,0xDC,0x10,0xE4,0xE2,
		0x41,0x67,0xC4,0x87,0x5C,0xF2,0xF7,0xA2,0x29,0x7D,0xA0,0x2B,0x8F,0x4B,0xA8,0xE0 };
	srand(time(NULL));
	double run_time;
	LARGE_INTEGER time_start;	//开始时间
	LARGE_INTEGER time_over;	//结束时间
	double dqFreq;		//计时器频率
	LARGE_INTEGER f;	//计时器频率
	QueryPerformanceFrequency(&f);
	
	
	dqFreq = (double)f.QuadPart;

	int MsgLen2 = 64;
	unsigned char MsgHash2[32] = { 0 };
	unsigned char
		Msg2[64] = { 0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

		0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

		0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,

		0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64 };
	for (int i = 0; i < 64; i++)
		printf("\\x%0x", Msg2[i]);
	QueryPerformanceCounter(&time_start);	//计时开始
	for(int i=0;i<10000;i++)
	SM3_256(Msg2, MsgLen2, MsgHash2);
	QueryPerformanceCounter(&time_over);	//计时结束
	run_time = 1 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	printf("\n计算512比特10000次的时间:%fs\n", run_time);

}