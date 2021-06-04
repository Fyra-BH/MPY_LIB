
from pyb import Pin
import utime

class si2c:
    __scl=Pin.__class__
    __sda=Pin.__class__
    __scl_pin=''
    __sda_pin=''
    def __init__(self,scl,sda):
        try:
            self.__scl_pin=scl
            self.__sda_pin=sda
            self.__scl=Pin(scl,Pin.OUT_PP,pull=Pin.PULL_UP)
            self.__sda=Pin(sda,Pin.OUT_PP,pull=Pin.PULL_UP)
            self.__scl.high()
            self.__sda.high()
            pass
        except:
            pass
    def __sda_in(self):
        self.__sda=Pin(self.__sda_pin,Pin.IN,pull=Pin.PULL_UP)
    def __sda_out(self):
        self.__sda=Pin(self.__sda_pin,Pin.OUT_PP,pull=Pin.PULL_UP)

    def start(self):
        self.__sda_out()
        self.__scl.high()
        self.__sda.high()
        utime.sleep_us(5)
        self.__sda.low()
        utime.sleep_us(6)
        self.__scl.low()

    def end(self):
        self.__scl.low()
        self.__sda_out()
        self.__sda.low()
        self.__scl.high()
        utime.sleep_us(6)
        self.__sda.high()
        utime.sleep_us(6)

    def wait_ack(self):
        time=0
        self.__sda_in()
        self.__sda.high()
        utime.sleep_us(2)
        self.__scl.high()
        while self.__sda.value() == 1:
            time += 1
            if time>250:
                self.end()
                return 1#应答失败
        self.__scl.low()
        return 0 #应答成功
    #产生应答,state=1应答，state=0无应答
    def ack(self,state=0):
        self.__scl.low()
        self.__sda_out()
        if state==1:
            self.__sda.low()
        else:
            self.__sda.high()
        utime.sleep_us(2)
        self.__scl.high()
        utime.sleep_us(5)
        self.__scl.low()

    def send_byte(self,b):
        b=bin(b).replace('0b','')
        b='{0:0>8}'.format(b)
        # print(b)
        self.__sda_out()# 输出模式
        # self.start()
        for ch in b:
            if ch == '0':
                self.__sda.low()
            else:
                self.__sda.high()
            utime.sleep_us(2)
            self.__scl.high()
            utime.sleep_us(2)
            self.__scl.low()
            utime.sleep_us(2)

    def read_byte(self,ask=1):
        self.__sda_in()
        b='0b'
        for i in range(8):
            self.__scl.low()
            utime.sleep_us(2)
            self.__scl.high()
            if self.__sda.value()==1:
                b+='1'
            else:
                b+='0'
        return int(b)