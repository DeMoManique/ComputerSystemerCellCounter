#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

// function to copy the image to a control image
void copyArray(unsigned char image[BMP_WIDTH][119],
               unsigned char control[BMP_WIDTH][119])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < 119; y++)
        {
            control[x][y] = image[x][y];
        }
    }
}
// Names are a bit misleading, just easier to think of this way
char aboveNeighbor(unsigned char image[BMP_WIDTH][119], int x, int y)
{
    if (x == 0)
    {
        return 0xFF;
    }
    return image[x - 1][y];
}

char belowNeighbor(unsigned char image[BMP_WIDTH][119], int x, int y)
{
    if (x == BMP_WIDTH - 1)
    {
        return 0xFF;
    }
    return image[x + 1][y];
}

char LeftNeighbor(unsigned char image[BMP_WIDTH][119], int x, int y)
{
    if (y == 0)
    {
        return (image[x][y] >> 1) | 0x80;
    }
    return (image[x][y] >> 1) | (image[x][y - 1] << 7);
}

char RightNeighbor(unsigned char image[BMP_WIDTH][119], int x, int y)
{
    if (y == 118)
    {
        return (image[x][y] << 1) | 0x01;
    }
    return (image[x][y] << 1) | (image[x][y + 1] >> 7);
}
// Collect all neighbors in one function to help with clarity in erode function
char erodeChar(unsigned char image[BMP_WIDTH][119], int x, int y)
{
    return aboveNeighbor(image, x, y) & belowNeighbor(image, x, y) & LeftNeighbor(image, x, y) & RightNeighbor(image, x, y);
}

char erodeBitArray(unsigned char image[BMP_WIDTH][119],
                   unsigned char control[BMP_WIDTH][119],
                   char mode)
{
    copyArray(image, control);
    int boolean = 0;

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < 119; y++)
        {
            if (control[x][y])
            {
                output[x][y] = erodeChar(control, x, y);
                boolean = 1;
            }
        }
    }
    return boolean;
}