#include "display_artillery.h"
#include "display.h"

#define CLOUD_COLOR display_color565(0xFC, 0xE1, 0x79)

/*
#include <stdio.h>
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned short WORD;

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER {
  WORD bfType;      // specifies the file type
  DWORD bfSize;     // specifies the size in bytes of the bitmap file
  WORD bfReserved1; // reserved; must be 0
  WORD bfReserved2; // reserved; must be 0
  DWORD bfOffBits;  // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;         // specifies the number of bytes required by the struct
  LONG biWidth;         // specifies width in pixels
  LONG biHeight;        // specifies height in pixels
  WORD biPlanes;        // specifies the number of color planes, must be 1
  WORD biBitCount;      // specifies the number of bits per pixel
  DWORD biCompression;  // specifies the type of compression
  DWORD biSizeImage;    // size of image in bytes
  LONG biXPelsPerMeter; // number of pixels per meter in x axis
  LONG biYPelsPerMeter; // number of pixels per meter in y axis
  DWORD biClrUsed;      // number of colors used by the bitmap
  DWORD biClrImportant; // number of colors that are important
} BITMAPINFOHEADER;

#pragma pack(pop)

static uint8_t *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader) {
  FILE *filePtr;                     // our file pointer
  BITMAPFILEHEADER bitmapFileHeader; // our bitmap file header
  uint8_t *bitmapImage;              // store image data
  int imageIdx = 0;                  // image index counter
  uint8_t tempRGB;                   // our swap variable

  // open file in read binary mode
  filePtr = fopen(filename, "rb");
  if (filePtr == NULL)
    return NULL;

  // read the bitmap file header
  fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

  // verify that this is a .BMP file by checking bitmap id
  if (bitmapFileHeader.bfType != 0x4D42) {
    fclose(filePtr);
    return NULL;
  }

  // read the bitmap info header
  fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

  // move file pointer to the beginning of bitmap data
  fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

  // allocate enough memory for the bitmap image data
  bitmapImage = (uint8_t *)malloc(bitmapInfoHeader->biSizeImage);

  // verify memory allocation
  if (!bitmapImage) {
    free(bitmapImage);
    fclose(filePtr);
    return NULL;
  }

  // read in the bitmap image data
  fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, filePtr);

  // make sure bitmap image data was read
  if (bitmapImage == NULL) {
    fclose(filePtr);
    return NULL;
  }

  // swap the R and B values to get RGB (bitmap is BGR)
  for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
    tempRGB = bitmapImage[imageIdx];
    bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
    bitmapImage[imageIdx + 2] = tempRGB;
  }

  // close file and return bitmap image data
  fclose(filePtr);
  return bitmapImage;
}

// const uint8_t *bitmap = Background.bmp;
*/

const unsigned char bitmap[] = {
	// 'Clouds, 96x20px
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
	0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00
};


// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void display_artillery_init() { // Clear the screen
  display_fillScreen(DISPLAY_BLACK);

  //BITMAPINFOHEADER bitmapInfoHeader;
  //uint8_t *bitmap = LoadBitmapFile("Clouds.bmp", &bitmapInfoHeader);
  // display_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
  display_drawBitmap(0,0, bitmap, 96, 20, CLOUD_COLOR);
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.