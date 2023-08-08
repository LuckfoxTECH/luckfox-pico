# replace with your target cross-compile toolchains.
# default for host compiler.
#

# running:
#     shell>export LD_LIBRARY_PATH=../../lib
#	  shell>./ipcamera_demo

# you will got this message "[error]get license from file failed..."
# fix this:
#      step 1: access http://iot.open.qq.com/wiki/index.html#CASE/IP_Camera.md 
#      
#      step 2: replace "pid" which is a product id number in main.c 
#              copy sn file, licence file and server public key file (file name like "pid".pem) to ./
#
#	   step 3: > make

CC:=gcc

C_FLAGS:=-O0 -g3  -m32 -std=c99

C_FILES:=\
	alarm.c \
	audiofile.c \
	audiovideo.c \
	ipcrotate.c \
	main.c \
	ota.c


all: 
	$(CC) $(C_FLAGS) $(C_FILES) -o ipcamera_demo -I../../include -L../../lib -ltxdevicesdk -lpthread -ldl

clean:
	rm ipcamra_demo