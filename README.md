# BMP Image rotation in x86 assembly and C

## Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Usage](#usage)


## About <a name = "about"></a>

Project for ARKO (Computer Architecture) course at Warsaw University of Technology. Project is implemented in x86 Assembly 64-bit version and in C language. Its purpose is to simply rotate BMP image by 90 degrees clockwise.

## Getting Started on Linux <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine with Linux OS for testing purposes.
### Prerequisites
- Linux
- NASM
- GCC

### Installing

- Install NASM

```
sudo apt-get install -y nasm
```
- Install gcc

```
sudo apt-get install gcc
```
- Clone repo
```
git clone https://github.com/bartooo/x86-bmp-image-rotation.git
```

## Usage <a name = "usage"></a>

- Open `rotation64.c` file
- In `main()` function:
    - Set name of BMP file you want to rotate (line 207)
    ```C
    pInfo = readBMP("<name-of-file>.bmp");	//  Set name of source file
    ```
    - Set name of BMP file where rotated file will be saved
    ```C
    saveBMP(pInfo, "<name-of-file>.bmp", memoryOut);	// Set name of destination file
    ```
- Save `rotation64.c` file
- Go to directory of project
- Compile code
```
sudo make all
```
- If compilation were successful you should see
```
nasm -o copyLines64.o -f elf64 copyLines64.asm
gcc -c -g -O0 rotation64.c
gcc -o rotation64 rotation64.o copyLines64.o
```
- Then run
```
./rotation64
```
- File should be rotated