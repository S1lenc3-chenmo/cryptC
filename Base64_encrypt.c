#include <stdio.h>
#include <string.h>
#include <stdint.h>

char BASE_TABLE[65] = "abcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKLMNOPQRSTUVWXYZ";

//byte to bit
void byteToBit(char *input, char *inputBit, int add)
{
    int i;
    for (i = 0; i < strlen(input); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            inputBit[i * 8 + j] = ((input[i] >> (7 - j)) & 1) + '0';
        }
    }
    if (add == 2)
    {
        for (int j = 0; j < 2; j++)
        {
            inputBit[i * 8 + j] = '0';
        }
    }
    else if (add == 1)
    {
        for (int j = 0; j < 4; j++)
        {
            inputBit[i * 8 + j] = '0';
        }
    }
}
//bit to byte
void bitToByte(char *inputBit, char *input, int add)
{
    int i = 0;
    for (i = 0; i < strlen(inputBit) / 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            input[i] += ((inputBit[i * 6 + j] - '0') << (5 - j));
        }
    }
}
//base64加密
int base64(char *input, char *output, int len)
{
    char inputBit[1000] = {0};
    //byte 转换为 bit
    byteToBit(input, inputBit, len % 3);
    //每6bit 转换为 byte
    bitToByte(inputBit, output, len % 3);
    return strlen(inputBit);
}
int main()
{
    char input[100] = {0}, output[100] = {0}, result[100] = {0};
    int len;

    printf("请输入明文:\n");
    scanf("%s", input);
    len = strlen(input);

    //base64加密
    int bitLen = base64(input, output, len);
    int j = 0;
    while (len > 0)
    {
        if (len >= 3)
        {
            result[j++] = BASE_TABLE[output[j]];
            result[j++] = BASE_TABLE[output[j]];
            result[j++] = BASE_TABLE[output[j]];
            result[j++] = BASE_TABLE[output[j]];
        }
        else if (len == 2)
        {
            result[j++] = BASE_TABLE[output[j]];
            result[j++] = BASE_TABLE[output[j]];
            result[j++] = BASE_TABLE[output[j]];
            result[j] = '=';
        }
        else if (len == 1)
        {
            result[j++] = BASE_TABLE[output[j]];
            result[j++] = BASE_TABLE[output[j]];
            result[j] = '=';
            result[++j] = '=';
        }
        len -= 3;
    }
    printf("%s", result);
    // for (int i = 0; i < len / 3; i++)
    // {
    //     printf("%c", BASE_TABLE[output[i]]);
    // }
    // if(len%3==1){
    //     printf("==\n");
    // }else if(len%3==2){
    //     printf("=\n");123
    // }
    return 0;
}
