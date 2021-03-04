import array, time
from ezfft import *

def test(N):
    x = array.array('f', [])
    for i in range(N):
        x.append(0.8**i)
        x.append(0)
    start = time.ticks_ms()
    if N == 8: fft8(x)
    elif N == 16: fft16(x)
    elif N == 32: fft32(x)
    elif N == 64: fft64(x)
    elif N == 128: fft128(x)
    elif N == 256: fft256(x)
    elif N == 1024: fft1024(x)
    end = time.ticks_ms()
    
    print("spent {} ms".format(end - start))
    
