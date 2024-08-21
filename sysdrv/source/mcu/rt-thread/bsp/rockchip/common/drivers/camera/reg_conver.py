# !/usr/bin/python2
# encoding: utf-8
import re
import numpy as np
import string

filename = "reg.txt"
cols = 2 # number of column
reg_addr = 0
reg_val = 1
REG_END = 0
REG_DELAY = 255

def openreadtxt(file_name):
    file = open(file_name,"r")
    file_data = file.readlines() # read all line
    lines = len(file_data)
    row = 0
    np.set_printoptions(formatter={'int':hex})
    str_array = np.zeros((lines, cols))
    data_array = np.zeros((lines, cols))
    hex_array = np.zeros((1, cols))
    hex_array = hex_array.astype(np.uint32)
    data_array = data_array.astype(np.uint32)
    for line in file_data:
        tmp_list = line.strip(',').strip('\n').split(',', 1)
        hex_array[0,0] = int(np.array(tmp_list)[0], 16)
        hex_array[0,1] = int(np.array(tmp_list)[1], 16)
        data_array[row, :] = hex_array[0, :]
        row += 1
    return data_array

def continues_reg_conversion(reg_array):
    cnt = 0
    k = 0
    lines = len(reg_array)
    data = np.zeros((lines, 1))
    data = data.astype(np.uint32)
    reg_array = reg_array.astype(np.uint32)

    for i in range(0, lines, 1):
        if (reg_array[i, reg_addr] == 0).any():
            if k > 0:
                cnt+=1
                print "0x%x, " % (k+2),
                for j in range(0, k + 2, 1):
                    print "0x%02x, " % data.take(j),
                print "\n",
                k = 0
        if (reg_array[i, reg_addr] == REG_DELAY).any():
            if k > 0:
                cnt+=1
                print "0x%x" % (k+2),
                for j in range(0, k + 2, 1):
                    print "0x%02x" % data.take(j),
                print "\n",
                k = 0
        else:
            if k == 0:
                reg_addr_tmp = reg_array[i, reg_addr]
                data[0] = ((reg_addr_tmp >> 8) & 0xff)
                data[1] = ((reg_addr_tmp >> 0) & 0xff)
                data[2] = reg_array[i, reg_val]
                k+=1
            else:
                if (reg_array[i - 1, reg_addr] + 1 == reg_array[i, reg_addr]).any():
                    data[k + 2] = reg_array[i, reg_val]
                    k+=1
                else:
                    cnt+=1
                    print "0x%x," % (k+2),
                    for j in range(0, k + 2, 1):
                        print "0x%02x," % data.take(j),
                    print "\n",
                    reg_addr_tmp = reg_array[i, reg_addr]
                    data[0] = ((reg_addr_tmp >> 8) & 0xff)
                    data[1] = ((reg_addr_tmp >> 0) & 0xff)
                    data[2] = reg_array[i, reg_val]
                    k = 1
                if i == (lines - 1):
                    print "0x%x," % (k+2),
                    for j in range(0, k + 2, 1):
                        print "0x%02x," % data.take(j),
                    print "\n",
                    cnt+=1
    print "0x%x, \n" % cnt,

if __name__=="__main__":
    data_array = openreadtxt(filename)
    continues_reg_conversion(data_array)
