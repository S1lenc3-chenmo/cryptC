#include <stdio.h>
#include <stdint.h>
 
/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */
 
void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x12345678;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}
 
void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], delta=0x12345678, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}
 
int main()
{
    uint32_t v1[2]={0x0EC311F0, 0x45C79AF3};
    uint32_t v2[2]={0xEDF5D910, 0x542702CB};
    uint32_t v[2]={0x9653b9be, 0x13c64a75};//verify
    uint32_t v0[2] = {0x8a93b725, 0x5cd48daa};//verify
    unsigned int key1[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
    unsigned int key2[4] = {0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c};
    unsigned int r=32;//num_rounds建议取值为32
    // v为要加密的数据是两个32位无符号整数
    // k为加密解密密钥，为4个32位无符号整数，即密钥长度为128位
    // printf("加密前原始数据：%x %x\n",v[0],v[1]);
    encipher(r, v, key1);
    printf("加密后的数据：%x %x\n",v[0],v[1]);
    encipher(r, v0, key1);
    printf("加密后的数据：%x %x\n",v0[0],v0[1]);
    decipher(r, v1, key1);
    printf("解密后的数据：%x %x ",v1[0],v1[1]);
    decipher(r, v2, key1);
    printf("%x %x\n",v2[0],v2[1]);
    return 0;
}