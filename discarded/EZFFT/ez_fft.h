#ifndef __EZ_FFT__H
#define __EZ_FFT__H

extern void fft_8(float* buff);
extern void fft_16(float* buff);
extern void fft_32(float* buff);
extern void fft_64(float* buff);
extern void fft_128(float* buff);
extern void fft_256(float* buff);
extern void fft_512(float* buff);
extern void fft_1024(float* buff);
extern void fft_N(int N, float* buff);

int bit_reverse(int x, int bit_len);
void ezfft_abs(float* buff, int N);

typedef struct complex
{
    float re;
    float im;
}complex_t;

complex_t W(int N, int k);

complex_t add_c(complex_t a, complex_t b);
complex_t sub_c(complex_t a, complex_t b);
complex_t multi_c(complex_t a, complex_t b);
void butterfly_compu(complex_t* a, complex_t* b, complex_t w);

#endif
