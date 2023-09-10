//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe zimage.bmp d.bmp e.bmp f.bmp g.bmp h.bmp i.bmp j.bmp k.bmp l.bmp m.bmp n.bmp o.bmp p.bmp q.bmp r.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

  //Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char control_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

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
      if (sum >= 260){
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
        if(!( ((x==0)         || checkPixel(control_image[x-1][y]))&&
              ((x==BMP_WIDTH-1) || checkPixel(control_image[x+1][y]))&&
              ((y==0)         || checkPixel(control_image[x][y-1]))&&
              ((y==BMP_HEIGTH-1)|| checkPixel(control_image[x][y+1])))){
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
int squareCheckLeft(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned int x, unsigned int y, unsigned int radius){
  int length = 0;
  for (int a = y-radius; a<= y+radius;a++){
    if((!(a<0) && !(a>=BMP_HEIGTH) && checkPixel(image[x-radius][a]))){
      return 1;
    } 
  }
  return 0;
}

int squareCheckRight(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned int x, unsigned int y, unsigned int radius){
  for (int a = y-radius; a<= y+radius;a++){
    if((!(a<0) && !(a>=BMP_HEIGTH) && checkPixel(image[x+radius][a]))){
      return 1;
    } 
  }
  return 0;
}
int squareCheckAbove(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned int x, unsigned int y, unsigned int radius){
  for (int a = x-radius; a<= x+radius;a++){
    if((!(a<0) && !(a>=BMP_WIDTH) && checkPixel(image[a][y-radius]))){
      return 1;
    } 
  }
  return 0;
}

int squareCheckBelow(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned int x, unsigned int y, unsigned int radius){
  for (int a = x-radius; a<= x+radius;a++){
    if((!(a<0) && !(a>=BMP_WIDTH) && checkPixel(image[a][y+radius]))){
      return 1;
    } 
  }
  return 0;
}

int countCells( unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
            unsigned int count,
            unsigned int coords[950][2],
            unsigned int radius){
  int detected = 0;
    for (int x = 0; x < BMP_WIDTH; x++){
      for(int y = 0; y < BMP_HEIGTH; y++){
        if(checkPixel(image[x][y])){
          if( ((y>=radius) && squareCheckAbove(image,x,y,radius))||
              ((y<(BMP_HEIGTH-radius)) && squareCheckBelow(image,x,y,radius))||
              ((x>=radius) && squareCheckLeft(image,x,y,radius))||
              ((x<(BMP_WIDTH-radius)) && squareCheckRight(image,x,y,radius))){
                continue;
              }else {
                coords[count][0] = x;
                coords[count][1] = y;
                printf("%d %d\n", x,y);
                count++;
                for(int a = x-radius; a <= x+radius; a++){
                  for(int b = y-radius; b <= y+radius; b++){
                    if(a >= 0 && a < BMP_WIDTH && b>= 0 && b <=BMP_HEIGTH){
                      for(int i = 0; i < 3; i++){
                        image[a][b][i] = 0;
                      }
                    }
                  }
                }
                x+=radius;
              }
           
        }
      }
    }
    return count;
}



//Main function
int main(int argc, char** argv)
{


  //Load image from file
  printf("loading image\n");
  read_bitmap(argv[1], input_image);

  //Run inversion
  printf("GreyScaling\n");
  toGreyScale(input_image,output_image);

  int unsigned count=0;
  int unsigned coords[950][2];
  write_bitmap(output_image, argv[4]);
  printf("Beginning erode\n");
  for(int i = 0; i <10; i++){
    printf("Copying image\n");
    for(int x = 0; x < BMP_WIDTH; x++){
      for (int y = 0; y < BMP_HEIGTH; y++){
        for(int i = 0; i < 3; i++){
          control_image[x][y][i]=output_image[x][y][i];
        }
      }
    }
    printf("Eroding\n");
    erode(control_image,output_image);
    printf("counting\n");
    count = countCells(output_image,count,coords,10);
  }
  printf("finished eroding\n");
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
    for(int a = coords[i][0]-5; a <= coords[i][0]+5;a++){
      if(a>=0 && a < BMP_WIDTH){
        output_image[a][coords[i][1]][0]=255;
        output_image[a][coords[i][1]][1]=0;
        output_image[a][coords[i][1]][2]=0;
      }
      
    }
      for(int b = coords[i][1]-5; b <= coords[i][1]+5;b++){
        if(b >= 0&& b < BMP_HEIGTH){
          output_image[coords[i][0]][b][0]=255;
          output_image[coords[i][0]][b][1]=0;
          output_image[coords[i][0]][b][2]=0;
        }
    }
  }
  write_bitmap(output_image,argv[3]);
  return 0;
}
