#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* 初始化向量S */
void initS(uint8_t *S)
{
    for (int i = 0; i < 256; i++)
    {
        S[i] = i;
    }
}

/* 初始化Key */
void initKey(uint8_t *T, uint8_t *k)
{
    int keyLen = strlen(k);
    if (keyLen < 256)
    {
        for (int i = 0; i < 256; i++)
        {
            T[i] = k[i % keyLen];
        }
    }
    else
    {
        for (int i = 0; i < 256; i++)
        {
            T[i] = k[i];
        }
    }
}

/* 置换S */
void swapS(uint8_t *S, uint8_t *T)
{
    int j = 0;
    uint8_t tmp;
    for (int i = 0; i < 256; i++)
    {
        j = (j + S[i] + T[i]) % 256;
        tmp = S[i];
        S[i] = S[j];
        S[j] = tmp;
    }
}

/* 生成密钥流 */
void generateKeyStream(uint8_t *S, uint8_t *k, int dataLen)
{
    int i = 0, j = 0, t;
    uint8_t tmp;
    for (int r = 0; r < dataLen; r++)
    {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        tmp = S[i];
        S[i] = S[j];
        S[j] = tmp;
        t = (S[i] + S[j]) % 256;
        k[r] = S[t];
    }
}
/* k与input异或 */
void K_xor_Input(uint8_t *k, uint8_t *input, int dataLen)
{
    for (int i = 0; i < dataLen; i++)
    {
        input[i] ^= k[i];
    }
}
int main()
{
    char input[100] = {0}, key[257] = {0}, S[257] = {0}, T[257] = {0}, k[257] = {0};

    printf("Please input data:\n");
    scanf("%s", input);
    //memcpy(input, "\xf9\xda\xe8\x9f\x38\x04\x1c\x7a\x7e", 9);
    printf("Please input key:\n");
    scanf("%s", key);
    //初始化状态向量S
    initS(S);
    //初始化Key位临时向量T
    initKey(T, key);
    //置换S，打乱顺序
    swapS(S, T);
    //生成密钥流
    generateKeyStream(S, k, strlen(input));
    //异或
    K_xor_Input(k, input, strlen(input));
    printf("RC4加密结果:\n");
    for (int i = 0; i < strlen(input); i++)
    {
        printf("%02x ", (uint8_t)input[i]);
    }
    printf("\n");
    K_xor_Input(k, input, strlen(input));
    printf("RC4解密结果:\n");
    for (int i = 0; i < strlen(input); i++)
    {
        printf("%02x ", (uint8_t)input[i]);
    }
    printf("\n");
    return 0;
}