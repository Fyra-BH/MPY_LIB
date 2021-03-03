### 文件说明

## main.c

用于在pc端检测功能正确性，具体操作如下：

在终端中输入 

gcc *.c* -o out
./out

即可看到测试输出

## ez_fft.c/ez_fft.h

单独提供简单的fft函数

## fft_Ngenerator.py

用于自动生成N点fft代码

## mpy_port

提供micropython接口，可作为模块编译进固件