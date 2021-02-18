CC=gcc
ASMBIN=nasm

all : asm cc link
asm : 
	$(ASMBIN) -o copyLines.o -f elf -l copyLines.lst copyLines.asm
cc :
	$(CC) -m32 -c -fpack-struct -g -O0 rotation.c
link :
	$(CC) -m32 -o rotation rotation.o copyLines.o
clean :
	rm *.o
	rm rotation
	rm copyLines.lst
