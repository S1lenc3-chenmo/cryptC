#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * S盒
 */
static const int S[16][16] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};
/**
 * 常量轮值表
 */
static const int Rcon[10] = {
    0x01000000, 0x02000000,
    0x04000000, 0x08000000,
    0x10000000, 0x20000000,
    0x40000000, 0x80000000,
    0x1b000000, 0x36000000};

/**
 * 列混合矩阵
 */
static const int colM[4][4] = {2, 3, 1, 1,
                               1, 2, 3, 1,
                               1, 1, 2, 3,
                               3, 1, 1, 2};
/* 有限域乘2 */
int gfMul2(int s)
{
    int result = s << 1;
    int flag = result & 0x100;
    if (flag != 0)
    {
        return ((result & 0xff) ^ 0x1b);
    }
    return result;
}
/* 有限域乘3 */
int gfMul3(int s)
{
    return gfMul2(s) ^ s;
}
/* 列混合 */
void mixColumn(uint8_t *p)
{
    uint8_t pTmp[4][4];
    uint8_t pTmp2[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pTmp[j][i] = p[4 * i + j];
        }
    }
    for (int x = 0; x < 4; x++)
    {
        pTmp2[0][x] = gfMul2(pTmp[0][x]) ^ gfMul3(pTmp[1][x]) ^ pTmp[2][x] ^ pTmp[3][x];
        pTmp2[1][x] = pTmp[0][x] ^ gfMul2(pTmp[1][x]) ^ gfMul3(pTmp[2][x]) ^ pTmp[3][x];
        pTmp2[2][x] = pTmp[0][x] ^ pTmp[1][x] ^ gfMul2(pTmp[2][x]) ^ gfMul3(pTmp[3][x]);
        pTmp2[3][x] = gfMul3(pTmp[0][x]) ^ pTmp[1][x] ^ pTmp[2][x] ^ gfMul2(pTmp[3][x]);
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            p[4 * i + j] = pTmp2[j][i];
        }
    }
}
/* 行移位 */
void lineShift(uint8_t *p)
{
    uint8_t pTmp[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pTmp[j][i] = p[4 * i + j];
        }
    }
    uint8_t tmp;
    tmp = pTmp[1][0];
    pTmp[1][0] = pTmp[1][1];
    pTmp[1][1] = pTmp[1][2];
    pTmp[1][2] = pTmp[1][3];
    pTmp[1][3] = tmp;

    uint8_t tmp1;
    tmp = pTmp[2][0];
    tmp1 = pTmp[2][1];
    pTmp[2][0] = pTmp[2][2];
    pTmp[2][1] = pTmp[2][3];
    pTmp[2][2] = tmp;
    pTmp[2][3] = tmp1;

    tmp = pTmp[3][3];
    pTmp[3][3] = pTmp[3][2];
    pTmp[3][2] = pTmp[3][1];
    pTmp[3][1] = pTmp[3][0];
    pTmp[3][0] = tmp;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            p[4 * i + j] = pTmp[j][i];
        }
    }
}
//iscc_lockk
void lineShift2(uint8_t *p)
{
    uint8_t pTmp[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pTmp[j][i] = p[4 * i + j];
        }
    }
    uint8_t tmp;
    tmp = pTmp[3][0];
    pTmp[3][0] = pTmp[3][1];
    pTmp[3][1] = pTmp[3][2];
    pTmp[3][2] = pTmp[3][3];
    pTmp[3][3] = tmp;

    uint8_t tmp1;
    tmp = pTmp[2][0];
    tmp1 = pTmp[2][1];
    pTmp[2][0] = pTmp[2][2];
    pTmp[2][1] = pTmp[2][3];
    pTmp[2][2] = tmp;
    pTmp[2][3] = tmp1;

    tmp = pTmp[1][3];
    pTmp[1][3] = pTmp[1][2];
    pTmp[1][2] = pTmp[1][1];
    pTmp[1][1] = pTmp[1][0];
    pTmp[1][0] = tmp;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            p[4 * i + j] = pTmp[j][i];
        }
    }
}
/* 轮密钥加 */
void addRoundKeys(uint8_t *p, uint8_t W[44][4], int j)
{
    int x = 0;
    for (int i = j * 4; i < j * 4 + 4; i++)
    {
        for (int y = 0; y < 4; y++)
        {
            p[x] ^= W[i][y];
            x++;
        }
    }
}
/* 字节代换 */
char subBytes(uint8_t byte)
{
    int high, low;
    high = (byte >> 4) & 0x0f;
    low = byte & 0x0f;
    return S[high][low];
}
/* 字循环 */
void wordCycle(char *word1, char *word2)
{
    word2[0] = word1[1];
    word2[1] = word1[2];
    word2[2] = word1[3];
    word2[3] = word1[0];
}
/* 右字循环 */
void rWordCycle(char *word1, char *word2)
{
    word2[0] = word1[3];
    word2[1] = word1[0];
    word2[2] = word1[1];
    word2[3] = word1[2];
}
/* 扩展Key*/
void extendKey(uint8_t *key, uint8_t W[44][4])
{
    //10轮加密
    // 初始key
    for (int t = 0; t < 16; t++)
    {
        W[t / 4][t % 4] = key[t] & 0xff;
    }
    //扩展
    for (int i = 4; i < 44; i++)
    {
        if (i % 4 == 0)
        {
            //W[i] = W[i-4] ^ T(W[i-1])
            //1.字循环
            wordCycle(W[i - 1], W[i]);
            //2.字节代换 + 轮常量异或
            for (int y = 0; y < 4; y++)
            {
                //(Rcon[i / 4 - 1]) >> ((3 - y) * 8) & 0xff
                W[i][y] = subBytes(W[i][y]) ^ ((Rcon[i / 4 - 1]) >> ((3 - y) * 8) & 0xff) ^ W[i - 4][y];
            }
        }
        else
        {
            //W[i] = W[i-4] ^ W[i-1]
            for (int y = 0; y < 4; y++)
            {
                W[i][y] = W[i - 1][y] ^ W[i - 4][y];
            }
        }
    }
}
//iscc_2021_lockk
void extendKey2(uint8_t *key, uint8_t W[44][4])
{
    //10轮加密
    // 初始key
    for (int t = 0; t < 16; t++)
    {
        W[t / 4][t % 4] = key[t] & 0xff;
    }
    //扩展
    for (int i = 4; i < 44; i++)
    {
        if (i % 4 == 0)
        {
            //W[i] = W[i-4] ^ T(W[i-1])
            //1.字循环
            wordCycle(W[i - 1], W[i]);
            //2.字节代换 + 轮常量异或
            for (int y = 0; y < 4; y++)
            {
                //(Rcon[i / 4 - 1]) >> ((3 - y) * 8) & 0xff
                if (y == 0)
                {
                    W[i][y] = subBytes(W[i - 1][y]) ^ ((Rcon[i / 4 - 1]) >> (24) & 0xff) ^ W[i - 4][y];
                }
                else if (y == 3)
                {
                    W[i][y] = subBytes(W[i - 1][1]) ^ W[i - 4][y];
                }
                else
                {
                    W[i][y] = subBytes(W[i - 1][y]) ^ W[i - 4][y];
                }
            }
        }
        else
        {
            //W[i] = W[i-4] ^ W[i-1]
            for (int y = 0; y < 4; y++)
            {
                W[i][y] = W[i - 1][y] ^ W[i - 4][y];
            }
        }
    }
}
/*
    p: 明文
    plen: 明文长度
    key: 密钥
*/
void aes_encrypt(uint8_t *p, int plen, char *key)
{
    int keyLen = strlen(key);
    /* 检测长度 */
    if (plen == 0 || plen % 16 != 0)
    {
        printf("字符长度位16的倍数！");
    }
    if (keyLen == 0 || keyLen != 16)
    {
        printf("AES-128bit密钥长度为16");
    }
    /* 扩展Key*/
    uint8_t W[44][4] = {0};
    extendKey(key, W);
    for (int x = 0; x < 44; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            printf("%x ", W[x][y]);
        }
        printf("\n");
    }
    for (int q = 0; q < plen; q += 16)
    {
        p = p + q;
        /* 前9轮 */
        int j = 0;
        /* 轮密钥加 */
        addRoundKeys(p, W, j);
        for (j = 1; j < 10; j++)
        {
            /* 字节代换 */
            for (int y = 0; y < 16; y++)
            {
                p[y] = subBytes(p[y]);
            }
            for (int u = 0; u < 16; u++)
            {
                printf("%02x", (uint8_t)p[u]);
            }
            printf("\n");
            /* 行移位 */
            lineShift(p);
            for (int u = 0; u < 16; u++)
            {
                printf("%02x", (uint8_t)p[u]);
            }
            printf("\n");
            /* 列混合 */
            mixColumn(p);
            /* 轮密钥加 */
            addRoundKeys(p, W, j);
        }
        /* 字节代换 */
        for (int y = 0; y < 16; y++)
        {
            p[y] = subBytes(p[y]);
        }
        /* 行移位 */
        lineShift(p);
        /* 轮密钥加 */
        addRoundKeys(p, W, j);
    }
}

/*
    main函数
    接收用户输入
*/
int main()
{
    char key[100] = {0};
    char pdata[100] = {0};
    int plen = 0;
    //scanf("%s", pdata);
    memcpy(pdata, "\x12\x34\x56\x78\x91\x23\x45\x67\x89\x12\x34\x56\x78\x94\x12\x57", 16);
    memcpy(key, "\x2b\x7e\x15\x16\x28\xae\xd2\xa6\xab\xf7\x15\x88\x09\xcf\x4f\x3c", 16);
    plen = strlen(pdata);
    printf("%s, %s, %d, %ld\n", pdata, key, plen, strlen(key));
    aes_encrypt(pdata, plen, key);
    printf("加密后的结果: ");
    for (int i = 0; i < plen; i++)
    {
        printf("%02x", (uint8_t)pdata[i]);
    }
    printf("\n");
    return 0;
}