# GNU Makefile

CC = /home/hexiongjun/SmartAuto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-fsl-linux-gnueabi-gcc
CFLAGS = -O3 -Wextra -Wall
CFILES = SearchGame.c TransGame.c Game.c
JAVAFILES = SearchGame.java Connect4.java Game.java TransGame.java
HASKELLFILES = Main.hs Connect4.hs GameTreeSearch.hs
FILES = Makefile inputs $(CFILES) $(JAVAFILES) $(HASKELLFILES)

.PHONY : all exe run java haskell clean

all : SearchGame 

clean : ; rm -f SearchGame SearchGame.ghc *.class *.o *.hi *.tar.gz *.zip

run : SearchGame inputs
	./SearchGame < inputs

SearchGame : $(CFILES)
	$(CC) $(CFLAGS) -o $@ $<

java : SearchGame.class
SearchGame.class : $(JAVAFILES)
	javac -O $<
	# to run, 'java SearchGame < inputs'

haskell : SearchGame.ghc
SearchGame.ghc : $(HASKELLFILES)
	ghc -o SearchGame.ghc -O --make $<
	# to run, './SearchGame.ghc < inputs'

tar:    $(FILES)
	tar -cf Fhourstones.tar $(FILES)
	gzip Fhourstones.tar
	zip -r Fhourstones.zip $(FILES)
