#include <stdio.h>
#include <string.h>
#include <stdint.h>

char BASE_TABLE[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

//byte to bit
void byteToBit(char *input, char *inputBit, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            inputBit[i * 6 + j] = ((input[i] >> (5 - j)) & 1) + '0';
        }
    }
}
//bit to byte
void bitToByte(char *inputBit, char *input, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            input[i] += ((inputBit[i * 8 + j] - '0') << (7 - j));
        }
    }
}
//base64解密
int base64(char *input, char *output, int len)
{
    char inputBit[1000] = {0};
    //byte 转换为 bit
    byteToBit(input, inputBit, len);
    //每8bit 转换为 byte
    bitToByte(inputBit, output, len);
    return strlen(input);
}
int main()
{
    char input[100] = {0}, output[100] = {0};
    int len;

    printf("请输入明文:\n");
    scanf("%s", input);
    len = strlen(input);

    if (input[len - 1] == 61)
    {
        if (input[len - 2] == 61)
        {
            len -= 2;
        }
        else
        {
            len -= 1;
        }
    }
    //base64解密
    int bitLen = base64(input, output, len);
    for (int i = 0; i < bitLen; i++)
    {
        printf("%s", index(BASE_TABLE, output[i]));
    }

    return 0;
}
