// To compile: gcc cbmp.c main.c -o main.exe -std=c99
// To run: main.exe zimage.bmp output.bmp
// To run template: main.exe "inputImagePath" "outputImagePath"
//  main.exe samples/medium/1MEDIUM.bmp samples/medium/1MEDIUMoutput.bmp samples/medium/1MEDIUMblackwhite.bmp
// main.exe samples/easy/10EASY.bmp samples/easy/10EASYoutput.bmp samples/easy/10EASYblackwhite.bmp

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cbmp.h"
#include "conversion.c"
#include "erode.c"
#include "count.c"

// Declaring the
clock_t start, end;
double cpu_time_used;

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
union images
{
  unsigned char greyImage[BMP_WIDTH][BMP_HEIGTH];
  struct bitImages
  {
    unsigned char bitImage[BMP_WIDTH][BIT_WIDTH];
    unsigned char controlImage[BMP_WIDTH][BIT_WIDTH];
  } bitImages;
} images;

void delay(int number_of_seconds) {
  // Converting time into milli_seconds
  int milli_seconds = 1000 * number_of_seconds;

  // Storing start time
  clock_t start_time = clock();

  // looping till required time is not achieved
  while (clock() < start_time + milli_seconds);
}

// Main function
int main(int argc, char** argv) {

  printf("tvunget plads %d alt plads %d", sizeof(input_image),sizeof(images));

  return 0;
}