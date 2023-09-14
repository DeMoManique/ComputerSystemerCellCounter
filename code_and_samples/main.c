// To compile: gcc cbmp.c main.c -o main.exe -std=c99
// To run: main.exe zimage.bmp output.bmp
// To run template: main.exe "inputImagePath" "outputImagePath"
//  main.exe samples/medium/1MEDIUM.bmp samples/medium/1MEDIUMoutput.bmp samples/medium/1MEDIUMblackwhite.bmp
// main.exe samples/easy/10EASY.bmp samples/easy/10EASYoutput.bmp samples/easy/10EASYblackwhite.bmp

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cbmp.h"

// Declaring the
clock_t start, end;
double cpu_time_used;

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char control_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image_bit[952][119];
unsigned char control_image_bit[952][119];

double otsu(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  
}

// Function to turn image into black and white
void toBlackWhite(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
                  unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
                  int threshold,
                  int thresholdLower)
{
  int sum;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      sum = 0;
      for (int c = 0; c < BMP_CHANNELS; c++)
      { // Sums up the rgb value
        sum += input_image[x][y][c];
      }
      if (sum >= threshold)
      { // checks with higher treshold instead of averaging (is that a word?) values
        for (int i = 0; i < 3; i++)
        {
          output_image[x][y][i] = 255;
        }
      }
      else if (sum >= thresholdLower)
      {
        for (int i = 0; i < 3; i++)
        {
          output_image[x][y][i] = 150;
        }
      }
      else
      {
        for (int i = 0; i < 3; i++)
        {
          output_image[x][y][i] = 0;
        }
      }
    }
  }
}

// Function to turn image into black and white bit array with a true bit all around the image so it is 952x119
void toBlackWhiteBitArray(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
                          unsigned char output_image_bit[952][119],
                          int threshold)
{
  int sum;
  for (int x = 0; x <= BMP_WIDTH + 1; x++)
  {
    if (x == 0 || x == 951)
    {
      for (int y = 0; y < 119; y++)
      {
        output_image_bit[x][y] = 0xFF;
      }
      continue;
    }
    for (int y = 0; y < 119; y++)
    {
      char value = 0;
      for (int bit = 0; bit < 8; bit++)
      {
        value = value << 1;

        if ((y == 0 && bit == 0) || (bit == 7 && y == 118))
        {
          value += 1;
          continue;
        }

        sum = 0;
        for (int c = 0; c < BMP_CHANNELS; c++)
        { // Sums up the rgb value
          sum += input_image[x][(y * 8) + bit][c];
        }

        if (sum >= threshold)
        { // checks with higher treshold instead of averaging values
          value += 1;
        }
      }
      // De to sidste bits er altid 0 fordi 950 ikke er deleligt med 8
      output_image_bit[x][y] = value;
    }
  }
}

// Function to check if pixel is black or white (is not needed but looks pretty)
int checkPixel(unsigned char rgb[BMP_CHANNELS])
{
  return rgb[0];
}

void erodeBitArray(unsigned char output_image_bit[952][119],
                   unsigned char control_image_bit[952][119])
{
  // basically a boolean
  for (int x = 1; x < 952; x++)
  {
    for (int y = 0; y < 119; y++)
    {
      if (y == 0)
      {
        output_image_bit[x][y] = control_image_bit[x][y] &
                                 ((control_image_bit[x][y] << 1) | (control_image_bit[x][y + 1] >> 7)) &
                                 (control_image_bit[x][y] >> 1 | 0x80) &
                                 control_image_bit[x - 1][y] &
                                 control_image_bit[x + 1][y];
      }
      else if (y == 118)
      {
        output_image_bit[x][y] = control_image_bit[x][y] &
                                 ((control_image_bit[x][y] << 1) | 0x01) &
                                 (control_image_bit[x][y] >> 1 | (control_image_bit[x][y - 1] & 0x01)) &
                                 control_image_bit[x - 1][y] &
                                 control_image_bit[x + 1][y];
      }
      else
      {
        output_image_bit[x][y] = control_image_bit[x][y] &
                                 ((control_image_bit[x][y] << 1) | (control_image_bit[x][y + 1] >> 7)) &
                                 (control_image_bit[x][y] >> 1 | (control_image_bit[x][y - 1] & 0x01)) &
                                 control_image_bit[x - 1][y] &
                                 control_image_bit[x + 1][y];
      }
    }
  }
}

int countCellsBit(unsigned char image[952][119],
                  int count)
{
  char bit;
  for (int x = 1; x < 952; x++)
  {
    for (int y = 0; y < 119; y++)
    {
      if (image[x][y])
      {
        bit = 7;
        while (bit >= 0)
        {
          if ((image[x][y] >> bit) & 0x01)
            if (bit == 4)
            {
              image[x - 3][y] & image[x + 4][y];
            }
            else if (bit > 4)
            {
              ((image[x - 3][y] >> (bit - 4)) | (image[x - 3][y - 1] << (12 - bit))) &
                  ((image[x + 4][y] >> (bit - 4)) | (image[x + 4][y - 1] << (12 - bit)));
            }
            else
            {
              ((image[x - 3][y] << (4 - bit)) | (image[x - 3][y + 1] >> (4 + bit))) &
                  ((image[x + 4][y] << (4 - bit)) | (image[x + 4][y + 1] >> (4 + bit)));
            }

          bit--;
        }
      }
    }
  }

  return count;
}

int checkLightPixel(unsigned char rgb[BMP_CHANNELS])
{
  if (rgb[0] == 150)
    return 1;
  return 0;
}

// Function that erodes the outer layer of cells
// Returns int to be used in for loop later
int erode(unsigned char control_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
          unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  int removed = 0; // basically a boolean
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (checkPixel(control_image[x][y]))
      {
        if (checkLightPixel(control_image[x][y]))
        {
          for (int i = 0; i < 3; i++)
          {
            removed = 1;
            output_image[x][y][i] = 0;
          }
        }
        if (!(((x == 0) || checkPixel(control_image[x - 1][y])) && // Checks for edge case first, then checks if above pixel is white
              ((x == BMP_WIDTH - 1) || checkPixel(control_image[x + 1][y])) &&
              ((y == 0) || checkPixel(control_image[x][y - 1])) &&
              ((y == BMP_HEIGTH - 1) || checkPixel(control_image[x][y + 1]))))
        {
          for (int i = 0; i < 3; i++)
          {
            removed = 1;
            output_image[x][y][i] = 0;
          }
        }
      }
    }
  }
  return removed;
}

int erodeFirst(unsigned char control_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
               unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  int removed = 0; // basically a boolean
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (checkLightPixel(control_image[x][y]))
      {
        if (!(((x == 0) || checkPixel(control_image[x - 1][y])) && // Checks for edge case first, then checks if above pixel is white
              ((x == BMP_WIDTH - 1) || checkPixel(control_image[x + 1][y])) &&
              ((y == 0) || checkPixel(control_image[x][y - 1])) &&
              ((y == BMP_HEIGTH - 1) || checkPixel(control_image[x][y + 1]))))
        {
          for (int i = 0; i < 3; i++)
          {
            removed = 1;
            output_image[x][y][i] = 0;
          }
        }
      }
    }
  }
  return removed;
}

// bunch of functions to help scan for cells
int squareCheckLeft(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y, int radius)
{
  for (int a = y - radius; a <= y + radius; a++)
  {
    if ((!(a < 0) && !(a > BMP_HEIGTH) && checkPixel(image[x - radius][a])))
    { // checks for edge cases first, then if the pixel is white
      return 1;
    }
  }
  return 0;
}

int squareCheckRight(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y, int radius)
{
  for (int a = y - radius; a <= y + radius; a++)
  {
    if ((!(a < 0) && !(a > BMP_HEIGTH) && checkPixel(image[x + radius][a])))
    {
      return 1;
    }
  }
  return 0;
}
int squareCheckAbove(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y, int radius)
{
  for (int a = x - radius; a <= x + radius; a++)
  {
    if ((!(a < 0) && !(a > BMP_WIDTH) && checkPixel(image[a][y - radius])))
    {
      return 1;
    }
  }
  return 0;
}

int squareCheckBelow(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y, int radius)
{
  for (int a = x - radius; a <= x + radius; a++)
  {
    if ((!(a < 0) && !(a > BMP_WIDTH) && checkPixel(image[a][y + radius])))
    {
      return 1;
    }
  }
  return 0;
}

// Function that counts cells small enough
int countCells(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
               unsigned int count,
               int coords[950][2],
               int radius)
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (checkPixel(image[x][y]))
      {
        if (((y >= radius) && squareCheckAbove(image, x, y, radius)) || // Checks for edge case, then scans
            ((y < (BMP_HEIGTH - radius)) && squareCheckBelow(image, x, y, radius)) ||
            ((x >= radius) && squareCheckLeft(image, x, y, radius)) ||
            ((x < (BMP_WIDTH - radius)) && squareCheckRight(image, x, y, radius)))
        {
          continue;
        }
        else
        {
          coords[count][0] = x; // keeps track of coordiantes for cells to mark them in output
          coords[count][1] = y;
          count++; // keeps track of number of cells, also used to store coordinates

          for (int a = x - radius; a <= x + radius; a++)
          { // for loop to paint the cell white
            for (int b = y - radius; b <= y + radius; b++)
            {
              if (a >= 0 && a < BMP_WIDTH && b >= 0 && b <= BMP_HEIGTH)
              { // edge cases
                for (int i = 0; i < 3; i++)
                {
                  image[a][b][i] = 0;
                }
              }
            }
          }
        }
      }
    }
  }
  return count;
}

// Main function
int main(int argc, char **argv)
{
  // cross paarameters
  int crossLength = 7;
  int crossWidth = 1;

  // analysis parameters
  int threshold = 330;
  int thresholdLower = 245;
  int searchRadius = 7;

  // needed variables
  int unsigned count = 0;
  int coords[950][2];

  // Load image from file
  read_bitmap(argv[1], input_image);
  printf("%f", otsu(input_image));

  // Converting image to black and white
  toBlackWhite(input_image, output_image, threshold, thresholdLower);
  toBlackWhiteBitArray(input_image, output_image_bit, threshold);
  count = countCells(output_image, count, coords, 13);
  write_bitmap(output_image, argv[3]);

  // copies the modified image to control image
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int i = 0; i < 3; i++)
      {
        control_image[x][y][i] = output_image[x][y][i];
      }
    }
  }

  start = clock();
  while (erodeFirst(control_image, output_image))
  { // while something was eroded
    count = countCells(output_image, count, coords, searchRadius);
    for (int x = 0; x < BMP_WIDTH; x++)
    { // copies the modified image to control image
      for (int y = 0; y < BMP_HEIGTH; y++)
      {
        for (int i = 0; i < 3; i++)
        {
          control_image[x][y][i] = output_image[x][y][i];
        }
      }
    }
  }

  while (erode(control_image, output_image))
  { // while something was eroded
    count = countCells(output_image, count, coords, searchRadius);
    for (int x = 0; x < BMP_WIDTH; x++)
    { // copies the modified image to control image
      for (int y = 0; y < BMP_HEIGTH; y++)
      {
        for (int i = 0; i < 3; i++)
        {
          control_image[x][y][i] = output_image[x][y][i];
        }
      }
    }
  }
  end = clock();

  printf("final count %d", count);

  for (int x = 0; x < BMP_WIDTH; x++)
  { // sets output_image to be the input_image
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int i = 0; i < 3; i++)
      {
        output_image[x][y][i] = input_image[x][y][i];
      }
    }
  }
  // paints the cross
  for (int i = 0; i < count; i++)
  {
    for (int a = coords[i][0] - crossLength; a <= coords[i][0] + crossLength; a++)
    {
      for (int b = coords[i][1] - crossWidth; b <= coords[i][1] + crossWidth; b++)
      {
        if (a >= 0 && a < BMP_WIDTH && b >= 0 && b < BMP_HEIGTH)
        {
          output_image[a][b][0] = 255;
          output_image[a][b][1] = 0;
          output_image[a][b][2] = 0;
        }
      }
    }
    for (int b = coords[i][1] - crossLength; b <= coords[i][1] + crossLength; b++)
    {
      for (int a = coords[i][0] - crossWidth; a <= coords[i][0] + crossWidth; a++)
      {
        if (b >= 0 && b < BMP_HEIGTH && a >= 0 && a < BMP_WIDTH)
        {
          output_image[a][b][0] = 255;
          output_image[a][b][1] = 0;
          output_image[a][b][2] = 0;
        }
      }
    }
  }

  write_bitmap(output_image, argv[2]);
  cpu_time_used = (double)(end - start);

  printf(" Total time: %f ms\n", cpu_time_used * 1000 / CLOCKS_PER_SEC);

  // print the bit array
  for (int x = 0; x < 952; x++)
  {
    for (int y = 0; y < 119; y++)
    {
  //    printf("%d ", output_image_bit[x][y]);
    }
    //printf("\n");
  }

  return 0;
}
