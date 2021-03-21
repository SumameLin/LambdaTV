import os
import struct

prease_flag = 0
file_num = 1
filePath = r'./space/xbm'  # 文件夹路径
fileList = os.listdir(filePath)  # os.listdir(file)会历遍文件夹内的文件并返回一个列表
for file in fileList:
    file_name = "./space/xbm/"+str(file_num) + '.xbm'
    print(file_name)
    f_xbm = open(file_name)
    xbm_data = f_xbm.read()
    for xbm in xbm_data:
        if xbm == 'x' and prease_flag == 0:
            prease_flag = 1
        elif prease_flag == 1:
            str_to_hex = xbm
            prease_flag = 2
        elif prease_flag == 2:
            str_to_hex += xbm
            # 打开二进制文件(追加完成)
            with open(os.path.abspath('.') + './space.bin', 'ab+') as f_bin:
                # B :unsigned char
                data_write = struct.pack('B', int(str_to_hex, 16))
                f_bin.write(data_write)
            prease_flag = 0
    f_xbm.close()
    f_bin.close()
    file_num += 1
