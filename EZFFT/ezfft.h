#ifndef __EZFFT__H
#define __EZFFT__H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI (3.1415926535)

typedef struct complex
{
    float re;
    float im;
}complex_t;

void butterfly_compu(complex_t* a, complex_t* b, complex_t w);
int fft_N(int N, float* buff);
int bit_reverse(int x, int bit_len);
void ezfft_abs(float* buff, int N);
int ezlog2(int N);
complex_t W(int N, int k);
complex_t add_c(complex_t a, complex_t b);
complex_t sub_c(complex_t a, complex_t b);
complex_t multi_c(complex_t a, complex_t b);

#ifdef DEBUG_ON_PC
    #define ezfft_malloc malloc
    #define ezfft_free free
#else
    #include "py/runtime.h"
    #define ezfft_malloc m_malloc
    #define ezfft_free m_free
#endif

#endif
