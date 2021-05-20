# 用于给信号加窗

import math

# 将序列a加窗后返回


def hamming(a):
    a0 = 0.53836
    N = len(a)
    for i in range(N):
        w = a0 - (1 - a0)*math.cos(2*math.pi*i/(N-1))
        a[i] *= w
    return a

def hann(a):
    a0 = 0.5
    N = len(a)
    for i in range(N):
        w = a0 - (1 - a0)*math.cos(2*math.pi*i/(N-1))
        a[i] *= w
    return a

