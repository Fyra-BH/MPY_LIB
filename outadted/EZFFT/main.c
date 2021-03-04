#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ez_fft.h"

#define PI (3.1415926535)

void  fft_test(int N)
{
    int flag = 1;
    int N_tab[8] = {8, 16, 32, 64, 128, 256, 512, 1024};

    for (size_t i = 0; i < 8; i++)
    {
        if (N == N_tab[i])
        {
            flag = 0;
            break;
        }
    }
    if (flag) 
    {
        printf("不支持的长度\n");
        return;
    } 
    
    float *x = (float*)malloc(sizeof(float) * N * 2);
    float fs = 8000.0;//采样率
    float f0 = 440;//待测频率
    for (int i = 0; i < N; i++)
    {
        x[i * 2] =sin(2 * PI * f0 / fs * i);
        x[i * 2 + 1] = 0;
    }
    printf("开始fft计算\n");
    if(N == 8) fft_8(x);
    else if(N == 16) fft_16(x);
    else if(N == 32) fft_32(x);
    else if(N == 64) fft_64(x);
    else if(N == 128) fft_128(x);
    else if(N == 256) fft_256(x);
    else if(N == 512) fft_512(x);
    else if(N == 1024) fft_1024(x);
    printf("计算完成\n");
    ezfft_abs(x, N);
    // for (int i = 0; i < N; i++)
    //     printf("x[%d] = %0.4f\n", i, x[i]);
    
    float max = 0;
    int max_index = 0;
    for (int i = 0; i < N / 2; i++)
    {
        if(max < x[i])
        {
            max = x[i];
            max_index = i;
        }
    }
    printf("max = %0.4f, index = %d, so f0 = %0.4f\n", max, max_index, max_index / (float)N * fs);
}


int main(int argc, char const *argv[])
{ 
    float a = 0.8;
    int N = 0;
    while (1)
    {
        printf("请输入fft的长度N, 输入0退出:\n");
        scanf("%d", &N);
        if(N == 0) break;
        fft_test(N);
    }
    return 0;
}
