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
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]; // skal fjernes
union images
{
  unsigned char greyImage[BMP_WIDTH][BMP_HEIGTH];
  struct bitImages
  {
    unsigned char bitImage[BMP_WIDTH][BIT_WIDTH];
    unsigned char controlImage[BMP_WIDTH][BIT_WIDTH];
  } bitImages;
} images;

void delay(int number_of_seconds)
{
  // Converting time into milli_seconds
  int milli_seconds = 1000 * number_of_seconds;

  // Storing start time
  clock_t start_time = clock();

  // looping till required time is not achieved
  while (clock() < start_time + milli_seconds);
}

// Main function
int main(int argc, char **argv)
{

  // reads the iamge
  read_bitmap(argv[1], input_image);
  // printf("greyscaling \n");
  imageGreyScaling(input_image, images.greyImage);
  // printf("calculating threshold: ");
  int threshold = otsuThreshold(images.greyImage);
  // printf("%d \nconverting to bits\n", threshold);

  imageToBits(images.greyImage, images.bitImages.bitImage, threshold);
  start = clock();
  int counter = countLarge(images.bitImages.bitImage, counter, input_image);
  erode(images.bitImages.bitImage, images.bitImages.controlImage, 0);
  erode(images.bitImages.bitImage, images.bitImages.controlImage, 0);
  while (erode(images.bitImages.bitImage, images.bitImages.controlImage, 1))
  {
    counter = count(images.bitImages.bitImage, counter, input_image);

  }
  end = clock();
  write_bitmap(input_image, argv[2]);
  printf("%d\n", counter);
  cpu_time_used = (double)(end - start);
  printf("Total time: %f ms\n", cpu_time_used * 1000 / CLOCKS_PER_SEC);
  //printf("bytes: %d", (sizeof(images)+sizeof(input_image)));
  return 0;
}