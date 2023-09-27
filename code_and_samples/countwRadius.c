#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

void paintCross(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y)
{
    for (int xx = x - crossLength; xx <= x + crossLength; xx++)
    { // paints the vertical line of the cross
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
    { // paints the horizontal line of the cross
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

// bit is the bit number, 0-7 from left to right
// The chars is horizontal, so it is from the top to the bottom
// 0
// 1
// 2
// 3
// 4
// 5
// 6
// 7

char aboveBelowRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
    unsigned char united; // united is a char, if this is 1 there is a pixel in the search radius
    char tempY;           // tempY is the y value of the char
    char value;           // The value of the bits to be search in next char
    char mid;
    char midshift;
    if (x < SEARCH_RADIUS || x + SEARCH_RADIUS >= BIT_WIDTH)
    { // If its to close to the left or right edge, return 0, because it is outside of the image
        return 0;
    }

    // To the left of the bit
    if (bit < SEARCH_RADIUS)
    {
        value = SEARCH_RADIUS - bit;
        tempY = y - 1;
        while (tempY != 0)
        {
            if (value == 0)
            {
                break;
            }
            else if (value < 8)
            {
                united = image[x][tempY] << (8 - value);
                midshift = 0;
                break;
            }
            else
            {
                united = image[x][tempY];
                value -= 8;
                tempY--;
            }
        }
    }
    else if (bit > SEARCH_RADIUS)
    {
        midshift = bit - SEARCH_RADIUS;
    }
    else
    {
        midshift = 0;
    }

    // it sets the values in the char back to its place after the bitshift
    mid = (image[x][y] << midshift) >> midshift;

    char bit7 = 7 - bit;

    // To the right of the bit
    if (bit7 < SEARCH_RADIUS)
    {
        value = SEARCH_RADIUS - (bit7);
        tempY = y + 1;
        while (tempY != BIT_WIDTH - 1)
        {
            if (value == 0)
            {
                break;
            }
            else if (value < 8)
            {
                united = image[x][tempY] >> (8 - value);
                midshift = 0;
                break;
            }
            else
            {
                united = image[x][tempY];
                value -= 8;
                tempY++;
            }
        }
    }
    else if (bit7 > SEARCH_RADIUS)
    {
        midshift = bit7 - SEARCH_RADIUS;
    }
    else
    {
        midshift = 0;
    }

    mid = image[x][y] >> midshift;

    return united | mid;
}

// if the return is 0, then there is no white pixel if not there is a white pixel

// It is the row above the bit
char leftRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
    char tempY;
    char value;

    // findes the y value of the char
    if (bit < SEARCH_RADIUS)
    {
        tempY = y - 1;
        value = SEARCH_RADIUS - bit;
        while (value > 8) {
            tempY--;
            value -= 8;
        }
        value--;
    }
    else if (bit >= SEARCH_RADIUS)
    {
        return image[x][y] & (0x80 >> (bit - SEARCH_RADIUS));
    }
    
    // if the char is to close to the top edge, return 0, because it is outside of the image
    if (tempY < 0)
    {
        return 0;
    }

    // returning the bit that is SEARCH_RADIUS away from the bit
    return image[x][tempY] & (0x01 << value);
}

// It is the row above the bit
char rightRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
    char tempY;
    char value;

    // findes the y value of the char
    if (7-bit < SEARCH_RADIUS) // what is left under the bit
    {
        tempY = y + 1;
        value = SEARCH_RADIUS - (7-bit);
        while (value > 8) {
            tempY++;
            value -= 8;
        }
        value--;
    }
    else if (7-bit >= SEARCH_RADIUS)
    {
        return image[x][y] & (0x01 << (7-bit - SEARCH_RADIUS));
    }
    
    // if the char is to close to the top edge, return 0, because it is outside of the image
    if (tempY < 0)
    {
        return 0;
    }

    // returning the bit that is SEARCH_RADIUS away from the bit
    return image[x][tempY] & (0x80 >> value);
}

char checkBox(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
    if (aboveBelowRow(image, x, y, bit) || // If one of the 4 rows around the bit contains a white pixel, return 1 else return 0
        rightRow(image, x, y, bit) ||
        leftRow(image, x, y, bit))
    {
        return 1;
    }
    return 0;
}

// TODO: Make it so it can erase the cell
// not done
void eraseCell(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit)
{
    for (int xx = x - 3; xx <= x + 4; xx++)
    { // goes from 3 chars to the left to 4 chars to the right
        if (xx < 0 && xx >= BMP_WIDTH)
        { // If its to close to the left or the right edge, continue, because it is outside of the image
            continue;
        }
        if (bit == 3)
        { // If the bit is in the middel of the char, just set the char to 0
            image[xx][y] = 0;
        }
        else if (bit < 3)
        {                                      // If the bit is above the middel
            image[xx][y] &= 0xFF >> (5 + bit); // ands the char with 0xFF shifted 5+bit to the right, so the bits in the cell is erased and the other bits are unchanged
            if (y != 0)
            {                                          // If the bit is not in the top row
                image[xx][y - 1] &= 0xFF << (3 - bit); // ands the char with 0xFF shifted 3-bit to the left, -||-
            }
        }
        else
        {                                       // If the bit is below the middel
            image[xx][y] &= 0xFF << (11 - bit); // ands the char with 0xFF shifted 11-bit to the left, -||-
            if (y != BIT_WIDTH - 1)
            {                                          // If the bit is not in the bottom row
                image[xx][y + 1] &= 0xFF >> (bit - 3); // ands the char with 0xFF shifted bit-3 to the right, -||-
            }
        }
    }
}
int count(unsigned char image[BMP_WIDTH][BIT_WIDTH], int counter, unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BIT_WIDTH; y++)
        {
            if (image[x][y])
            { // checks if there is a pixel in the char, through out the hole image
                for (char bit = 0; bit < 8; bit++)
                {
                    if ((image[x][y] >> (7 - bit)) & 0x01)
                    { // finds the first true bit, from left to right
                        if (!checkBox(image, x, y, bit))
                        {                                            // checks if there can be a box around the bit with no white pixels (if there is a white pixel in the box, return 1 so it is negated)
                            paintCross(input_image, x, y * 8 + bit); // paints a cross on the image
                            counter++;                               // adds one to the counter of cells
                            eraseCell(image, x, y, bit);             // erases the cell
                            break;                                   // breaks out of the for loop, so it does not check the rest of the bits
                        }
                    }
                }
            }
        }
    }
    return counter; // returns the counter
}
