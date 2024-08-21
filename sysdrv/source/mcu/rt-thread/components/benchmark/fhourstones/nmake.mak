# NMAKE Makefile for Microsoft Windows

# To build:
#   nmake -f nmake.mak <target>

# FIXME: dependencies aren't fully described here,
#        as SearchGame.c includes TransGame.c, which includes Game.c
#        This should be fixed in the Unix makefile as well,
#        but that currently avoids the problem by compiling all C, every time.

CFLAGS = /O2

all : SearchGame.exe

clean : ; del SearchGame.exe *.obj

run : SearchGame.exe inputs
	SearchGame < inputs
