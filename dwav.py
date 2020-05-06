# wav解码对象
#说明:默认wav文件不含fact块

dis='▁▁▂▃▅▆▇▉▉'

class dwav:
    __riff_chunk={}
    __fmt_chunk={}
    __buff_size=0
    __data_size=0
    __file=None
    def __init__(self,fname):
        print("open {}".format(fname))
        self.__file=open(fname,'rb')
        buff=self.__file.read(38)
        self.__riff_chunk['docID']=str(buff[0:4]).replace('b','').replace("'","")#文件头标识get
        self.__riff_chunk['f_size']=int.from_bytes(buff[4:8],'little')#文件大小get
        self.__riff_chunk['f_type']=str(buff[8:0xc]).replace('b','').replace("'","")#文件类型get
        self.__fmt_chunk['fmt_blockID']=str(buff[0xc:0x10]).replace('b','').replace("'","")
        self.__fmt_chunk['block_len']=int.from_bytes(buff[0x10:0x14],'little')#块大小get
        self.__fmt_chunk['coding_format']=int.from_bytes(buff[0x14:0x16],'little')#编码格式get
        self.__fmt_chunk['tracks']=int.from_bytes(buff[0x16:0x18],'little')#声道数get
        self.__fmt_chunk['sample_freq']=int.from_bytes(buff[0x18:0x1c],'little')#采样率get
        self.__fmt_chunk['byte_rate']=int.from_bytes(buff[0x1c:0x20],'little')#数据传输速率get
        self.__fmt_chunk['blocks_align']=int.from_bytes(buff[0x20:0x22],'little')#数据块对齐单位get
        self.__fmt_chunk['sample_bits']=int.from_bytes(buff[0x22:0x24],'little')#采样位数get
        self.__buff_size=int(self.__fmt_chunk['byte_rate']/100) #10ms的缓冲区
        print("buff_size={}".format(self.__buff_size))
        for var in self.__riff_chunk:
            print('{0:-<20}'.format(var),end='')
            print(self.__riff_chunk[var])
        for var in self.__fmt_chunk:
            print('{0:-<20}'.format(var),end='')
            print(self.__fmt_chunk[var])
        buff=self.__file.read(4) #'d' 'a' 't' 'a'
        buff=self.__file.read(4)
        self.__data_size=int.from_bytes(buff,'little')#数据大小get
        print('data size ={}'.format(self.__data_size))
        
    def __send_buff(self,buff):
        sz=len(buff)
        for i in range(int(sz/4)):
            data=int(buff[i*4]+buff[i*4+1]*256)
            # print(data)
            data=int(data/8192)
            print(dis[data],end='')
    def play(self):
        t=0
        data_buff=self.__file.read(self.__buff_size);
        while True:
            self.__send_buff(data_buff)
            if len(data_buff)!=self.__buff_size:
                break
            data_buff=self.__file.read(self.__buff_size)
            t+=10
            # print("\r{0:.2f}s passed".format(t/1000),end='')

        