﻿#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//IP置换
char IP[8][8] =
    {
        {58, 50, 42, 34, 26, 18, 10, 2},
        {60, 52, 44, 36, 28, 20, 12, 4},
        {62, 54, 46, 38, 30, 22, 14, 6},
        {64, 56, 48, 40, 32, 24, 16, 8},
        {57, 49, 41, 33, 25, 17, 9, 1},
        {59, 51, 43, 35, 27, 19, 11, 3},
        {61, 53, 45, 37, 29, 21, 13, 5},
        {63, 55, 47, 39, 31, 23, 15, 7}};
//逆IP置换
char C_IP[8][8] =
    {
        {40, 8, 48, 16, 56, 24, 64, 32},
        {39, 7, 47, 15, 55, 23, 63, 31},
        {38, 6, 46, 14, 54, 22, 62, 30},
        {37, 5, 45, 13, 53, 21, 61, 29},
        {36, 4, 44, 12, 52, 20, 60, 28},
        {35, 3, 43, 11, 51, 19, 59, 27},
        {34, 2, 42, 10, 50, 18, 58, 26},
        {33, 1, 41, 9, 49, 17, 57, 25}};
//扩展置换E
char E[8][6] =
    {
        {32, 1, 2, 3, 4, 5},
        {4, 5, 6, 7, 8, 9},
        {8, 9, 10, 11, 12, 13},
        {12, 13, 14, 15, 16, 17},
        {16, 17, 18, 19, 20, 21},
        {20, 21, 22, 23, 24, 25},
        {24, 25, 26, 27, 28, 29},
        {28, 29, 30, 31, 32, 1}};
//扩展置换P
char P[4][8] =
    {
        {16, 7, 20, 21, 29, 12, 28, 17},
        {1, 15, 23, 26, 5, 18, 31, 10},
        {2, 8, 24, 14, 32, 27, 3, 9},
        {19, 13, 30, 6, 22, 11, 4, 25}};
//S盒
char S_Box[8][65] =
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
         0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
         4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
         15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
         3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
         0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
         13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
         13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
         13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
         1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
         13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
         10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
         3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
         14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
         4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
         11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
         10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
         9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
         4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
         13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
         1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
         6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
         1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
         7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
         2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};
//选择置换PC-1
char PC_1[8][7] =
    {
        {57, 49, 41, 33, 25, 17, 9},
        {1, 58, 50, 42, 34, 26, 18},
        {10, 2, 59, 51, 43, 35, 27},
        {19, 11, 3, 60, 52, 44, 36},
        {63, 55, 47, 39, 31, 23, 15},
        {7, 62, 54, 46, 38, 30, 22},
        {14, 6, 61, 53, 45, 37, 29},
        {21, 13, 5, 28, 20, 12, 4}};
//选择置换PC-2
char PC_2[8][6] =
    {
        {14, 17, 11, 24, 1, 5},
        {3, 28, 15, 6, 21, 10},
        {23, 19, 12, 4, 26, 8},
        {16, 7, 27, 20, 13, 2},
        {41, 52, 31, 37, 47, 55},
        {30, 40, 51, 45, 33, 48},
        {44, 49, 39, 56, 34, 53},
        {46, 42, 50, 36, 29, 32}};
//左移位数
int move_time[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

/* 检测长度 */
void checkLen(uint8_t *input, uint8_t *key)
{
    int iLen, kLen;
    iLen = strlen(input);
    kLen = strlen(key);
    if (iLen % 8 != 0)
    {
        printf("输入长度应该为64bit的倍数");
        exit(0);
    }
    if (kLen != 8)
    {
        printf("Key的长度为64bit");
        exit(0);
    }
}
/* 置换 */
void substitution(char *tmpk, char *keyBit, char *PC, int len)
{
    // for (int i = 0; i < 8; i++)
    // {
    //     for (int j = 0; j < len / 8; j++)
    //     {
    //         tmpk[i * 8 + j] = keyBit[PC[i][j]];
    //     }
    // }
    for (int i = 0; i < len; i++)
    {
        tmpk[i] = keyBit[*(PC + i) - 1];
    }
    tmpk[len] = '\0';
}

void bitXor(char *L, char *R, int len)
{
    for (int i = 0; i < len; i++)
    {
        R[i] == L[i] ? (R[i] = '0') : (R[i] = '1');
    }
}

/* 8bit -> 64bit*/
uint64_t extendBit(uint8_t *key)
{
    uint64_t k;
    for (int i = 0; i < 8; i++)
    {
        k = k << 8 * i + key[i];
    }
    printf("%ld\n", k);
}
/* 字节转换为位 8位*/
void byte2bit(uint8_t *key, char *keyBit, int len)
{
    for (int i = 0; i < len / 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            keyBit[i * 8 + j] = ((key[i] >> (7 - j)) & 1) + 48;
        }
    }
}
/* 字节转换为位 4位 */
void byte2bit4(uint8_t *key, char *keyBit, int len)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            keyBit[i * 4 + j] = ((key[i] >> (3 - j)) & 1) + 48;
        }
    }
}
/* 循环左移 */
void rol(char *C, char *tmp, int len, int s)
{
    for (int i = 0; i < len; i++)
    {
        tmp[i] = C[(i + s) % len];
    }
}
/* 生成子密钥 */
void setSubKey(uint8_t *key, char subKey[16][48])
{
    char tmpKey[56], C[56], D[28], E[28], T[56] = {0};
    char keyBit[64];
    byte2bit(key, keyBit, 64);
    substitution(tmpKey, keyBit, *PC_1, 56);
    strcpy(C, tmpKey);      //C0
    strcpy(D, tmpKey + 28); //D0
    for (int i = 0; i < 16; i++)
    {
        rol(C, E, 28, move_time[i]);
        strncpy(C, E, 28);
        rol(D, E, 28, move_time[i]);
        strncpy(D, E, 28);
        strncpy(T, C, 28);
        strncpy(T + 28, D, 28);
        substitution(subKey[i], T, *PC_2, 48);
    }
}
/* 置换S盒 */
void substitute_S(char *S, char *R, char *tmp)
{
    int row, column, j = 0;
    for (int i = 0; i < 48; i += 6)
    {
        row = ((R[i] - 48) << 1) + (R[i + 5] - 48);
        column = ((R[i + 1] - 48) << 3) + ((R[i + 2] - 48) << 2) + ((R[i + 3] - 48) << 1) + (R[i + 4] - 48);
        tmp[j++] = *(S + (j * 65) + row * 16 + column);
    }
    tmp[9] = '\0';
}

/* f加密函数 */
void f_encrypt(char *R, char *subKey)
{
    char extendR[48], tmpBit[32] = {0};
    char tmp[9] = {0};
    /* E置换 */
    substitution(extendR, R, *E, 48);
    /* K[i]和R异或 */
    bitXor(subKey, extendR, 48);
    /* 置换S盒 */
    substitute_S(*S_Box, extendR, tmp);
    /* byte TO 4bit*/
    byte2bit4(tmp, tmpBit, 32);
    /* 置换P盒 */
    substitution(R, tmpBit, *P, 32);
}

int main()
{
    char input[100], key[100], dataBit[65], subKey[16][48], tmp[65], L[33], R[33], R_tmp[33] = {0};
    uint8_t result[100] = {0};
    printf("请输入明文:\n");
    scanf("%s", input);
    printf("请输入key:\n");
    scanf("%s", key);

    //检测长度
    checkLen(input, key);
    //生成密钥key
    setSubKey(key, subKey);
    for (int j = 0; j < strlen(input); j += 8)
    {
        //初始置换IP
        byte2bit(input + j, tmp, 64);
        substitution(dataBit, tmp, *IP, 64);
        //分成两部分
        strncpy(L, dataBit, 32);
        strncpy(R, dataBit + 32, 32);
        //16轮加密变换
        for (int i = 0; i < 16; i++)
        {
            strncpy(R_tmp, R, 32);
            f_encrypt(R, subKey[i]);
            bitXor(L, R, 32);
            strncpy(L, R_tmp, 32);
        }
        strncpy(tmp, R, 32);
        strncpy(tmp + 32, L, 32);
        substitution(dataBit, tmp, *C_IP, 64);
        //bit TO hex
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                result[x + j] ^= ((dataBit[x * 8 + y] - 48) << (7 - y));
            }
        }
        for (int i = 0; i < 8; i++)
        {
            printf("%02x ", result[i + j]);
        }
    }
    return 0;
}