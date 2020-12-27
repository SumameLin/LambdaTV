import os
import struct

prease_flag=0
file_num=1
filePath=r'./xbm' # 文件夹路径
fileList=os.listdir(filePath)# os.listdir(file)会历遍文件夹内的文件并返回一个列表
for file in fileList:
    file_name="./xbm/"+str(file_num) +'.xbm'
    print(file_name)
    f_xbm = open(file_name)
    xbm_data=f_xbm.read()
    for xbm in xbm_data:
        if xbm == 'x' and prease_flag == 0:
            prease_flag=1
        elif prease_flag == 1:
            str_to_hex=xbm
            prease_flag=2
        elif prease_flag == 2:
            str_to_hex+=xbm
            with open(os.path.abspath('.')+'/9.bin','ab+') as f_bin: #打开二进制文件(追加完成)
                data_write=struct.pack('B',int(str_to_hex,16)) # B : unsigned char
                f_bin.write(data_write)
            prease_flag=0
    f_xbm.close()
    f_bin.close()
    file_num+=1