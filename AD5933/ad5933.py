# MicroPython AD5933 driver, I2C interface

from micropython import const
import time
import ujson
import math
import array
# register definitions

REG_CTRL1 = const(0X80)  # 控制
REG_CTRL2 = const(0x81)

REG_START_FREQ1 = const(0x82)  # 起始频率
REG_START_FREQ2 = const(0x83)
REG_START_FREQ3 = const(0x84)

REG_INC_FREQ1 = const(0x85)  # 频率增量
REG_INC_FREQ2 = const(0x86)
REG_INC_FREQ3 = const(0x87)

REG_NUM_INC_FREQ1 = const(0x88)  # 增量数
REG_NUM_INC_FREQ2 = const(0x89)

REG_NUM_SCYCLES1 = const(0x8a)  # 建立时间周期数
REG_NUM_SCYCLES2 = const(0x8b)

REG_STATUS = const(0x8f)  # 状态

REG_TEMP1 = const(0x92)  # 温度数据
REG_TEMP2 = const(0x93)

REG_REAL_DATA1 = const(0x94)  # 实值
REG_REAL_DATA2 = const(0x95)

REG_IMAG_DATA1 = const(0x96)  # 虚值
REG_IMAG_DATA2 = const(0x97)

# commands (are to written into 0x80)
# CMD_NONE_OPERATE            = const(0x00) #无操作
CMD_INIT_WITH_START_FREQ = const(0x10)  # 以起始频率初始化
CMD_START_FREQ_SCAN = const(0x20)  # 启动频率扫描
CMD_STEPUP_FREQ = const(0x30)  # 递增频率
CMD_REPEAT_FREQ = const(0x40)  # 重复频率
# CMD_NONE_OPERATE            = const(0x80) #无操作
CMD_READ_TMP = const(0x90)  # 测量温度
CMD_POWER_DOWN_MODE = const(0xa0)  # 省电模式
CMD_IDLE_MODE = const(0xb0)  # 待机模式

# states
STA_TEMP_CONVERT_EFECT = const(0x01)  # 温度转换状态位
STA_MEASURE_EFECT = const(0x02)  # 实值/虚值转换状态位
STA_FREQ_SCAN_FIN = const(0x04)  # 频率扫描完成状态位

ad5933_config_filename = "config.json"


class AD5933:
    def __init__(self, i2c):
        self.I2C = i2c
        self.ID = self.I2C.scan()[0]
        self.PGA = 1  # PGA增益 控制寄存器D8位(D0~15),D8 = 0时x5, D8 = 1时x1
        try:
            f = open(ad5933_config_filename)
            args = ujson.load(f)
            self.GAIN_FACTOR_BASE, self.GAIN_FACTOR_END, self.CLK_FREQ, self.SYS_PHASE, self.start_f, self.step, self.num, self.cycle = \
                args["gain_factor_base"], args["gain_factor_end"], args["clk_freq"], args["sys_phase"], args["start_f"], args["step"], args["num"], args["cycle"] \

        except:
            print("please call AD5933_obj.setup")
            print("default set_up(ref_r = 10000, clk_freq = 16776000, start_freq = 10000, freq_step = 500, num_inc_freq = 180, cycle = 15)")

    # 更新时钟频率

    def update_clk_freq(self, freq):
        self.CLK_FREQ = freq
    # 读寄存器值

    def read_reg(self, addr, size=1):
        ret, data_raw = 0, list(self.I2C.readfrom_mem(self.ID, addr, size))
        for var in data_raw:
            ret = ret*256 + var
        return ret
    # 写寄存器值(二进制数列形式)

    def write_reg_buff(self, addr, buf):
        self.I2C.writeto_mem(self.ID, addr, buf)
    # 写寄存器值(整数形式)

    def write_reg_int(self, addr, val):
        self.I2C.writeto_mem(self.ID, addr, bytes(list([val])))
    # 向控制寄存器0x80写命令

    def write_cmd(self, cmd):
        #         print('write ' + str(bin(cmd)))
        if self.PGA == 1:
            cmd = cmd | 0x01
        self.write_reg_int(REG_CTRL1, cmd)
    # 选择时钟源, c = 0内部时钟, c != 0外部时钟

    def select_clk(self, c):
        if c == 0:
            self.write_reg_int(REG_CTRL2, 0x00)
        else:
            self.write_reg_int(REG_CTRL2,  0xff)
    # 获取状态

    def get_sta(self):
        return self.read_reg(REG_STATUS)

    def reset(self):
        self.write_reg_int(REG_CTRL2, 0xff)
    # 读取温度

    def get_temp(self):
        self.write_cmd(CMD_READ_TMP)
        while(self.get_sta() & STA_TEMP_CONVERT_EFECT != 1):
            pass
        temp = 0
        temp = self.read_reg(REG_TEMP1)*256
        temp += self.read_reg(REG_TEMP2)
        if temp & 0x2000 == 1:
            return (temp - 16384)/32  # 负温度
        else:
            return temp/32  # 正温度
    # 设置起始频率

    def set_start_freq(self, freq):
        reg_val = int((freq/(self.CLK_FREQ/4))*2**27)
        self.write_reg_int(REG_START_FREQ1, (reg_val >> 16) & 0xff)
        self.write_reg_int(REG_START_FREQ2, (reg_val >> 8) & 0xff)
        self.write_reg_int(REG_START_FREQ3, (reg_val) & 0xff)
    # 设置频率增量

    def set_freq_step(self, freq_step):
        reg_val = int((freq_step/(self.CLK_FREQ/4))*2**27)
        self.write_reg_int(REG_INC_FREQ1, (reg_val >> 16) & 0xff)
        self.write_reg_int(REG_INC_FREQ2, (reg_val >> 8) & 0xff)
        self.write_reg_int(REG_INC_FREQ3, (reg_val) & 0xff)
    # 设置增量数

    def set_num_inc_freq(self, num):
        self.write_reg_int(REG_NUM_INC_FREQ1, (num >> 8) & 0xff)
        self.write_reg_int(REG_NUM_INC_FREQ2, num & 0xff)
    #设置建立时间周期数, times为周期数倍数, 可取的值为1, 2, 4

    def set_scycle(self, num, times=1):
        self.write_reg_int(REG_NUM_SCYCLES2, num & 0xff)
        reg_val = (num >> 8) & 0x01
        if times == 2:
            reg_val = reg_val | (0x01 << 1)
        elif times == 4:
            reg_val = reg_val | (0x3 << 1)
        self.write_reg_int(REG_NUM_SCYCLES1, reg_val)

    def read_ri_data(self):  # read real and imagine data
        reg_real = self.read_reg(REG_REAL_DATA1) << 8
        reg_real += self.read_reg(REG_REAL_DATA2)
        reg_imagine = self.read_reg(REG_IMAG_DATA1) << 8
        reg_imagine += self.read_reg(REG_IMAG_DATA2)
        reg_real, reg_imagine = _s_int16(reg_real), _s_int16(reg_imagine)
        return reg_real, reg_imagine
    # 等待数据转换

    def wait_data_convert(self):
        while(self.get_sta() & STA_MEASURE_EFECT != 0x02):
            pass
    # 扫描是否结束

    def scan_fin(self):
        return self.get_sta() & STA_FREQ_SCAN_FIN == 0x04

    def get_phase(self, re, im):
        phase = 0.0
        if re > 0 and im > 0:
            phase = math.atan(im / re) * 180 / math.pi
        if re < 0 and im > 0:
            phase = math.atan(im / re) * 180 / math.pi + 180
        if re < 0 and im < 0:
            phase = math.atan(im / re) * 180 / math.pi + 180
        if re > 0 and im < 0:
            phase = math.atan(im / re) * 180 / math.pi + 360
        # phase = math.atan(im / re) * 180 / math.pi
        return phase

    # #使用已知电阻确定增益系数
    def set_up(self, ref_r=10000, clk_freq=16776000, start_freq=10000, freq_step=500, num_inc_freq=180, cycle=15):
        print("ref =", ref_r, ", clk_freq =", clk_freq, ", start_freq =", start_freq, ", freq_step =", freq_step,
              ", num_inc_freq =", num_inc_freq, ", cycle =", cycle)
        self.REF_R = ref_r
        self.CLK_FREQ = clk_freq
        self.reset()
        self.select_clk(0)
        self.set_start_freq(start_freq)
        self.set_freq_step(freq_step)
        self.set_num_inc_freq(num_inc_freq)
        self.set_scycle(cycle)
        self.write_cmd(CMD_IDLE_MODE)
        self.write_cmd(CMD_INIT_WITH_START_FREQ)
        time.sleep_ms(10)
        self.write_cmd(CMD_START_FREQ_SCAN)
        self.wait_data_convert()
        reg_real, reg_imagine = self.read_ri_data()
        print("reg_real = " + str(reg_real))
        print("reg_imagine = " + str(reg_imagine))
        temp = (reg_real**2 + reg_imagine**2)**0.5
        self.GAIN_FACTOR_BASE = (1 / ref_r) / temp
        self.SYS_PHASE = array.array("f", [])
        while(not self.scan_fin()):
            self.wait_data_convert()
            reg_real, reg_imagine = self.read_ri_data()
            self.SYS_PHASE.append(self.get_phase(reg_real, reg_imagine))
            self.write_cmd(CMD_STEPUP_FREQ)
            # print(reg_real, "\t", reg_imagine, "\t", self.get_phase(reg_real, reg_imagine))

        self.GAIN_FACTOR_END = (1 / ref_r) / \
            (reg_real**2 + reg_imagine**2)**0.5

        self.start_f, self.step, self.num, self.cycle = start_freq, freq_step, num_inc_freq, cycle
        f, args = open(ad5933_config_filename, "w"), dict()
        args["gain_factor_base"], args["gain_factor_end"], args["clk_freq"], args["start_f"], args["step"], args["num"], args["cycle"], args["sys_phase"], \
            = self.GAIN_FACTOR_BASE, self.GAIN_FACTOR_END, self.CLK_FREQ, self.start_f, self.step, self.num, self.cycle, list(self.SYS_PHASE)
        ujson.dump(args, f)
        f.close()

    def measure(self):
        self.write_cmd(CMD_REPEAT_FREQ)
        self.wait_data_convert()
        reg_real, reg_imagine = self.read_ri_data()
        temp = (reg_real**2 + reg_imagine**2)**0.5
        return (1/self.GAIN_FACTOR_BASE/temp)

    def scan_freq(self):
        print("start =", self.start_f, ", end =", self.start_f +
              self.step * self.num, ", step =", self.step)
        res_z, res_p, cnt = array.array("f", []), array.array("f", []), 0
        self.reset()
        self.select_clk(0)
        self.set_start_freq(int(self.start_f))
        self.set_freq_step(int(self.step))
        self.set_num_inc_freq(self.num)
        self.set_scycle(self.cycle)
        self.write_cmd(CMD_IDLE_MODE)
        self.write_cmd(CMD_INIT_WITH_START_FREQ)
        time.sleep_ms(10)
        self.write_cmd(CMD_START_FREQ_SCAN)
        while(not self.scan_fin()):
            self.wait_data_convert()
            reg_real, reg_imagine = self.read_ri_data()
            # print(reg_real, "\t", reg_imagine, "\t", self.get_phase(reg_real, reg_imagine))
            temp = (reg_real**2 + reg_imagine**2)**0.5
            z = (1/self.get_fixedGF(self.start_f + cnt * self.step)/temp)
            res_z.append(z)
            phase = self.get_phase(reg_real, reg_imagine) - self.SYS_PHASE[cnt]
            if phase < -180:
                phase = phase + 360
            res_p.append(phase)
            # res_p.append(self.get_phase(reg_real, reg_imagine))
            self.write_cmd(CMD_STEPUP_FREQ)
            cnt += 1
        print("{} points measured".format(cnt))
        return res_z, res_p
    # 获取修正的增益系数

    def get_fixedGF(self, freq):
        return self.GAIN_FACTOR_BASE + (self.GAIN_FACTOR_END - self.GAIN_FACTOR_BASE)/(self.step * self.num + self.start_f) * freq
        # return self.GAIN_FACTOR_BASE

# 转换为16位有符号数


def _s_int16(n):
    if (n >> 15) & 0x01 == 1:
        return n - (1 << 16)
    else:
        return n
