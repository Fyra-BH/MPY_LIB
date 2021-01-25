import serial
import serial.tools.list_ports as list_ports
import os

comports = list_ports.comports()

com_list = []
for var in comports:
    print("\t".join(list(var)))
    com_list.append(var[0])
if len(com_list) == 0:
    print("无串口")
    exit()

com_index, com = input("请选择端口(输入0~{0:}) : ".format(len(com_list)-1)), None
try: com = com_list[int(com_index)]
except: com = com_list[0]

baud = 0
try: baud = int(input("请设置波特率(默认115200) :"))
except: baud = 115200

with serial.Serial() as ser:
    ser.baudrate = baud
    ser.port = com
    ser.timeout = 1
    ser.open()
    ser.write(b"\r\n")
    while True:
        line = ser.readline()
        print(str(line, encoding="utf-8"))
        # print(line)
        if line == b'>>> ': break #进入repl
    cmd = b"import os;os.listdir()\r\n"
    ser.timeout = 1
    ser.write(cmd)
    ser.readline()
    line = str(ser.readline(), encoding="utf-8")
    line = line[line.index("[")+1 : line.index("]")-1].replace("'", "")
    print("根目录 :")
    print(line)

    local_file =  input("请选择本地文件 :")
    # local_file = "test.txt"
    (path, filename) = os.path.split(local_file)
    dist_file = input("请选择要上传的目录 :")
    # dist_file ="/"

    PER_SIZE = 512
    cmd = bytes('f_tmp = open("{0:}{1:}","wb")\r\n'.format(dist_file, filename), encoding="utf-8")
    ser.write(cmd)
    f1 = open(local_file, "rb")
    bytes_cnt = 0
    while True:
        buf = str(list(f1.read(PER_SIZE)))
        cmd = bytes("buf_tmp = bytes({0:})\r\n".format(buf) , encoding="utf-8")
        ser.write(cmd)
        _ = (str(ser.readline(),encoding="utf-8"))
        cmd = b"f_tmp.write(buf_tmp)\r\n"
        ser.write(cmd)
        while True:
            line = str(ser.readline(),encoding="utf-8")
            try:
                count = int(line)
                bytes_cnt += count
                print("count = ", count)
                break
            except: 
                if len(line) < 50:
                    print(line)
        print(bytes_cnt//1024,"KB written")
        if len(buf) < PER_SIZE: break
            
    cmd = b"f_tmp.close()\r\n"
    ser.write(cmd)
    ser.timeout =2
    while True:
        line = str(ser.readline(),encoding="utf-8")
        print(line)
        if line == ">>> ": break