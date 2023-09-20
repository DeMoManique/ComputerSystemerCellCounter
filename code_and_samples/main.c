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
        // bit shifting value 1 to the left
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

void bitArrayToPicture()
{
}

char erodeBitArray(unsigned char output_image_bit[952][119],
                   unsigned char control_image_bit[952][119])
{
  int boolean = 0;
  // basically a boolean
  for (int x = 1; x < 952; x++)
  {
    for (int y = 0; y < 119; y++)
    {
      if (control_image[x][y])
      {
        boolean = 1;
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
  return boolean;
}

int countCellsBit(unsigned char image[952][119],
                  int count)
{
  char bit;
  for (int x = 1; x < 952; x++)
  {
    for (int y = 0; y < 119; y++)
    {
      // checks if one of the 8 bits is true
      if (image[x][y])
      {
        bit = 7;
        while (bit >= 0)
        {
          // finds the first true bit, from left to right
          if ((image[x][y] >> bit) & 0x01)
          {
            if (y == 0 && bit > 3)
            {
              ((x > 3) && ((image[x - 3][y] & 0x7F) >> (bit - 4)) || (x <= 946) && ((image[x + 4][y] & 0x7F) >> (bit - 4)) || checkTheXAksis(image, x, y, bit))
            }
            else if (y == 118 && bit < 5)
            {
              ((x > 3) && ((image[x - 3][y] & 0xFE) << (4 - bit)) || (x <= 946) && ((image[x + 4][y] & 0xFE) << (4 - bit)) || checkTheXAksis(image, x, y, bit))
            }

            if (bit == 4)
            {
              if (((x > 3) && image[x - 3][y] || (x <= 946) && image[x + 4][y]) || checkTheXAksis(image, x, y, 0x10))
              {
              }
              else
              {
                count++;
                for (int i = -3; i <= 4; i++)
                {
                  image[x + i][y] = 0;
                }
              }
            }
            else if (bit > 4)
            {
              if (((x > 3) && ((image[x - 3][y] >> (bit - 4)) || (y > 0) && (image[x - 3][y - 1] << (12 - bit)))) ||
                  ((x <= 946) && ((image[x + 4][y] >> (bit - 4)) || (y > 0) && (image[x + 4][y - 1] << (12 - bit)))) ||
                  checkTheXAksis(image, x, y, 0x80))
              {
              }
              else
              {
                count++;
              }
            }
            else
            {
              if (((x > 3) && ((image[x - 3][y] << (4 - bit)) || (y < 118) && (image[x - 3][y + 1] >> (4 + bit)))) ||
                  ((x <= 946) && ((image[x + 4][y] << (4 - bit)) || (y < 118) && (image[x + 4][y + 1] >> (4 + bit)))) ||
                  checkTheXAksis(image, x, y, 0x80))
              {
              }
              else
              {
                count++;
              }
            }
          }
          bit--;
        }
      }
    }
  }
  return count;
}

// Function to check if the pixels on x aksis is white
char checkTheXAksis(unsigned char image[952][119], int x, int y, char bit)
{
  char flyt = 0;
  int start = 0;
  char bitstart = 0;
  char bitend = 0;
  char yend = y;

  // if the cell is to close to the edge
  // left
  if (x < 4)
  {
    start = 1;
    flyt = x + 4;
  }
  // right
  else if (x > 946)
  {
    start = 950;
    flyt = (950 - x) + 4;
  }
  // middle
  else
  {
    start = x - 3;
    flyt = 8;
  }

  // bit number
  // 7 6 5 4 3 2 1 0

  // if the cell is close to the edge
  // top
  if (y == 0 && bit > 4)
  {
    bitstart = 0x40;
    bitend = 0x01 << bit - 4;
  }
  // bottom
  else if (y == 118 && bit < 4)
  {
    bitstart = 0x01 << bit + 3;
    bitend = 0x02;
  }
  // middle
  else
  {
    bitstart = 0x01 << ((bit + 3) % 8);
    bitend = 0x01 << ((bit - 4) % 8);
    if (bit > bitstart)
    {
      y - 1;
    }
    if (bit < bitend)
    {
      y + 1;
    }
  }

  // checks if 8 pixels on a row is white, both top and bottom
  for (int i = 0; i < flyt; i++)
  {
    if (y == 0 && bit > 4)
    {
      return (image[start + i][yend] & bitend);
    }
    else if (y == 118 && bit < 4)
    {
      return ((image[start + i][y] & bitstart));
    }
    else
    {
      return ((image[start + i][y] & bitstart)) | (image[start + i][yend] & bitend);
    }
  }
  return 0;
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
    // printf("\n");
  }

  return 0;
}
