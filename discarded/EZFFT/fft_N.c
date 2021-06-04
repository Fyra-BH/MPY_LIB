#include "ez_fft.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI (3.1415926535)
/**
 * @brief 求N对2的对数
 * 
 * @param N 2的整次幂
 * @return int 当N不是2的整次幂时返回0
 */
static int ezlog2(int N)
{
    int ret = 0;
    while (!(N&0x01))
    {
        N >>= 1;
        ret ++;
    }
    int bitsum = 0;
    for (int i = 0; i < sizeof(N) * 8; i++)
        if(N & (1 << i)) bitsum ++;
    if(bitsum != 1) ret = 0;
    return ret;
}

/**
 * @brief N点fft，注意N为2的整次幂
 * 
 * @param N fft点数
 * @param buff 缓冲区，实部虚部交错
 */
void fft_N(int N, float* buff)
{
    if(ezlog2(N) == 0) return;
    complex_t* buff_c = (complex_t*)malloc(sizeof(complex_t) * N);
    //将原序列的序号比特翻转，并存入临时数组
    for (size_t i = 0; i < N; i++)
    {
        buff_c[i].re = *(buff + 2 * bit_reverse(i, ezlog2(N)));
        buff_c[i].im = *(buff + 2 * bit_reverse(i, ezlog2(N)) + 1);
    }
    complex_t *w = (complex_t *)malloc(sizeof(complex_t) * N / 2);
    for (int i = 0; i < N / 2; i++)
        *(w + i) = W(N, i);
    //蝶形运算
    for (int i = 0; i < ezlog2(N); i++)
    {
        for (int j = 0; j < N / (1<<(i + 1)); j++)
        {
            for (int k = 0; k < 1<<i; k++)
            {
                int x = (1<<(i + 1)) * j + k;
                int y = x + (1 << i);
                int z = (i != 0) * k * N / (1 << (i + 1));
                // printf("butterfly_compu(&buff_c[%d], &buff_c[%d], w%d);\n",x, y, z);
                butterfly_compu(&buff_c[x], &buff_c[y], w[z]);
            }
        }
    }

    for (size_t i = 0; i < N; i++)
    {
        *(buff + i * 2) = buff_c[i].re;
        *(buff + i * 2 + 1) = buff_c[i].im;
    }
    free(buff_c);
    free(w);
}

