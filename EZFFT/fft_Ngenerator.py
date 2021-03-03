# fft蝶形运算具有规律性，于是可以制作代码生成器产生部分代码

import math

str_buff = [\
"void fft_N(float* buff)",
"{",
"    complex_t tmp[N];",
"    for (size_t i = 0; i < N; i++)",
"    {",
"        tmp[i].re = *(buff + 2 * i);",
"        tmp[i].im = *(buff + 2 * i + 1 );",
"    }",
"    complex_t buff_c[N];",
"    //将原序列的序号比特翻转，并存入临时数组",
"    for (size_t i = 0; i < N; i++)",
"    {",
"        buff_c[i] = tmp[bit_reverse(i, bitLen)];",
"    }",
]

str_buff_end =[\
"    for (size_t i = 0; i < N; i++)",
"    {",
"        *(buff + i * 2) = buff_c[i].re;",
"        *(buff + i * 2 + 1) = buff_c[i].im;",
"    }",
"}"
]

if __name__ == "__main__":
    N = int(input("please input N,(N must be 2^x)\n"))
    bitLen = int(math.log2(N)) #位倒序时要用到

    # 替换N
    str_buff = list(map(lambda x: x.replace('N', str(N)).replace("bitLen", str(bitLen)), str_buff))
    str_buff_end = list(map(lambda x: x.replace('N', str(N)).replace("bitLen", str(bitLen)), str_buff_end))

    # 添加旋转因子
    for i in range(N//2):
        str_buff.append("    complex_t w{1} = W({0}, {1});".format(N, i))

    # 开始bitLen轮蝶形运算
    for i in range(bitLen):
        str_buff.append("//第{}轮运算".format(i+1))
        template, str_tmp = "   butterfly_compu(&buff_c[{0}], &buff_c[{1}], w{2});", []
        # 分开运算
        for j in range(N // (2**(i + 1))):
            for k in range(2**i):
                x = 2**(i + 1) * j + k
                y = x + 2**i
                z = 0 if i == 0 else k * N // 2**(i + 1)
                str_tmp.append(template.format(x, y, z))
        str_buff += str_tmp
    str_buff += str_buff_end
    
    file_name = input("input your file name : eg. fft_N.c\n")
    with open(file_name, "w", encoding="utf-8") as fp:
        for line in str_buff:
            fp.write(line + "\n")