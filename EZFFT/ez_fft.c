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
int bit_reverse(int x, int bit_len)
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
complex_t multi_c(complex_t a, complex_t b)
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
complex_t add_c(complex_t a, complex_t b)
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
complex_t W(int N, int k)
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
void butterfly_compu(complex_t* a, complex_t* b, complex_t w)
{
    complex_t tmp1, tmp2;
    tmp1 = add_c(*a, multi_c(*b, w));
    tmp2 = sub_c(*a, multi_c(*b, w));
    *a = tmp1;
    *b = tmp2;
}

/**
 * @brief 计算复数序列的摸，使用原缓冲区的前一半储存
 * 
 * @param buff 缓冲区，实部虚部交错
 * @param N 复数列长度，为缓冲区长度的一半
 */
void ezfft_abs(float* buff, int N)
{
    for (int i = 0; i < N; i++)
        *(buff + i) = pow(*(buff + i * 2) * *(buff + i * 2) + *(buff + i * 2 + 1) * *(buff + i * 2 + 1), 0.5);
}