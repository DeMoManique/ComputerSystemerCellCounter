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
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char control_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image_bit[952][119];
unsigned char control_image_bit[952][119];

void delay(int number_of_seconds)
{
  // Converting time into milli_seconds
  int milli_seconds = 1000 * number_of_seconds;

  // Storing start time
  clock_t start_time = clock();

  // looping till required time is not achieved
  while (clock() < start_time + milli_seconds)
    ;
}

// Main function
int main(int argc, char** argv)
{
  start = clock();
  int threshold;
  //printf("Reading image \n");
  read_bitmap(argv[1], input_image);
  //printf("greyscaling \n");
  unsigned char greyImage[BMP_WIDTH][BMP_HEIGTH];
  imageGreyScaling(input_image, greyImage);
  //printf("calculating threshold: ");
  threshold = otsuThreshold(greyImage);
  //printf("%d \nconverting to bits\n", threshold);
  unsigned char bitImage[BMP_WIDTH][119];

    imageToBits(greyImage, bitImage, threshold-20);

  //printf("printing image\n");
  // printBits(bitImage, output_image);
  // write_bitmap(output_image, argv[2]);
  //printf("Eroding image\n");
  unsigned char controlImage[BMP_WIDTH][119];
  int counter = 0;
  //printBits(bitImage, output_image);
  //write_bitmap(output_image, argv[3]);
  while (erode(bitImage, controlImage)) {
    counter = count(bitImage, counter, input_image);
  }
  write_bitmap(input_image, argv[2]);
  printf("%d\n", counter);
  end = clock();
  cpu_time_used = (double)(end - start);
  printf("Total time: %f ms\n", cpu_time_used * 1000 / CLOCKS_PER_SEC);
  return 0;
}