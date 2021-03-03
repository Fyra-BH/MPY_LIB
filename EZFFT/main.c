#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ez_fft.h"


int main(int argc, char const *argv[])
{
/*    
    float a = 0.8;
    int N = 8;
    float x[16];
    for (size_t i = 0; i < N; i++)
    {
        x[i * 2] =pow(a, (float)i);
        x[i * 2 + 1] = 0;
    }
    fft_8(x);
    for (size_t i = 0; i < 8; i++)
    {
        printf("%0.4f, j %0.4f\n", *(x + i * 2), *(x + i * 2 + 1));
    }
*/
/*  
    float a = 0.8;
    int N = 16;
    float x[32];
    for (size_t i = 0; i < N; i++)
    {
        x[i * 2] =pow(a, (float)i);
        x[i * 2 + 1] = 0;
    }
    fft_16(x);
    for (size_t i = 0; i < 16; i++)
    {
        printf("%0.4f, j %0.4f\n", *(x + i * 2), *(x + i * 2 + 1));
    }
*/
    float a = 0.8;
    int N = 32;
    float x[64];
    for (size_t i = 0; i < N; i++)
    {
        x[i * 2] =pow(a, (float)i);
        x[i * 2 + 1] = 0;
    }
    //测试32点fft
    fft_32(x);
    for (size_t i = 0; i < 32; i++)
    {
        printf("%0.4f, j %0.4f\n", *(x + i * 2), *(x + i * 2 + 1));
    }
    return 0;
}
