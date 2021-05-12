#include "ezfft.h"

void  fft_test(int N)
{
    if (!ezlog2(N)) 
    {
        printf("不支持的长度\n");
        return;
    } 
    
    float *x = (float*)ezfft_malloc(sizeof(float) * N * 2);
    float fs = 8000.0;//采样率
    float f0 = 440;//待测频率
    for (int i = 0; i < N; i++)
    {
        x[i * 2] =sin(2 * PI * f0 / fs * i);
        x[i * 2 + 1] = 0;
    }
    printf("开始fft计算\n");
    fft_N(N, x);
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
    int N = 16;
    float *buff = (float *)malloc(N * 2 * sizeof(float));
    for (int i = 0; i < N; i++)
    {
        *(buff + i * 2) = powf(a, i);
        *(buff + i * 2 + 1) = 0;
    }
    ifft_N(N, buff);
    for (int i = 0; i < N; i++)
    {
        printf("(%f, %f)\n",*(buff + i * 2), *(buff + i * 2 + 1));
    }
    
    // while (1)
    // {
    //     printf("请输入fft的长度N, 输入0退出:\n");
    //     scanf("%d", &N);
    //     if(N == 0) break;
    //     fft_test(N);
    // }

    return 0;
}
