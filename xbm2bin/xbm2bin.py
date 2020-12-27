import os
import struct

xbm_data = open('40.xbm').read()
# print(xbm_data)
prease_flag=0

for xbm in xbm_data:
    if xbm == 'x' and prease_flag == 0:
        prease_flag=1
    elif prease_flag == 1:
        str_to_hex=xbm
        prease_flag=2
    elif prease_flag == 2:
        str_to_hex+=xbm
        with open(os.path.abspath('.')+'/apple.bin','ab+') as f_bin: #打开二进制文件(追加完成)
            data_write=struct.pack('B',int(str_to_hex,16))
            f_bin.write(data_write)
        prease_flag=0
f_bin.close()