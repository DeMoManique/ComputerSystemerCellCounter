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

  // reads the iamge
  read_bitmap(argv[1], input_image);
  // printf("greyscaling \n");
  // printf("calculating threshold: ");
  start = clock();
  for (int i = 0; i < 1000; i++) {
    imageGreyScaling(input_image, images.greyImage);
  }
  end = clock();
  double greyTime = (double)(end - start);

  start = clock();
  int threshold = 0;
  for (int i = 0; i < 1000; i++) {
    threshold = otsuThreshold(images.greyImage);
  }
  end = clock();
  double threshTime = (double)(end - start);


  // printf("%d \nconverting to bits\n", threshold);
  start = clock();
  for (int i = 0; i < 1000; i++) {
    imageGreyScaling(input_image, images.greyImage);
    imageToBits(images.greyImage, images.bitImages.bitImage, threshold);
  }
  end = clock();
  double toBitTime = (double)(end - start) - greyTime;
  int counter = 0;
  start = clock();
  for (int i = 0; i < 1000; i++) {
    counter = 0;
    imageGreyScaling(input_image, images.greyImage);
    imageToBits(images.greyImage, images.bitImages.bitImage, threshold);
    counter = countLarge(images.bitImages.bitImage, counter, input_image);
    erode(images.bitImages.bitImage, images.bitImages.controlImage, 0);
    erode(images.bitImages.bitImage, images.bitImages.controlImage, 0);
    counter = countLarge(images.bitImages.bitImage, counter, input_image);
    while (erode(images.bitImages.bitImage, images.bitImages.controlImage, 1)) {
      counter = count(images.bitImages.bitImage, counter, input_image);
    }
  }
  end = clock();
  double erodePlusCount = (double)(end - start) - greyTime - toBitTime;

  for (int i = 0; i < 1000; i++) {
    imageGreyScaling(input_image, images.greyImage);
    imageToBits(images.greyImage, images.bitImages.bitImage, threshold);
    while(erode(images.bitImages.bitImage, images.bitImages.controlImage, 0));
  }
  end = clock();
  double erodeTime = (double)(end - start) - greyTime - toBitTime;

  for (int i = 0; i < 1000; i++) {
    imageGreyScaling(input_image, images.greyImage);
    imageToBits(images.greyImage, images.bitImages.bitImage, threshold);
    while(erode(images.bitImages.bitImage, images.bitImages.controlImage, 1));
  }
  end = clock();
  double erodeTimeTwo = (double)(end - start) - greyTime - toBitTime;


  write_bitmap(input_image, argv[2]);
  printf("grey time: %f ms\n", greyTime * 1000 / CLOCKS_PER_SEC);
  printf("Otsu time: %f ms\n", threshTime * 1000 / CLOCKS_PER_SEC);
  printf("To Bit time: %f ms\n", toBitTime * 1000 / CLOCKS_PER_SEC);
  printf("Erode + count time: %f ms\n", erodePlusCount * 1000 / CLOCKS_PER_SEC);
  printf("erode time: %f ms\n", erodeTime * 1000 / CLOCKS_PER_SEC);
  printf("erode Two time: %f ms\n", erodeTimeTwo * 1000 / CLOCKS_PER_SEC);

  return 0;
}