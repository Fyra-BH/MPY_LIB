#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ez_fft.h"

void fft_8(float* buff)
{
    complex_t* buff_c = (complex_t*)malloc(sizeof(complex_t) * 8);
    //将原序列的序号比特翻转，并存入临时数组
    for (size_t i = 0; i < 8; i++)
    {
        buff_c[i].re = *(buff + 2 * bit_reverse(i, 3));
        buff_c[i].im = *(buff + 2 * bit_reverse(i, 3) + 1);
    }
    complex_t w0 = W(8, 0);
    complex_t w1 = W(8, 1);
    complex_t w2 = W(8, 2);
    complex_t w3 = W(8, 3);
//第1轮运算
   butterfly_compu(&buff_c[0], &buff_c[1], w0);
   butterfly_compu(&buff_c[2], &buff_c[3], w0);
   butterfly_compu(&buff_c[4], &buff_c[5], w0);
   butterfly_compu(&buff_c[6], &buff_c[7], w0);
//第2轮运算
   butterfly_compu(&buff_c[0], &buff_c[2], w0);
   butterfly_compu(&buff_c[1], &buff_c[3], w2);
   butterfly_compu(&buff_c[4], &buff_c[6], w0);
   butterfly_compu(&buff_c[5], &buff_c[7], w2);
//第3轮运算
   butterfly_compu(&buff_c[0], &buff_c[4], w0);
   butterfly_compu(&buff_c[1], &buff_c[5], w1);
   butterfly_compu(&buff_c[2], &buff_c[6], w2);
   butterfly_compu(&buff_c[3], &buff_c[7], w3);
    for (size_t i = 0; i < 8; i++)
    {
        *(buff + i * 2) = buff_c[i].re;
        *(buff + i * 2 + 1) = buff_c[i].im;
    }
    free(buff_c);
}
