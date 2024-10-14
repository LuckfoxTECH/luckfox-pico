#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c) 2018, Linaro Limited
#

def get_args():
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('--name', required=True,
                        help='Name of variable in C file')
    parser.add_argument('--out', required=True, help='Name of C file')
    parser.add_argument('--inf', required=True, help='Name of in file')

    return parser.parse_args()

def main():
    import array

    args = get_args()

    inf = open(args.inf, 'r')
    f = open(args.out, 'w')

    f.write("/* automatically generated */\n")
    f.write("#include <stdint.h>\n")
    f.write("#include <stddef.h>\n\n")

    f.write("const uint8_t " + args.name + "[] = {\n")
    i = 0
    for x in array.array("B", map(ord, (inf.read()))):
        f.write("0x" + '{0:02x}'.format(x) + ",")
        i = i + 1
        if i % 8 == 0:
            f.write("\n")
        else:
            f.write(" ")

    f.write("'\\0'};\n")
    f.write("const size_t " + args.name + "_size = sizeof(" + 
            args.name + ") - 1;\n")

    f.close()
    inf.close()

if __name__ == "__main__":
    main()
