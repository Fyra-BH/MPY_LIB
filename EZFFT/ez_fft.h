#ifndef __EZ_FFT__H
#define __EZ_FFT__H

void fft_8(float* buff);
void fft_16(float* buff);
void fft_32(float* buff);

typedef struct complex
{
    float re;
    float im;
}complex_t;

#endif
