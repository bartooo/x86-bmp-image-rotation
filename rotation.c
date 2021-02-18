#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

extern void copyLines(unsigned char* copywritePtr, unsigned char* readPtr, int rem, int imageHeight, int lineBytes);

typedef struct
{
	unsigned short bfType; 
	unsigned long  bfSize; 		// size of file in bytes
	unsigned short bfReserved1; 
	unsigned short bfReserved2; 
	unsigned long  bfOffBits; 	// offset in bytes from header to pixels
	unsigned long  biSize; 		// specifies the number of bytes required by the struct
	long  biWidth; 				// width in pixels
	long  biHeight; 			// height in pixels
	short biPlanes; 
	short biBitCount; 
	unsigned long  biCompression; 
	unsigned long  biSizeImage; // size of file in bytes
	long biXPelsPerMeter; 
	long biYPelsPerMeter; 
	unsigned long  biClrUsed; 
	unsigned long  biClrImportant;
	unsigned long  RGBQuad_0;
	unsigned long  RGBQuad_1;
} bmpHdr;

typedef struct
{
	int width, height;		// width and height of image
	unsigned char* pImg;	// pointer to top left corner
	int cX, cY;				// current coordinates
	int col;				// current color
} imgInfo;

void* freeResources(FILE* pFile, void* pFirst, void* pSnd)
{
	if (pFile != 0)
		fclose(pFile);
	if (pFirst != 0)
		free(pFirst);
	if (pSnd !=0)
		free(pSnd);
	return 0;
}

imgInfo* readBMP(const char* fname)
{
	imgInfo* pInfo = 0;
	FILE* fbmp = 0;
	bmpHdr bmpHead;
	int lineBytes, y;
	unsigned long imageSize = 0;
	unsigned char* ptr;

	pInfo = 0;
	// open file in read binary mode
	fbmp = fopen(fname, "rb");
	if (fbmp == 0)
    {
        printf("error in opening file\n");
        return 0;
    }
    
	// read the bitmap file header
	fread((void *) &bmpHead, sizeof(bmpHead), 1, fbmp);
	// some basic checks
	if (bmpHead.bfType != 0x4D42 || bmpHead.biPlanes != 1 ||
		bmpHead.biBitCount != 1 ||
		(pInfo = (imgInfo *) malloc(sizeof(imgInfo))) == 0)
        {
            printf("error in basic checks\n");
		    return (imgInfo*) freeResources(fbmp, pInfo->pImg, pInfo);
        }
    
	pInfo->width = bmpHead.biWidth;
	pInfo->height = bmpHead.biHeight;
	imageSize = (((pInfo->width + 31) >> 5) << 2) * pInfo->height;

	if ((pInfo->pImg = (unsigned char*) malloc(imageSize)) == 0)
    {
        printf("error in allocation\n");
		return (imgInfo*) freeResources(fbmp, pInfo->pImg, pInfo);
    }

	// process height (it can be negative)
	ptr = pInfo->pImg;
	lineBytes = ((pInfo->width + 31) >> 5) << 2; // line size in bytes
	if (pInfo->height > 0)
	{
		// "upside down", bottom of the image first
		ptr += lineBytes * (pInfo->height - 1);
		lineBytes = -lineBytes;
	}
	else
		pInfo->height = -pInfo->height;

	// reading image
	// moving to the proper position in the file
	if (fseek(fbmp, bmpHead.bfOffBits, SEEK_SET) != 0)
    {
        printf("error in moving to the beggining of data\n");
		return (imgInfo*) freeResources(fbmp, pInfo->pImg, pInfo);
    }

	for (y=0; y < pInfo->height; ++y)
	{
		fread(ptr, 1, abs(lineBytes), fbmp);
		ptr += lineBytes;
	}
	fclose(fbmp);
	return pInfo;
}

unsigned char* allocateMem(const imgInfo* pInfo)
{
    int colBytes = ((pInfo->height + 31) >> 5) << 2;
    unsigned char* memory = (unsigned char*) malloc(colBytes * pInfo->width);
	return memory;
}

void rotate(const imgInfo* pInfo, unsigned char *writePtr)
{
    int lineBytes = ((pInfo->width + 31) >> 5) << 2;
    int colBytes = ((pInfo->height + 31) >> 5) << 2;
    unsigned char *readPtr;
    unsigned char *copywritePtr = writePtr;
	int rem;
    int readBytesCol = 0;
    int imageHeight = pInfo->height;

    int w = 0;
    while (w < pInfo->width)
    {
        readPtr = pInfo->pImg + lineBytes * (pInfo->height - 1);
        copywritePtr = writePtr + (w * colBytes);
        readPtr += readBytesCol;

		rem = (w % 8);
        copyLines(copywritePtr, readPtr, rem, imageHeight, lineBytes);

        w++;
        if ((w % 8) == 0)
        {
            readBytesCol++;
        }
    }
}

int saveBMP(const imgInfo* pInfo, const char* fname, unsigned char *writePtr)
{
	int lineBytes = ((pInfo->width + 31) >> 5) << 2;
    int colBytes = ((pInfo->height + 31) >> 5) << 2;
	bmpHdr bmpHead = 
	{
	0x4D42,				// unsigned short bfType; 
	sizeof(bmpHdr),		// unsigned long  bfSize; 
	0, 0,				// unsigned short bfReserved1, bfReserved2; 
	sizeof(bmpHdr),		// unsigned long  bfOffBits; 
	40,					// unsigned long  biSize; 
	pInfo->height,		// long  biWidth; 
	pInfo->width,		// long  biHeight; 
	1,					// short biPlanes; 
	1,					// short biBitCount; 
	0,					// unsigned long  biCompression; 
	colBytes* pInfo->width,	// unsigned long  biSizeImage; 
	11811,				// long biXPelsPerMeter; = 300 dpi
	11811,				// long biYPelsPerMeter; 
	2,					// unsigned long  biClrUsed; 
	0,					// unsigned long  biClrImportant;
	0x00000000,			// unsigned long  RGBQuad_0;
	0x00FFFFFF			// unsigned long  RGBQuad_1;
	};

	FILE * fbmp;
	unsigned char *ptr;
	int y;

	if ((fbmp = fopen(fname, "wb")) == 0)
    {
        printf("error in opening file");
		return -1;
    }

	if (fwrite(&bmpHead, sizeof(bmpHdr), 1, fbmp) != 1)
	{
		fclose(fbmp);
		return -2;
	}

    ptr = writePtr + colBytes * (pInfo->width - 1);
	for (y = (pInfo->width); y > 0; --y, ptr -= colBytes)
		if (fwrite(ptr, sizeof(unsigned char), colBytes, fbmp) != colBytes)
		{
			fclose(fbmp);
			return -3;
		}
	fclose(fbmp);
	return 0;
}


int main(int argc, char* argv[])
{
	imgInfo* pInfo;
	
	pInfo = readBMP("kitty.bmp");
    unsigned char* memoryOut = allocateMem(pInfo);
    rotate(pInfo, memoryOut);
    saveBMP(pInfo, "out.bmp", memoryOut);
	return 0;
}