#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ez_fft.h"

void fft_16(float* buff)
{
    complex_t* buff_c = (complex_t*)malloc(sizeof(complex_t) * 16);
    //将原序列的序号比特翻转，并存入临时数组
    for (size_t i = 0; i < 16; i++)
    {
        buff_c[i].re = *(buff + 2 * bit_reverse(i, 4));
        buff_c[i].im = *(buff + 2 * bit_reverse(i, 4) + 1);
    }
    complex_t w0 = W(16, 0);
    complex_t w1 = W(16, 1);
    complex_t w2 = W(16, 2);
    complex_t w3 = W(16, 3);
    complex_t w4 = W(16, 4);
    complex_t w5 = W(16, 5);
    complex_t w6 = W(16, 6);
    complex_t w7 = W(16, 7);
//第1轮运算
   butterfly_compu(&buff_c[0], &buff_c[1], w0);
   butterfly_compu(&buff_c[2], &buff_c[3], w0);
   butterfly_compu(&buff_c[4], &buff_c[5], w0);
   butterfly_compu(&buff_c[6], &buff_c[7], w0);
   butterfly_compu(&buff_c[8], &buff_c[9], w0);
   butterfly_compu(&buff_c[10], &buff_c[11], w0);
   butterfly_compu(&buff_c[12], &buff_c[13], w0);
   butterfly_compu(&buff_c[14], &buff_c[15], w0);
//第2轮运算
   butterfly_compu(&buff_c[0], &buff_c[2], w0);
   butterfly_compu(&buff_c[1], &buff_c[3], w4);
   butterfly_compu(&buff_c[4], &buff_c[6], w0);
   butterfly_compu(&buff_c[5], &buff_c[7], w4);
   butterfly_compu(&buff_c[8], &buff_c[10], w0);
   butterfly_compu(&buff_c[9], &buff_c[11], w4);
   butterfly_compu(&buff_c[12], &buff_c[14], w0);
   butterfly_compu(&buff_c[13], &buff_c[15], w4);
//第3轮运算
   butterfly_compu(&buff_c[0], &buff_c[4], w0);
   butterfly_compu(&buff_c[1], &buff_c[5], w2);
   butterfly_compu(&buff_c[2], &buff_c[6], w4);
   butterfly_compu(&buff_c[3], &buff_c[7], w6);
   butterfly_compu(&buff_c[8], &buff_c[12], w0);
   butterfly_compu(&buff_c[9], &buff_c[13], w2);
   butterfly_compu(&buff_c[10], &buff_c[14], w4);
   butterfly_compu(&buff_c[11], &buff_c[15], w6);
//第4轮运算
   butterfly_compu(&buff_c[0], &buff_c[8], w0);
   butterfly_compu(&buff_c[1], &buff_c[9], w1);
   butterfly_compu(&buff_c[2], &buff_c[10], w2);
   butterfly_compu(&buff_c[3], &buff_c[11], w3);
   butterfly_compu(&buff_c[4], &buff_c[12], w4);
   butterfly_compu(&buff_c[5], &buff_c[13], w5);
   butterfly_compu(&buff_c[6], &buff_c[14], w6);
   butterfly_compu(&buff_c[7], &buff_c[15], w7);
    for (size_t i = 0; i < 16; i++)
    {
        *(buff + i * 2) = buff_c[i].re;
        *(buff + i * 2 + 1) = buff_c[i].im;
    }
    free(buff_c);
}
