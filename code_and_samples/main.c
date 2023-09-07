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

void erode( unsigned char control_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], 
            unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for(int x = 0; x < BMP_WIDTH; x++){
    for (int y = 0; y < BMP_HEIGTH; y++){
      if(checkPixel(control_image[x][y])){
        if(!( ((x==0)         || checkPixel(control_image[x-1][y]))&&
              ((x==BMP_WIDTH) || checkPixel(control_image[x+1][y]))&&
              ((y==0)         || checkPixel(control_image[x][y-1]))&&
              ((y==BMP_HEIGTH)|| checkPixel(control_image[x][y+1])))){
            for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
        }
        else {
          for(int i=0; i<3;i++){
                output_image[x][y][i] = 255;
              }
        }
      }else {
        for(int i=0; i<3;i++){
                output_image[x][y][i] = 0;
              }
      }
    }

  }
}


int countCells( unsigned char control_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
            unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
            unsigned int count,
            unsigned int coords[950][2]){
  int radius = 5;
  int detected = 0;
  //for (int x = 0; x < 5; x++){
  //  for (int y = 0; y < 5; y++){
  //    if(checkPixel(control_image[x][y])){
  //      for(int a = 0; a < x+5; a++){
  //        if(checkPixel(control_image[a][y+5])){
  //
  //        }
  //      }
  //      for(int a = 0; a < y+5; a++){
  //        if(checkPixel(control_image[x+5][a])){
  //          
  //        }
  //      }
  //    }
  //  }
  //  for (int y = 5; y < BMP_HEIGTH-5; y++){
  //
  //  }
  //}
  //for(int y = 0; y < 5; y++){
  //  for(int x = 5; x < BMP_WIDTH-5;x++){
  //
  //  }
  //}
  for(int x = 5; x < BMP_WIDTH-5;x++){
    for(int y = 5; y < BMP_HEIGTH-5;y++){
      detected = 0;
      if(checkPixel(control_image[x][y])){
        for(int a = x-5; a <= x+5; a++){
          if(checkPixel(control_image[a][y+5])){
            detected = 1;
          }
          if(checkPixel(control_image[a][y-5])){
            detected = 1;
          }
        }
        for(int a = y-5; a <= y+5; a++){
          if(checkPixel(control_image[x+5][a])){
            detected = 1;
          }
          if(checkPixel(control_image[x-5][a])){
            detected = 1;
          }
        }
        if(detected){continue;}
        coords[count][0]=x;
        coords[count][1]=y;
        count++;
        for(int a = x-5; a <= x+5;a++){
          for(int b = y-5; b <= y+5; b++){
            for(int i = 0; i < 3; i++){
              control_image[a][b][i] = 0;
              output_image[a][b][i] = 0;
            }
          }
        }
      }
    }
  }
  printf("%d ",count);
  return count;
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
  //erode(control_image,output_image);
  //write_bitmap(output_image, argv[2]);
  //erode(output_image,control_image);
  //write_bitmap(control_image, argv[3]);
  //erode(control_image,output_image);
  //write_bitmap(output_image, argv[4]);
  //erode(output_image,control_image);
  //write_bitmap(control_image, argv[5]);
  //erode(control_image,output_image);
  //write_bitmap(output_image, argv[6]);
  //erode(output_image,control_image);
  //write_bitmap(control_image, argv[7]);
  //erode(control_image,output_image);
  //write_bitmap(output_image, argv[8]);
  //erode(output_image,control_image);
  //write_bitmap(control_image, argv[9]);
  //erode(control_image,output_image);
  int unsigned count=0;
  int unsigned coords[950][2];
  for(int i = 0; i <5; i++){
    erode(output_image,control_image);
    count = countCells(control_image,output_image,count,coords);
    erode(control_image,output_image);
    count = countCells(output_image,control_image,count,coords);
  }
  printf("final count %d",count);
  for(int x=0; x < BMP_WIDTH;x++){
    for(int y = 0; y < BMP_HEIGTH;y++){
      for(int i = 0; i < 3; i++){
        output_image[x][y][i]=input_image[x][y][i];
      }
    }
  }
  write_bitmap(output_image,argv[2]);
  for(int i = 0; i < count; i++){
    for(int a = coords[i][0]-10; a < coords[i][0]+10;a++){
      for(int b = coords[i][1]-3; b < coords[i][1]+3;b++){
      output_image[a][b][1]=0;
      output_image[a][b][2]=0;
    }
    }
    for(int a = coords[i][0]-3; a < coords[i][0]+3;a++){
      for(int b = coords[i][1]-10; b < coords[i][1]+10;b++){
      output_image[a][b][1]=0;
      output_image[a][b][2]=0;
    }
    }
  }
  write_bitmap(output_image,argv[3]);
  return 0;
}
