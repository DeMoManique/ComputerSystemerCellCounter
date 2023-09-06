//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe zimage.bmp d.bmp e.bmp f.bmp g.bmp h.bmp i.bmp j.bmp k.bmp l.bmp m.bmp n.bmp o.bmp p.bmp q.bmp r.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

void toGreyScale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  int sum;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      sum = 0;
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        sum += input_image[x][y][c];
      }
      if (sum >= 300){
        for(int i = 0; i < 3; i++){
          output_image[x][y][i] = 255;
        }
      } else {
        for(int i = 0; i < 3; i++){
          output_image[x][y][i] = 0;
        }
      }
    }
  }
}
int checkPixel(unsigned char rgb[BMP_CHANNELS]){
  if(rgb[0] == 255) return 1;
  return 0;
}

void erode(unsigned char control_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for(int x = 0; x < BMP_WIDTH; x++){
    for (int y = 0; y < BMP_HEIGTH; y++){
      if(checkPixel(control_image[x][y])){
        if(x == 0){
          if(y == 0){
            if(!(checkPixel(control_image[x+1][y])&&checkPixel(control_image[x][y+1]))){
              for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
            }
          }
          else if(y == BMP_HEIGTH-1){
            if(!(checkPixel(control_image[x+1][y])&&checkPixel(control_image[x][y-1]))){
              for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
            }
          }
          if(!(checkPixel(control_image[x+1][y])&&checkPixel(control_image[x][y+1])&&checkPixel(control_image[x][y-1]))){
              for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
          }
        }
        if(x == BMP_WIDTH-1){
          if(y == 0){
            if(!(checkPixel(control_image[x-1][y])&&checkPixel(control_image[x][y+1]))){
              for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
            }
          }
          if(y == BMP_HEIGTH-1){
            if(!(checkPixel(control_image[x-1][y])&&checkPixel(control_image[x][y-1]))){
              for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
            }
          }
          if(!(checkPixel(control_image[x-1][y])&&checkPixel(control_image[x][y+1])&&checkPixel(control_image[x][y-1]))){
              for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
          }
        }
        if(!(checkPixel(control_image[x+1][y])&&checkPixel(control_image[x-1][y])&&checkPixel(control_image[x][y+1])&&checkPixel(control_image[x][y-1]))){
            for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
        }
        else {
          for(int i=0; i<3;i++){
                output_image[x][y][i] = 255;
              }
        }
      } else {
        for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
      }
    }
  }
}


  //Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char control_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

//Main function
int main(int argc, char** argv)
{
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  //if (argc != 3)
  //{
  //    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
  //    exit(1);
  //}

  printf("Example program - 02132 - A1\n");

  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run inversion
  toGreyScale(input_image,control_image);
  toGreyScale(input_image,output_image);
  erode(control_image,output_image);
  write_bitmap(output_image, argv[2]);
  erode(output_image,control_image);
  write_bitmap(control_image, argv[3]);
  erode(control_image,output_image);
  write_bitmap(output_image, argv[4]);
  erode(output_image,control_image);
  write_bitmap(control_image, argv[5]);
  erode(control_image,output_image);
  write_bitmap(output_image, argv[6]);
  erode(output_image,control_image);
  write_bitmap(control_image, argv[7]);
  erode(control_image,output_image);
  write_bitmap(output_image, argv[8]);
  erode(output_image,control_image);
  write_bitmap(control_image, argv[9]);
  erode(control_image,output_image);
  write_bitmap(output_image, argv[10]);
  erode(output_image,control_image);
  write_bitmap(control_image, argv[11]);
  erode(control_image,output_image);

  return 0;
}
