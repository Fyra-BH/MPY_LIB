from ezfft import *
from array import *
from math import *
import time, gc

def test(N):
    fs, f0 = 8000, 777
    x = array('f', [])
    for i in range(N):
        x.append(sin(2 * pi * i * f0 / fs))
        x.append(0)
        if i % 100 == 0: gc.collect()
    start = time.ticks_ms()
    fft(x)
    end = time.ticks_ms()
    print("fft spent {} ms".format(end - start))
    start = time.ticks_ms()
    y = ezabs(x)
    end = time.ticks_ms()
    print("abs spent {} ms".format(end - start))
    return list(y).index(max(y)) * fs / N
    