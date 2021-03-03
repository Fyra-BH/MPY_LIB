#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ez_fft.h"

void fft_32(float* buff)
{
    complex_t* buff_c = (complex_t*)malloc(sizeof(complex_t) * 32);
    //将原序列的序号比特翻转，并存入临时数组
    for (size_t i = 0; i < 32; i++)
    {
        buff_c[i].re = *(buff + 2 * bit_reverse(i, 5));
        buff_c[i].im = *(buff + 2 * bit_reverse(i, 5) + 1);
    }
    complex_t w0 = W(32, 0);
    complex_t w1 = W(32, 1);
    complex_t w2 = W(32, 2);
    complex_t w3 = W(32, 3);
    complex_t w4 = W(32, 4);
    complex_t w5 = W(32, 5);
    complex_t w6 = W(32, 6);
    complex_t w7 = W(32, 7);
    complex_t w8 = W(32, 8);
    complex_t w9 = W(32, 9);
    complex_t w10 = W(32, 10);
    complex_t w11 = W(32, 11);
    complex_t w12 = W(32, 12);
    complex_t w13 = W(32, 13);
    complex_t w14 = W(32, 14);
    complex_t w15 = W(32, 15);
//第1轮运算
   butterfly_compu(&buff_c[0], &buff_c[1], w0);
   butterfly_compu(&buff_c[2], &buff_c[3], w0);
   butterfly_compu(&buff_c[4], &buff_c[5], w0);
   butterfly_compu(&buff_c[6], &buff_c[7], w0);
   butterfly_compu(&buff_c[8], &buff_c[9], w0);
   butterfly_compu(&buff_c[10], &buff_c[11], w0);
   butterfly_compu(&buff_c[12], &buff_c[13], w0);
   butterfly_compu(&buff_c[14], &buff_c[15], w0);
   butterfly_compu(&buff_c[16], &buff_c[17], w0);
   butterfly_compu(&buff_c[18], &buff_c[19], w0);
   butterfly_compu(&buff_c[20], &buff_c[21], w0);
   butterfly_compu(&buff_c[22], &buff_c[23], w0);
   butterfly_compu(&buff_c[24], &buff_c[25], w0);
   butterfly_compu(&buff_c[26], &buff_c[27], w0);
   butterfly_compu(&buff_c[28], &buff_c[29], w0);
   butterfly_compu(&buff_c[30], &buff_c[31], w0);
//第2轮运算
   butterfly_compu(&buff_c[0], &buff_c[2], w0);
   butterfly_compu(&buff_c[1], &buff_c[3], w8);
   butterfly_compu(&buff_c[4], &buff_c[6], w0);
   butterfly_compu(&buff_c[5], &buff_c[7], w8);
   butterfly_compu(&buff_c[8], &buff_c[10], w0);
   butterfly_compu(&buff_c[9], &buff_c[11], w8);
   butterfly_compu(&buff_c[12], &buff_c[14], w0);
   butterfly_compu(&buff_c[13], &buff_c[15], w8);
   butterfly_compu(&buff_c[16], &buff_c[18], w0);
   butterfly_compu(&buff_c[17], &buff_c[19], w8);
   butterfly_compu(&buff_c[20], &buff_c[22], w0);
   butterfly_compu(&buff_c[21], &buff_c[23], w8);
   butterfly_compu(&buff_c[24], &buff_c[26], w0);
   butterfly_compu(&buff_c[25], &buff_c[27], w8);
   butterfly_compu(&buff_c[28], &buff_c[30], w0);
   butterfly_compu(&buff_c[29], &buff_c[31], w8);
//第3轮运算
   butterfly_compu(&buff_c[0], &buff_c[4], w0);
   butterfly_compu(&buff_c[1], &buff_c[5], w4);
   butterfly_compu(&buff_c[2], &buff_c[6], w8);
   butterfly_compu(&buff_c[3], &buff_c[7], w12);
   butterfly_compu(&buff_c[8], &buff_c[12], w0);
   butterfly_compu(&buff_c[9], &buff_c[13], w4);
   butterfly_compu(&buff_c[10], &buff_c[14], w8);
   butterfly_compu(&buff_c[11], &buff_c[15], w12);
   butterfly_compu(&buff_c[16], &buff_c[20], w0);
   butterfly_compu(&buff_c[17], &buff_c[21], w4);
   butterfly_compu(&buff_c[18], &buff_c[22], w8);
   butterfly_compu(&buff_c[19], &buff_c[23], w12);
   butterfly_compu(&buff_c[24], &buff_c[28], w0);
   butterfly_compu(&buff_c[25], &buff_c[29], w4);
   butterfly_compu(&buff_c[26], &buff_c[30], w8);
   butterfly_compu(&buff_c[27], &buff_c[31], w12);
//第4轮运算
   butterfly_compu(&buff_c[0], &buff_c[8], w0);
   butterfly_compu(&buff_c[1], &buff_c[9], w2);
   butterfly_compu(&buff_c[2], &buff_c[10], w4);
   butterfly_compu(&buff_c[3], &buff_c[11], w6);
   butterfly_compu(&buff_c[4], &buff_c[12], w8);
   butterfly_compu(&buff_c[5], &buff_c[13], w10);
   butterfly_compu(&buff_c[6], &buff_c[14], w12);
   butterfly_compu(&buff_c[7], &buff_c[15], w14);
   butterfly_compu(&buff_c[16], &buff_c[24], w0);
   butterfly_compu(&buff_c[17], &buff_c[25], w2);
   butterfly_compu(&buff_c[18], &buff_c[26], w4);
   butterfly_compu(&buff_c[19], &buff_c[27], w6);
   butterfly_compu(&buff_c[20], &buff_c[28], w8);
   butterfly_compu(&buff_c[21], &buff_c[29], w10);
   butterfly_compu(&buff_c[22], &buff_c[30], w12);
   butterfly_compu(&buff_c[23], &buff_c[31], w14);
//第5轮运算
   butterfly_compu(&buff_c[0], &buff_c[16], w0);
   butterfly_compu(&buff_c[1], &buff_c[17], w1);
   butterfly_compu(&buff_c[2], &buff_c[18], w2);
   butterfly_compu(&buff_c[3], &buff_c[19], w3);
   butterfly_compu(&buff_c[4], &buff_c[20], w4);
   butterfly_compu(&buff_c[5], &buff_c[21], w5);
   butterfly_compu(&buff_c[6], &buff_c[22], w6);
   butterfly_compu(&buff_c[7], &buff_c[23], w7);
   butterfly_compu(&buff_c[8], &buff_c[24], w8);
   butterfly_compu(&buff_c[9], &buff_c[25], w9);
   butterfly_compu(&buff_c[10], &buff_c[26], w10);
   butterfly_compu(&buff_c[11], &buff_c[27], w11);
   butterfly_compu(&buff_c[12], &buff_c[28], w12);
   butterfly_compu(&buff_c[13], &buff_c[29], w13);
   butterfly_compu(&buff_c[14], &buff_c[30], w14);
   butterfly_compu(&buff_c[15], &buff_c[31], w15);
    for (size_t i = 0; i < 32; i++)
    {
        *(buff + i * 2) = buff_c[i].re;
        *(buff + i * 2 + 1) = buff_c[i].im;
    }
    free(buff_c);
}
