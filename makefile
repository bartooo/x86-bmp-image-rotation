CC=gcc
ASMBIN=nasm

all : asm cc link
asm : 
	$(ASMBIN) -o copyLines64.o -f elf64 copyLines64.asm
cc :
	$(CC) -c -g -O0 rotation64.c
link :
	$(CC) -o rotation64 rotation64.o copyLines64.o
clean :
	rm *.o
	rm rotation64
