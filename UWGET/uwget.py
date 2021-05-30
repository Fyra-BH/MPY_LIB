# 尝试在MicroPython上实现http下载
import socket

MSG =[
    'GET {0} HTTP/1.1',
    'Host: {1}',
    # 'Connection: keep-alive',
    'User-Agent: Mozilla/5.0 ',
    'Accept-Language: zh-CN,zh;q=0.9',
    'transfer-coding: chunked',
    '',
    '',
]


'''
下载给定url的文件
'''
def get(url, path=None, buffsize=1024):
    info = url.split('/')
    if buffsize < 1024: buffsize = 1024 # set an appropriate buffsize, lager than 1024
    if(info[0] == 'http:'):
        try:
            domain = info[2]
            file_name = info[-1]
            if path == None: path = file_name # set default save path

            host = socket.getaddrinfo(domain,80)[0][-1][0] # get ipaddr of server
            client = socket.socket()
            client.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1024*4)
            client.connect((host, 80))
            client.send('\r\n'.join(MSG).format(url, host).encode('utf8'))
            
            rsp=client.recv(buffsize) # we need to remove some extral information from the first frame 
            rsp = rsp[rsp.index(b'Content-Type') : ]
            rsp = rsp[rsp.index(b'\r\n') + 4 : ]
            
            with open(file_name, 'wb') as fp:
                fp.write(rsp)
                while True:
                    rsp=client.recv(buffsize) # we need to remove some extral information from the first frame 
                    print(len(rsp))
                    if(len(rsp) == 0): break
                    fp.write(rsp)
        except:
            print("some problems occurred !")
    else:
        print("protocol not supported")

