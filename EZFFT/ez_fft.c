#include "ez_fft.h"

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

/**
 * @brief 求N对2的对数
 * 
 * @param N 2的整次幂
 * @return int 当N不是2的整次幂时返回0
 */
int ezlog2(int N)
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
 * @return int 正常返回0, 否则返回1
 */
int fft_N(int N, float* buff)
{
    if(ezlog2(N) == 0 ) return 1;
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
    return 0;
}
