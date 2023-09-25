#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

char placeholder = 0;

// function to copy the image to a control image used in erode
void copyArray(unsigned char image[BMP_WIDTH][BIT_WIDTH],
               unsigned char control[BMP_WIDTH][BIT_WIDTH])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BIT_WIDTH; y++)
        {
            control[x][y] = image[x][y];
        }
    }
}
// Names are a bit misleading, just easier to think of this way
char aboveNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    if (x == 0)
    { // if its the "top" edge, return all white
        return 0x00;
    }
    return image[x - 1][y]; // else return the char "above"
}

char belowNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    if (x == BMP_WIDTH - 1)
    { // if its the "bottom" edge, return all white
        return 0x00;
    }
    return image[x + 1][y]; // else return the char "below"
}

char leftNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    // the left neighbor can be put on top by bit shifting to the right
    if (y == 0)
    { // if its the left most edge, add 1 as most siginficant bit
        return ((image[x][y] >> 1) | 0x00);
    }
    return ((image[x][y] >> 1) | (image[x][y - 1] << 7)); // else add the least significant bit of the last char
}

char rightNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    // the right neighbor can be put on top by bit shifting to the left
    if (y == BIT_WIDTH - 1)
    {
        // if its the left most edge, add 1 as least significant bit
        return ((image[x][y] << 1) | 0x00);
    }
    return ((image[x][y] << 1) | (image[x][y + 1] >> 7)); // else add the most siginficant bit of the last char
}

char leftAboveNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    if (x == 0)
    { // if its the "top" edge, return all white
        return 0xFF;
    }
    if (y == 0)
    { // if its the left most edge, add 1 as most siginficant bit
        return ((image[x - 1][y] >> 1) | 0x80);
    }
    return ((image[x - 1][y] >> 1) | (image[x - 1][y - 1] << 7)); // else add the least significant bit of the last char
}

char leftBelowNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    if (x == 0)
    { // if its the "top" edge, return all white
        return 0xFF;
    }
    if (y == BIT_WIDTH - 1)
    { // if its the left most edge, add 1 as LSB
        return ((image[x - 1][y] << 1) | 0x01);
    }
    return ((image[x - 1][y] << 1) | (image[x - 1][y + 1] >> 7)); // else add the least significant bit of the last char
}

char rightAboveNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    if (x == BIT_WIDTH - 1)
    { // if its the "bottom" edge, return all white
        return 0xFF;
    }
    if (y == 0)
    { // if its the left most edge, add 1 as most siginficant bit
        return ((image[x + 1][y] >> 1) | 0x80);
    }
    return ((image[x + 1][y] >> 1) | (image[x + 1][y - 1] << 7)); // else add the least significant bit of the last char
}

char rightBelowNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    if (x == BMP_WIDTH - 1)
    { // if its the "bottom" edge, return all white
        return 0xFF;
    }
    if (y == BIT_WIDTH - 1)
    { // if its the left most edge, add 1 as LSB
        return ((image[x + 1][y] << 1) | 0x01);
    }
    return ((image[x + 1][y] << 1) | (image[x + 1][y + 1] >> 7)); // else add the least significant bit of the last char
}

// Check all neighbors in one function, makes erode more readable
unsigned char erodeChar(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    return ((aboveNeighbor(image, x, y) & belowNeighbor(image, x, y)) & (leftNeighbor(image, x, y) & rightNeighbor(image, x, y)));
}

unsigned char erodeModeUpRight(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    return (aboveNeighbor(image, x, y) & rightNeighbor(image, x, y));
}

unsigned char erodeModeDownRight(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    return (belowNeighbor(image, x, y) & rightNeighbor(image, x, y));
}

unsigned char erodeModeUpLeft(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    return (aboveNeighbor(image, x, y) & leftNeighbor(image, x, y));
}

unsigned char erodeModeDownLeft(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    return (belowNeighbor(image, x, y) & leftNeighbor(image, x, y));
}

char switchMode = 1;
char erode(unsigned char image[BMP_WIDTH][BIT_WIDTH],
           unsigned char control[BMP_WIDTH][BIT_WIDTH],
           char mode)
{
    copyArray(image, control); // Makes control image be equal to image
    char boolean = 0;          // a boolean to return, is 1 if something was removed

    switch (mode)
    {
    case 1:
        
        switch (switchMode)
        {
        case 1:
            for (int x = 0; x < BMP_WIDTH; x++)
            {
                for (int y = 0; y < BIT_WIDTH; y++)
                {
                    if (control[x][y])
                    { // if there is a pixel in the char
                        image[x][y] &= erodeModeUpRight(control, x, y);
                        boolean = 1;
                    }
                }
            }
            switchMode = 2;
            break;

        case 2:
            for (int x = 0; x < BMP_WIDTH; x++)
            {
                for (int y = 0; y < BIT_WIDTH; y++)
                {
                    if (control[x][y])
                    { // if there is a pixel in the char
                        image[x][y] &= erodeModeDownRight(control, x, y);
                        boolean = 1;
                    }
                }
            }
            switchMode = 3;
            break;

        case 3:
            for (int x = 0; x < BMP_WIDTH; x++)
            {
                for (int y = 0; y < BIT_WIDTH; y++)
                {
                    if (control[x][y])
                    { // if there is a pixel in the char
                        image[x][y] &= erodeModeDownLeft(control, x, y);
                        boolean = 1;
                    }
                }
            }
            switchMode = 4;
            break;

        case 4:
            for (int x = 0; x < BMP_WIDTH; x++)
            {
                for (int y = 0; y < BIT_WIDTH; y++)
                {
                    if (control[x][y])
                    { // if there is a pixel in the char
                        image[x][y] &= erodeModeUpLeft(control, x, y);
                        boolean = 1;
                    }
                }
            }
            switchMode = 1;
            break;
        }
        break;

    default:
        for (int x = 0; x < BMP_WIDTH; x++)
        {
            for (int y = 0; y < BIT_WIDTH; y++)
            {
                if (control[x][y])
                { // if there is a pixel in the char
                    image[x][y] &= erodeChar(control, x, y);
                    boolean = 1;
                }
            }
        }
        break;
    }

    return boolean; // returns 1 or 0
}
