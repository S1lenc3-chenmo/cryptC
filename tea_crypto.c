#include<stdio.h>

void DecryptTEA(unsigned int firstChunk, unsigned int secondChunk, unsigned int* key)
{
    unsigned int y = firstChunk;
    unsigned int z = secondChunk;
    unsigned int sum = 0;

    unsigned int delta = 0x9e3779b9;

    sum = delta << 5; //32轮运算，所以是2的5次方；16轮运算，所以是2的4次方；8轮运算，所以是2的3次方
 
    for (int i = 0; i < 32; i++) //8轮运算
    {
        z -= (y << 4) + key[2] ^ y + sum ^ (y >> 5) + key[3];
        y -= (z << 4) + key[0] ^ z + sum ^ (z >> 5) + key[1];
        sum -= delta;
    }

    firstChunk = y;
    secondChunk = z;
    printf("%x\n", firstChunk);
    printf("%x\n", secondChunk);
}

void EncryptTEA(unsigned int firstChunk, unsigned int secondChunk, unsigned int* key)
{
    unsigned int y = firstChunk;
    unsigned int z = secondChunk;
    unsigned int sum = 0;

    unsigned int delta = 0x9e3779b9;

    for (int i = 0; i < 32; i++)//8轮运算(需要对应下面的解密核心函数的轮数一样)
    {
        sum += delta;
        y += ((z << 4) + key[0]) ^ (z + sum) ^ ((z >> 5) + key[1]);
        z += ((y << 4) + key[2]) ^ (y + sum) ^ ((y >> 5) + key[3]);
    }

    firstChunk = y;
    secondChunk = z;
    printf("%x\n", firstChunk);
    printf("%x\n", secondChunk);
}

int main(){
    unsigned int key1[4] = {0x53b8ec81, 0xc8861dc1, 0xbba011d6, 0x3e77143b};
    unsigned int key2[4] = {0xb0f93946, 0xbeac3890, 0xf44a72f5, 0x2024fed5};
    //unsigned int first = 0xf949cc4b;
    //unsigned int second = 0xeba67fd9;
//    unsigned int first = 0xee62d1ed;
//    unsigned int second = 0x58e71477;
    unsigned int first = 0x666c6167;
    unsigned int second = 0x7b313233;


    EncryptTEA(first, second, key2);
    //DecryptTEA(first, second, key1);
    return 0;
}