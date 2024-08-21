#!/usr/bin/env python3
#
# Copyright 2023, Rockchip Electronics Co., Ltd
# hisping lin, <hisping.lin@rock-chips.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#

def get_args():
	import argparse

	parser = argparse.ArgumentParser()

	parser.add_argument('--out', required=True, \
		help='Name of image for the TA')

	parser.add_argument('--tadir', required=True, help='Path of TA director')

	return parser.parse_args()

def main():
	import os
	import array
	import glob
	import struct
	import uuid
	try:
		from Cryptodome.Hash import SHA256
	except ImportError:
		from Crypto.Hash import SHA256

	args = get_args();

	path = args.tadir + "/*.ta"
	files = glob.glob(path)
	for f in files:
		print(f)

	fimg = open(args.out, 'wb')
	magic = 0x524B5441 #RKTA
	img_ver = 0x00000001
	ta_num = len(files)
	reserve = 0
	header = struct.pack('<IIII', magic, img_ver, ta_num, reserve)
	fimg.write(header)

	g_offset = len(header) + (ta_num * 32)
	for f in files:
		fta = open(f, 'rb')
		tadata = fta.read()
		fta.close
		ta_name = os.path.basename(f)
		ta_name_without_extension = os.path.splitext(ta_name)[0]
		ta_uuid = uuid.UUID(ta_name_without_extension)
		ta_uuid_hex = bytes.fromhex(ta_uuid.hex)
		fimg.write(ta_uuid_hex)
		ta_offset = struct.pack('<I', g_offset)
		fimg.write(ta_offset)
		ta_len = struct.pack('<I', len(tadata))
		fimg.write(ta_len)
		ta_ver_num = len(ta_name_without_extension) - 34;
		ta_ver = struct.pack('<I', ta_ver_num)
		fimg.write(ta_ver)
		ta_reserve = struct.pack('<I', reserve)
		fimg.write(ta_reserve)
		g_offset += len(tadata)

	for f in files:
		fta = open(f, 'rb')
		tadata = fta.read()
		fta.close
		fimg.write(tadata)
	fimg.close

	fimg = open(args.out, 'rb+')
	userta = fimg.read()
	h = SHA256.new()
	h.update(userta)
	img_digest = h.digest()
	fimg.write(img_digest)
	fimg.close

	print("pack ta binary success!")

if __name__ == "__main__":
	main()
