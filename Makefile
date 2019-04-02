CC=i686-w64-mingw32-gcc
WINDRES=i686-w64-mingw32-windres
CCFLAGS=-nostdlib
LDFLAGS=-Wl,--dynamicbase,--export-all-symbols,--entry="_WinMain@16"
LIBS=-luser32 -lwsock32 -lcrtdll -lkernel32

all: w3daytim

w3daytim:
	$(CC) -c w3daytim.c -o w3daytim.o
	$(WINDRES) w3daytim.rc -O coff -o w3daytim.res
	$(CC) w3daytim.o w3daytim.res -o w3daytim.exe $(LDFLAGS) $(CCFLAGS) $(LIBS)

clean:
	rm w3daytim.exe
	rm w3daytim.o
	rm w3daytim.res
	rm -r dist

dist: w3daytim
	mkdir -p dist
	cp w3daytim.exe dist/w3daytim.exe
	cp cfg/w3daytim.ini.tpl dist/w3daytim.ini
	gawk 'sub("$$", "\r")' README > dist/readme.txt
	gawk 'sub("$$", "\r")' COPYING > dist/copying.txt
	zip -r dist/src.zip cfg/ res/ Makefile w3daytim.c w3daytim.rc COPYING README README.md
	cd dist; zip -r w3daytim.zip src.zip w3daytim.exe w3daytim.ini readme.txt copying.txt

