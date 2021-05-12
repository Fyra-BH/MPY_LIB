a = 0.8;
N = 0:15;

x = a.^N;
y = ifft(x)';
y