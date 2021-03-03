#include "ez_fft.h"
// #include "lib/libm/libm.h"
#include <stdio.h>
#include <math.h>
// #include "py/parsenum.h"

#define PI (3.1415926535)



/**
 * @brief bit reverse
 * 
 * @param x the number to be reversed
 * @param bit_len .eg: for N in 1~7, bit len = 3. (as 2^3 = 8)
 * @return int result
 */
static int bit_reverse(int x, int bit_len)
{
    int ret = 0;
    for(int i = 0; i < bit_len; i ++)
    {
        if(x & (0x1 << i))
            ret |= 0x1 << (bit_len - 1 - i);
    }
    return ret;
}

/**
 * @brief complex multiply
 * 
 * @param a 
 * @param b 
 * @return complex_t 
 */
static complex_t multi_c(complex_t a, complex_t b)
{
    complex_t ret;
    ret.re = a.re * b.re - a.im * b.im;
    ret.im = a.re * b.im + a.im * b.re;
    return ret;
}

/**
 * @brief complex add
 * 
 * @param a 
 * @param b 
 * @return complex_t 
 */
static complex_t add_c(complex_t a, complex_t b)
{
    complex_t ret;
    ret.re = a.re + b.re;
    ret.im = a.im + b.im;
    return ret;
}

/**
 * @brief complex subtraction
 * 
 * @param a 
 * @param b 
 * @return complex_t a - b
 */
complex_t sub_c(complex_t a, complex_t b)
{
    complex_t ret;
    ret.re = a.re - b.re;
    ret.im = a.im - b.im;
    return ret;
}

/**
 * @brief 旋转因子
 * 
 * @param N 下标
 * @param k 上标
 * @return complex_t 
 */
static complex_t W(int N, int k)
{
    complex_t ret;
    ret.re =  cos(2 * PI / N * k);
    ret.im = -sin(2 * PI / N * k);
    return ret;
}

/**
 * @brief 蝶形运算
 * 
 * @param a 
 * @param b 
 * @param w 旋转因子 
 */
static void butterfly_compu(complex_t* a, complex_t* b, complex_t w)
{
    complex_t tmp1, tmp2;
    tmp1 = add_c(*a, multi_c(*b, w));
    tmp2 = sub_c(*a, multi_c(*b, w));
    *a = tmp1;
    *b = tmp2;
}

/**
 * @brief 8point FFT
 * 
 * @param buff length of buff should be 16, and put the real part at even index, 
 * imaginary part at odd index  
 */
void fft_8(float* buff)
{
    complex_t tmp[8];
    for (size_t i = 0; i < 8; i++)
    {
        tmp[i].re = *(buff + 2 * i);
        tmp[i].im = *(buff + 2 * i + 1 );
    }
    complex_t buff_c[8];
    //将原序列的序号比特翻转，并存入临时数组
    for (size_t i = 0; i < 8; i++)
    {
        buff_c[i] = tmp[bit_reverse(i, 3)];
    }
    complex_t w0 = W(8, 0);
    complex_t w1 = W(8, 1);
    complex_t w2 = W(8, 2);
    complex_t w3 = W(8, 3);
    //第一轮
    butterfly_compu(&buff_c[0], &buff_c[1], w0);
    butterfly_compu(&buff_c[2], &buff_c[3], w0);
    butterfly_compu(&buff_c[4], &buff_c[5], w0);
    butterfly_compu(&buff_c[6], &buff_c[7], w0);
    //第二轮
    butterfly_compu(&buff_c[0], &buff_c[2], w0);
    butterfly_compu(&buff_c[1], &buff_c[3], w2);
    butterfly_compu(&buff_c[4], &buff_c[6], w0);
    butterfly_compu(&buff_c[5], &buff_c[7], w2);
    //第三轮
    butterfly_compu(&buff_c[0], &buff_c[4], w0);
    butterfly_compu(&buff_c[1], &buff_c[5], w1);
    butterfly_compu(&buff_c[2], &buff_c[6], w2);
    butterfly_compu(&buff_c[3], &buff_c[7], w3);
    for (size_t i = 0; i < 8; i++)
    {
        *(buff + i * 2) = buff_c[i].re;
        *(buff + i * 2 + 1) = buff_c[i].im;
    }
}
/**
 * @brief 16point FFT
 * 
 * @param buff length of buff should be 32, and put the real part at even index, 
 * imaginary part at odd index  
 */
void fft_16(float* buff)
{
    complex_t tmp[16];
    for (size_t i = 0; i < 16; i++)
    {
        tmp[i].re = *(buff + 2 * i);
        tmp[i].im = *(buff + 2 * i + 1 );
    }
    complex_t buff_c[16];
    //将原序列的序号比特翻转，并存入临时数组
    for (size_t i = 0; i < 16; i++)
    {
        buff_c[i] = tmp[bit_reverse(i, 4)];
        // printf("buff_c[%d] = tmp[%d];\n", i, bit_reverse(i, 4));
    }
    complex_t w0 = W(16, 0);
    complex_t w1 = W(16, 1);
    complex_t w2 = W(16, 2);
    complex_t w3 = W(16, 3);
    complex_t w4 = W(16, 4);
    complex_t w5 = W(16, 5);
    complex_t w6 = W(16, 6);
    complex_t w7 = W(16, 7);
    //第一轮
    butterfly_compu(&buff_c[0], &buff_c[1], w0);
    butterfly_compu(&buff_c[2], &buff_c[3], w0);
    butterfly_compu(&buff_c[4], &buff_c[5], w0);
    butterfly_compu(&buff_c[6], &buff_c[7], w0);
    butterfly_compu(&buff_c[8], &buff_c[9], w0);
    butterfly_compu(&buff_c[10], &buff_c[11], w0);
    butterfly_compu(&buff_c[12], &buff_c[13], w0);
    butterfly_compu(&buff_c[14], &buff_c[15], w0);
    //第二轮
    butterfly_compu(&buff_c[0], &buff_c[2], w0);
    butterfly_compu(&buff_c[1], &buff_c[3], w4);
    butterfly_compu(&buff_c[4], &buff_c[6], w0);
    butterfly_compu(&buff_c[5], &buff_c[7], w4);
    butterfly_compu(&buff_c[8], &buff_c[10], w0);
    butterfly_compu(&buff_c[9], &buff_c[11], w4);
    butterfly_compu(&buff_c[12], &buff_c[14], w0);
    butterfly_compu(&buff_c[13], &buff_c[15], w4);
    //第三轮
    butterfly_compu(&buff_c[0], &buff_c[4], w0);
    butterfly_compu(&buff_c[1], &buff_c[5], w2);
    butterfly_compu(&buff_c[2], &buff_c[6], w4);
    butterfly_compu(&buff_c[3], &buff_c[7], w6);
    butterfly_compu(&buff_c[8], &buff_c[12], w0);
    butterfly_compu(&buff_c[9], &buff_c[13], w2);
    butterfly_compu(&buff_c[10], &buff_c[14], w4);
    butterfly_compu(&buff_c[11], &buff_c[15], w6);
    //第四轮
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
}
/**
 * @brief 32point FFT
 * 
 * @param buff length of buff should be 64, and put the real part at even index, 
 * imaginary part at odd index  
 */
void fft_32(float* buff)
{
    complex_t tmp[32];
    for (size_t i = 0; i < 32; i++)
    {
        tmp[i].re = *(buff + 2 * i);
        tmp[i].im = *(buff + 2 * i + 1 );
    }
    complex_t buff_c[32];
    //将原序列的序号比特翻转，并存入临时数组
    for (size_t i = 0; i < 32; i++)
    {
        buff_c[i] = tmp[bit_reverse(i, 5)];
        // printf("buff_c[%d] = tmp[%d];\n", i, bit_reverse(i, 5));
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
    //第一轮
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
    //第二轮
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
    //第三轮
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
    //第四轮
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
    //第五轮
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
}
