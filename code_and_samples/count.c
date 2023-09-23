#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

#define crossLength 5
#define crossWidth 1
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
          image[xx][yy][1] = 0;
          image[xx][yy][2] = 0;
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
          image[xx][yy][1] = 0;
          image[xx][yy][2] = 0;
        }
      }
    }
  }
}
// char checkTheXAksis(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
// {
//   char flyt = 0;
//   int start = 0;
//   char bitstart = 0;
//   char bitend = 0;
//   char yend = y;

//   // if the cell is to close to the edge
//   // left
//   if (x < 4)
//   {
//     start = 1;
//     flyt = x + 4;
//   }
//   // right
//   else if (x > 946)
//   {
//     start = 950;
//     flyt = (950 - x) + 4;
//   }
//   // middle
//   else
//   {
//     start = x - 3;
//     flyt = 8;
//   }

//   // bit number
//   // 7 6 5 4 3 2 1 0

//   // if the cell is close to the edge
//   // top
//   if (y == 0 && bit > 4)
//   {
//     bitstart = 0x40;
//     bitend = 0x01 << bit - 4;
//   }
//   // bottom
//   else if (y == 118 && bit < 4)
//   {
//     bitstart = 0x01 << bit + 3;
//     bitend = 0x02;
//   }
//   // middle
//   else
//   {
//     bitstart = 0x01 << ((bit + 3) % 8);
//     bitend = 0x01 << ((bit - 4) % 8);
//     if (bit > bitstart)
//     {
//       y - 1;
//     }
//     if (bit < bitend)
//     {
//       y + 1;
//     }
//   }

//   // checks if 8 pixels on a row is white, both top and bottom
//   for (int i = 0; i < flyt; i++)
//   {
//     if (y == 0 && bit > 4)
//     {
//       return (image[start + i][yend] & bitend);
//     }
//     else if (y == 118 && bit < 4)
//     {
//       return ((image[start + i][y] & bitstart));
//     }
//     else
//     {
//       return ((image[start + i][y] & bitstart)) | (image[start + i][yend] & bitend);
//     }
//   }
//   return 0;
// }
// int countCellsBit(unsigned char image[952][BIT_WIDTH],
//                   int count)
// {
//   char bit;
//   for (int x = 1; x < 952; x++)
//   {
//     for (int y = 0; y < BIT_WIDTH; y++)
//     {
//       // checks if one of the 8 bits is true
//       if (image[x][y])
//       {
//         bit = 7;
//         while (bit >= 0)
//         {
//           // finds the first true bit, from left to right
//           if ((image[x][y] >> bit) & 0x01)
//           {
//             // If it is to close to the top
//             if ((y == 0 && bit > 3) &&
//                 (((x > 3) && ((image[x - 3][y] & 0x7F) >> (bit - 4))) ||
//                  ((x <= 946) && ((image[x + 4][y] & 0x7F) >> (bit - 4))) ||
//                  checkTheXAksis(image, x, y, bit)))
//             {
//             }
//             // If it is to close to the bottom
//             else if ((y == 118 && bit < 5) &&
//                      (((x > 3) && ((image[x - 3][y] & 0xFE) << (4 - bit))) ||
//                       ((x <= 946) && ((image[x + 4][y] & 0xFE) << (4 - bit))) ||
//                       checkTheXAksis(image, x, y, bit)))
//             {
//             }
//             // in the middle of the char
//             else if (bit == 4)
//             {
//               if (((x > 3) && image[x - 3][y] || (x <= 946) && image[x + 4][y]) || checkTheXAksis(image, x, y, 0x10))
//               {
//               }
//               else
//               {
//                 count++;
//                 for (int i = -3; i <= 4; i++)
//                 {
//                   image[x + i][y] = 0;
//                 }
//               }
//             }
//             else if (bit > 4)
//             {
//               if (((x > 3) && ((image[x - 3][y] >> (bit - 4)) || (image[x - 3][y - 1] << (12 - bit)))) ||
//                   ((x <= 946) && ((image[x + 4][y] >> (bit - 4)) || (image[x + 4][y - 1] << (12 - bit)))) ||
//                   checkTheXAksis(image, x, y, 0x80))
//               {
//               }
//               else
//               {
//                 count++;
//               }
//             }
//             else
//             {
//               if (((x > 3) && ((image[x - 3][y] << (4 - bit)) || (image[x - 3][y + 1] >> (4 + bit)))) ||
//                   ((x <= 946) && ((image[x + 4][y] << (4 - bit)) || (image[x + 4][y + 1] >> (4 + bit)))) ||
//                   checkTheXAksis(image, x, y, 0x80))
//               {
//               }
//               else
//               {
//                 count++;
//               }
//             }
//           }
//           bit--;
//         }
//       }
//     }
//   }
//   return count;
// }

char aboveRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
  if (x < 3)
  {
    return 0;
  }
  if (bit == 3)
  {
    return image[x - 3][y];
  }
  else if (bit < 3)
  {
    if (y == 0)
    {
      return (image[x - 3][y] >> (3 - bit));
    }
    return ((image[x - 3][y] >> (3 - bit)) | 
            (image[x - 3][y - 1] << (5 + bit)));
  }
  else
  {
    if (y == BIT_WIDTH - 1)
    {
      return (image[x - 3][y] << (bit - 3));
    }
    return ((image[x - 3][y] << (bit - 3)) | (image[x - 3][y + 1] >> (11 - bit)));
  }
}
char belowRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
  if (x >= BMP_WIDTH - 4)
  {
    return 0;
  }
  if (bit == 3)
  {
    return image[x + 4][y];
  }
  else if (bit < 3)
  {
    if (y == 0)
    {
      return (image[x + 4][y] >> (3 - bit));
    }
    return ((image[x + 4][y] >> (3 - bit)) | 
            (image[x + 4][y - 1] << (5 + bit)));
  }
  else
  {
    if (y == BIT_WIDTH - 1)
    {
      return (image[x + 4][y] << (bit - 3));
    }
    return ((image[x + 4][y] << (bit - 3)) | (image[x + 4][y + 1] >> (11 - bit)));
  }
}
char leftRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
  if (y == 0 && bit < 3)
  {
    return 0;
  }
  if (bit < 3)
  {
    for (int xx = x - 3; xx <= x + 4; xx++)
    {
      if (xx < 0 && xx >= BMP_WIDTH)
      {
        continue;
      }
      if ((image[xx][y - 1] >> (2 - bit)) & 0x01)
      {
        return 1;
      }
    }
  }
  else
  {
    for (int xx = x - 3; xx <= x + 4; xx++)
    {
      if (xx < 0 && xx >= BMP_WIDTH)
      {
        continue;
      }
      if ((image[xx][y] >> (bit - 3)) & 0x80)
      {
        return 1;
      }
    }
  }
  return 0;
}
char rightRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
  if (y == BIT_WIDTH - 1 && bit > 3)
  {
    return 0;
  }
  if (bit > 3)
  {
    for (int xx = x - 3; xx <= x + 4; xx++)
    {
      if (xx < 0 && xx >= BMP_WIDTH)
      {
        continue;
      }
      if ((image[xx][y + 1] << (bit - 4)) & 0x80)
      {
        return 1;
      }
    }
  }
  else
  {
    for (int xx = x - 3; xx <= x + 4; xx++)
    {
      if (xx < 0 && xx >= BMP_WIDTH)
      {
        continue;
      }
      if ((image[xx][y] >> (3 - bit)) & 0x01)
      {
        return 1;
      }
    }
  }
  return 0;
}

char checkBox(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
  if (aboveRow(image, x, y, bit) ||
      belowRow(image, x, y, bit) ||
      rightRow(image, x, y, bit) ||
      leftRow(image, x, y, bit))
  {
    return 1;
  }
  return 0;
}
void eraseCell(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
  for (int xx = x - 3; xx <= x + 4; xx++)
  {
    if (xx < 0 && xx >= BMP_WIDTH)
    {
      continue;
    }
    if (bit == 3)
    {
      image[xx][y] = 0;
    }
    else if (bit < 3)
    {
      image[xx][y] &= 0xFF >> (5 + bit);
      if (y != 0)
      {
        image[xx][y - 1] &= 0xFF << (3 - bit);
      }
    }
    else
    {
      image[xx][y] &= 0xFF << (11 - bit);
      if (y != BIT_WIDTH - 1)
      {
        image[xx][y + 1] &= 0xFF >> (bit - 3);
      }
    }
  }
}
int count(unsigned char image[BMP_WIDTH][BIT_WIDTH], int counter,unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BIT_WIDTH; y++)
    {
      if (image[x][y])
      {
        for (char bit = 0; bit < 8; bit++)
        {
          if ((image[x][y] >> (7 - bit)) & 0x01)
          {
            if (!checkBox(image, x, y, bit))
            {
              paintCross(input_image, x, y * 8 + bit);
              counter++;
              eraseCell(image, x, y, bit);
              break;
            }
          }
        }
      }
    }
  }
  return counter;
}

