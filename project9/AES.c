#include "mbedtls/cipher.h"
#include "mbedtls/dhm.h"
#include "mbedtls/md.h"
#include<windows.h> 
#include "mbedtls/aes.h"

//S盒
const unsigned char S_Table[256] =
{
0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
//字节代换
void SubByte(unsigned char* text, int w)
{
	if (w == 0) {
		for (int i = 0; i < 4; i++)
		{
			for (int n = 0; n < 4; n++)
			{
				int raw = (*(text + n + i * 4) >> 4);//高四位做行
				int column = (*(text + n + i * 4) & 0x0F);//低四位做列
				*(text + n + i * 4) = *(S_Table + column + raw * 16);
			}
		}
	}
	else//为了G函数
	{
		for (int i = 0; i < 4; i++)
		{
			int raw = (*(text + i) >> 4);//高四位做行
			int column = (*(text + i) & 0x0F);//低四位做列
			*(text + i) = *(S_Table + column + raw * 16);
		}
	}
}
//逆S盒
const unsigned char ReS_Table[256] =
{
0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};
//逆字节代换
void ReSubByte(unsigned char* cipher, int g)
{
	if (g == 0) {
		for (int i = 0; i < 4; i++)
		{
			for (int n = 0; n < 4; n++)
			{
				int raw = (*(cipher + n + i * 4) >> 4);//高四位做行
				int column = (*(cipher + n + i * 4) & 0x0F);//低四位做列
				*(cipher + n + i * 4) = *(ReS_Table + column + raw * 16);
			}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			int raw = (*(cipher + i) >> 4);//高四位做行
			int column = (*(cipher + i) & 0x0F);//低四位做列
			*(cipher + i) = *(S_Table + column + raw * 16);
		}
	}


}
//行位移
void Shift(unsigned char* text)
{
	//shift 1
	unsigned char temp = *(text + 1);
	for (int i = 0; i < 3; i++)
	{
		*(text + 1 + 4 * i) = *(text + 5 + 4 * i);
	}
	*(text + 13) = temp;
	//shift 2
	unsigned char temp1 = *(text + 10);
	*(text + 10) = *(text + 2);
	*(text + 2) = temp1;
	unsigned char temp2 = *(text + 6);
	*(text + 6) = *(text + 14);
	*(text + 14) = temp2;
	//shift 3
	unsigned char temp3 = *(text + 15);
	for (int i = 0; i < 3; i++)
	{
		*(text + 15 - i * 4) = *(text + 11 - i * 4);
	}
	*(text + 3) = temp3;
}
//逆行位移
void ReShift(unsigned char* text)
{
	//shift 1
	unsigned char temp = *(text + 13);
	for (int i = 0; i < 3; i++)
	{
		*(text + 13 - 4 * i) = *(text + 9 - 4 * i);
	}
	*(text + 1) = temp;
	//shift 2
	unsigned char temp1 = *(text + 2);
	*(text + 2) = *(text + 10);
	*(text + 10) = temp1;
	unsigned char temp2 = *(text + 14);
	*(text + 14) = *(text + 6);
	*(text + 6) = temp2;
	//shift 3
	unsigned char temp3 = *(text + 3);
	for (int i = 0; i < 3; i++)
	{
		*(text + 3 + 4 * i) = *(text + 7 + 4 * i);
	}
	*(text + 15) = temp3;
}
//显示
void show(unsigned char* text, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%02X", text[i]);
		printf("  ");
	}
}
//混和矩阵
unsigned char MixC[16] = {
	0x02,0x03,0x01,0x01,
	0x01,0x02,0x03,0x01,
	0x01,0x01,0x02,0x03,
	0x03,0x01,0x01,0x02
};
//混和逆矩阵
unsigned char ReMixC[16] = {
	0x0E,0x0B,0x0D,0x09,
	0x09,0x0E,0x0B,0x0D,
	0x0D,0x09,0x0E,0x0B,
	0x0B,0x0D, 0x09, 0x0E
};
//列混合
void Mixcolumn(unsigned char* text, unsigned char* MixC1)
{
	unsigned char result[16];
	for (int i = 0; i < 4; i++)
	{
		for (int n = 0; n < 4; n++)
		{
			unsigned char temp = 0x00;
			for (int k = 0; k < 4; k++)
			{
				//有限域上的运算
				switch (*(MixC1 + i * 4 + k))
				{
				case 0x01:
				{
					temp ^= *(text + n*4+k);
					break;
				}
				case 0x02:
				{
					if ((*(text + n * 4 + k) >> 7) == 0x00)
					{
						temp ^= ((*(text + n * 4 + k)) << 1) % 0x100;
					}
					else
					{
						temp ^= ((*(text + n * 4 + k)) << 1) % 0x100 ^ 0x1B;
					}
					break;
				}
				case 0x03:
				{
					if ((*(text + n * 4 + k) >> 7) == 0x00)
					{
						temp ^= ((*(text + n * 4 + k)) << 1) % 0x100;
					}
					else
					{
						temp ^= ((*(text + n * 4 + k)) << 1) % 0x100 ^ 0x1B;
					}
					temp ^= *(text + n * 4 + k);
					break;
				}		
			}
			}
			result[n * 4 + i] = temp;
		}
	}
	//将结果写入
	for (int i = 0; i < 16; i++)
	{
		*(text + i) = *(result + i);
	}
}
//逆列混合
void ReMixcolumn(unsigned char* text, unsigned char* MixC1)
{
	unsigned char result[16];
	for (int i = 0; i < 4; i++)
	{
		for (int n = 0; n < 4; n++)
		{
			unsigned char temp = 0x00;
			for (int k = 0; k < 4; k++)
			{
				unsigned char temp1 = *(text + n * 4 + k);
			
				//有限域上的运算
				switch (*(MixC1 + i*4 + k))
				{
				case 0x09://8+1
				{
					for (int i = 0; i < 3; i++)
					{
						if ((temp1 >> 7) == 0x00)
						{
							temp1 = (temp1 << 1) % 0x100;
						}
						else
						{
							temp1 = (temp1 << 1) % 0x100 ^ 0x1B;
						}
					
					}
					temp ^= temp1^*(text + n * 4 + k);

					break;
				}
				case 0x0B://8+2+1
				{
					for (int i = 0; i < 3; i++)
					{
						if ((temp1 >> 7) == 0x00)
						{
							temp1 = (temp1 << 1) % 0x100;
						}
						else
						{
							temp1 = (temp1 << 1) % 0x100 ^ 0x1B;
						}
						
					}
					temp ^= temp1;
					
					temp1 = *(text + n * 4 + k);
					
					for (int i = 0; i < 1; i++)
					{
						if ((temp1 >> 7) == 0x00)
						{
							temp1 = (temp1 << 1) % 0x100;
						}
						else
						{
							temp1 = (temp1 << 1) % 0x100 ^ 0x1B;
						}
						
					}
					temp ^= temp1; 
					temp^=* (text + n * 4 + k);
				
					break;
				}
				case 0x0D://8+4+1
				{
					for (int i = 0; i < 3; i++)
					{
						if ((temp1 >> 7) == 0x00)
						{
							temp1 = (temp1 << 1) % 0x100;
						}
						else
						{
							temp1 = (temp1 << 1) % 0x100 ^ 0x1B;
						}
						
					}
					temp ^= temp1 ;
					
					temp1 = *(text + n * 4 + k);

					for (int i = 0; i < 2; i++)
					{
						if ((temp1 >> 7) == 0x00)
						{
							temp1 = (temp1 << 1) % 0x100;
						}
						else
						{
							temp1 = (temp1 << 1) % 0x100 ^ 0x1B;
						}
					}
					temp ^= temp1 ^ *(text + n * 4 + k);
					break;
				}
				case 0x0E://8+4+2
				{
					for (int i = 0; i < 3; i++)
					{
						if ((temp1 >> 7) == 0x00)
						{
							temp1 = (temp1 << 1) % 0x100;
						}
						else
						{
							temp1 = (temp1 << 1) % 0x100 ^ 0x1B;
						}
					
					}
					temp ^= temp1;
				
					temp1 = *(text + n * 4 + k);
					for (int i = 0; i < 2; i++)
					{
						if ((temp1 >> 7) == 0x00)
						{
							temp1 = (temp1 << 1) % 0x100;
						}
						else
						{
							temp1 = (temp1 << 1) % 0x100 ^ 0x1B;
						}
						
					}
					temp ^= temp1;
					
					temp1 = *(text + n * 4 + k);
					for (int i = 0; i < 1; i++)
					{
						if ((temp1 >> 7) == 0x00)
						{
							temp1 = (temp1 << 1) % 0x100;
						}
						else
						{
							temp1 = (temp1 << 1) % 0x100 ^ 0x1B;
						}
						
					}
					temp ^= temp1;
					
					temp1 = *(text + n * 4 + k);
					break;
				}
				}
				
			}
			result[n * 4 + i] = temp;
		}
	}
	//将结果写入
	for (int i = 0; i < 16; i++)
	{
		*(text + i) = *(result + i);
	}
}
//轮常量
unsigned char Rcon[10] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36 };
//G函数
unsigned char* G(unsigned char* key, int i)//i位论常量次数 
{
	unsigned char re[4] = { *(key + 13),*(key + 14) ,*(key + 15) ,*(key + 12) };
	SubByte(&re[0], 1);
	*(&re[0]) ^= *(Rcon + i);
	*(&re[0] + 1) ^= 0x00;
	*(&re[0] + 2) ^= 0x00;
	*(&re[0] + 3) ^= 0x00;
	return re;
}
//G函数的逆
unsigned char* GInverse(unsigned char* key, int i)//i位论常量次数 
{
	unsigned char re[4] = { *(key + 3) ,*(key + 7),*(key + 11) ,*(key + 15) };
	*(&re[0]) ^= *(Rcon + i);
	*(&re[0] + 1) ^= 0x00;
	*(&re[0] + 2) ^= 0x00;
	*(&re[0] + 3) ^= 0x00;
	ReSubByte(&re[0], 1);
	unsigned char temp[4] = { *(key + 15), *(key + 3),*(key + 7),*(key + 11) };
	return temp;
}
//密钥拓展
void KeyExpand(unsigned char* key, int n, int g)
{
	if (g == 0) {
		unsigned char* W = G(key, n);
		for (int i = 0; i < 4; i++)
		{
			*(key + i) = *(key + i) ^ *(W + i);
		}
		for (int i = 0; i < 4; i++)
		{
			*(key + i + 4) = *(key + i + 4) ^ *(key + i);
		}
		for (int i = 0; i < 4; i++)
		{
			*(key + i + 8) = *(key + i + 8) ^ *(key + i + 4);
		}
		for (int i = 0; i < 4; i++)
		{
			*(key + i + 12) = *(key + i + 12) ^ *(key + i + 8);
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			*(key + i + 12) = *(key + i + 12) ^ *(key + i + 8);
		}
		for (int i = 0; i < 4; i++)
		{
			*(key + i + 8) = *(key + i + 8) ^ *(key + i + 4);
		}
		for (int i = 0; i < 4; i++)
		{
			*(key + i + 4) = *(key + i + 4) ^ *(key + i);
		}
		unsigned char* W = G(key, n);
		for (int i = 0; i < 4; i++)
		{
			*(key + i) = *(key + i) ^ *(W + i);
		}
	}
}
////密钥加
void Add(unsigned char* ciphertext, unsigned char* key)
{
	for (int i = 0; i < 16; i++)
	{
		*(ciphertext + i) = *(ciphertext + i) ^ *(key + i);
	}
}
void MyAES(unsigned char* plaintext, unsigned char* key, unsigned char* ciphertext)//128-bit
{
	//初始置换
	for (int i = 0; i < 16; i++)
	{
		*(ciphertext + i) = *(plaintext + i) ^ *(key + i);
	}
	//9轮变换
	for (int i = 0; i < 9; i++)
	{
		SubByte(ciphertext, 0);
		Shift(ciphertext);
		Mixcolumn(ciphertext, &MixC[0]);
		KeyExpand(key, i, 0);
		Add(ciphertext, key);
	}
	SubByte(ciphertext, 0);
	Shift(ciphertext);
	KeyExpand(key, 9, 0);
	Add(ciphertext, key);
}
void Decry(unsigned char* plaintext, unsigned char* key, unsigned char* ciphertext)
{
	//初始置换
	for (int i = 0; i < 16; i++)
	{
		*(plaintext + i) = *(ciphertext + i) ^ *(key + i);
	}
	//9轮变换
	for (int i = 0; i < 9; i++)
	{
		ReShift(plaintext);
		ReSubByte(plaintext, 0);
		KeyExpand(key, 9 - i, 1);
		Add(plaintext, key);
		ReMixcolumn(plaintext, &ReMixC[0]);

	}
	ReShift(plaintext);
	ReSubByte(plaintext, 0);
	KeyExpand(key, 0, 1);
	Add(plaintext, key);
}
int main()
{
	//aes_test(MBEDTLS_CIPHER_AES_128_CBC);
	unsigned char plaintext0[16] = { 0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
	unsigned char key0[16] = { 0x0f,0x15,0x71,0xc9,0x47,0xd9,0xe8,0x59,0x0c,0xb7,0xad,0xd6,0xaf,0x7f,0x67,0x98 };
	unsigned char plaintext1[128] = "2021004601420000";
	unsigned char key1[128] = "2021004601420000";
	unsigned char plaintext2[128] = "2021004600670000";
	unsigned char key2[128] = "2021004600670000";
	unsigned char ciphertext0[128];
	unsigned char ciphertext1[128];
	unsigned char ciphertext2[128];
	unsigned char mm0[128];
	unsigned char mm1[128];
	unsigned char mm2[128];

	printf("明文0:\n");
	show(plaintext0, 16);
	printf("\n密钥1:\n");
	show(key0, 16);
	MyAES(plaintext0, key0, ciphertext0);
	printf("\n加密后的密文:\n");
	show(ciphertext0, 16);
	printf("\n");
	Decry(mm0, key0, ciphertext0);
	printf("解密后的明文:\n");
	show(mm0, 16);
	printf("\n\n————————————————————————————————\n\n");
	
	printf("明文1:\n");
	show(plaintext1, 16);
	printf("\n密钥1:\n");
	show(key1, 16);
	MyAES(plaintext1, key1, ciphertext1);
	printf("\n加密后的密文:\n");
	show(ciphertext1, 16);
	printf("\n");
	Decry(mm1, key1, ciphertext1);
	printf("解密后的明文:\n");
	show(mm1, 16);
	printf("\n\n————————————————————————————————\n\n");

	printf("明文2:\n");
	show(plaintext2, 16);
	printf("\n密钥1:\n");
	show(key2, 16);
	MyAES(plaintext2, key2, ciphertext2);
	printf("\n加密后的密文:\n");
	show(ciphertext2, 16);
	printf("\n");
	Decry(mm2, key2, ciphertext2);
	printf("解密后的明文:\n");
	show(mm2, 16);
	printf("\n\n————————————————————————————————\n\n");

	double time = 0;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);//开始计时 
	for (int i = 0; i < 256; i++)
	{
		plaintext0[0] += i;
		MyAES(plaintext1, key1, ciphertext1);
	}
	QueryPerformanceCounter(&nEndTime);//停止计时  
	time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;//计算程序执行时间单位为s  
	printf("实验编写程序加密时间:%fms\n", time*1000);

	double time1 = 0;
	LARGE_INTEGER nFreq1;
	LARGE_INTEGER nBeginTime1;
	LARGE_INTEGER nEndTime1;
	QueryPerformanceFrequency(&nFreq1);
	QueryPerformanceCounter(&nBeginTime1);//开始计时 
	for (int i = 0; i < 256; i++)
	{
		aes_test(MBEDTLS_CIPHER_AES_128_CBC,i);
	}
	QueryPerformanceCounter(&nEndTime1);//停止计时  
	time1 = (double)(nEndTime1.QuadPart - nBeginTime1.QuadPart) / (double)nFreq1.QuadPart;//计算程序执行时间单位为s  
	printf("库函数编写程序加密时间:%fms", time1 * 1000);
	return 1;
}

