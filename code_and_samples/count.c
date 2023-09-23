#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include "main.c"

#define crossLength 5
#define crossWidth 1

char checkTheXAksis(unsigned char image[BMP_WIDTH][119], int x, int y, char bit)
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
            // If it is to close to the top
            if ((y == 0 && bit > 3) &&
                (((x > 3) && ((image[x - 3][y] & 0x7F) >> (bit - 4))) ||
                 ((x <= 946) && ((image[x + 4][y] & 0x7F) >> (bit - 4))) ||
                 checkTheXAksis(image, x, y, bit)))
            {
            }
            // If it is to close to the bottom
            else if ((y == 118 && bit < 5) &&
                     (((x > 3) && ((image[x - 3][y] & 0xFE) << (4 - bit))) ||
                      ((x <= 946) && ((image[x + 4][y] & 0xFE) << (4 - bit))) ||
                      checkTheXAksis(image, x, y, bit)))
            {
            }
            // in the middle of the char
            else if (bit == 4)
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
              if (((x > 3) && ((image[x - 3][y] >> (bit - 4)) || (image[x - 3][y - 1] << (12 - bit)))) ||
                  ((x <= 946) && ((image[x + 4][y] >> (bit - 4)) || (image[x + 4][y - 1] << (12 - bit)))) ||
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
              if (((x > 3) && ((image[x - 3][y] << (4 - bit)) || (image[x - 3][y + 1] >> (4 + bit)))) ||
                  ((x <= 946) && ((image[x + 4][y] << (4 - bit)) || (image[x + 4][y + 1] >> (4 + bit)))) ||
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

char aboveRow()
{
}
char belowRow()
{
}
char rightRow()
{
}
char leftRow()
{
}
char checkBox()
{
  return aboveRow() & belowRow() & rightRow() & leftRow();
}
void eraseCell(unsigned char image[BMP_WIDTH][119], int x, int y, char bit)
{
}
int count(unsigned char image[BMP_WIDTH][119], int counter)
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < 119; y++)
    {
      if (image[x][y])
      {
        for (char bit = 0; bit < 8; bit++)
        {
          if ((image[x][y] >> (7 - bit)) & 0x01)
          {
            if (!checkBox())
            {
              paintCross(input_image, x, y * 8 + bit);
              counter++;
              eraseCell(image, x, y, bit);
              // Optimization: break if there are no more 1 bits in char
            }
          }
        }
      }
    }
  }
}

void paintCross(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y)
{
  for (int xx = x - crossLength; xx <= x + crossLength; xx++)
  {
    if (xx >= 0 && xx < BMP_WIDTH)
    {
      for (int yy = y - crossWidth; yy <= y + crossWidth; yy++)
      {
        if (yy >= 0 && yy < BMP_HEIGTH)
        {
          image[xx][yy][0] = 255;
          image[xx][yy][0] = 0;
          image[xx][yy][0] = 0;
        }
      }
    }
  }
  for (int yy = y - crossLength; yy <= y + crossLength; yy++)
  {
    if (yy >= 0 && yy < BMP_HEIGTH)
    {
      for (int xx = x - crossWidth; xx <= x + crossWidth; xx++)
      {
        if (xx >= 0 && xx < BMP_WIDTH)
        {
          image[xx][yy][0] = 255;
          image[xx][yy][0] = 0;
          image[xx][yy][0] = 0;
        }
      }
    }
  }
}