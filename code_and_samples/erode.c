#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

// function to copy the image to a control image used in erode
void copyArray(unsigned char image[BMP_WIDTH][BIT_WIDTH],
    unsigned char control[BMP_WIDTH][BIT_WIDTH])
{
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BIT_WIDTH; y++) {
            control[x][y] = image[x][y];
        }
    }
}
// Names are a bit misleading, just easier to think of this way
char aboveNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    if (x == 0) { // if its the "top" edge, return all white
        return 0xFF;
    }
    return image[x - 1][y]; // else return the char "above"
}

char belowNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    if (x == BMP_WIDTH - 1) { // if its the "bottom" edge, return all white
        return 0xFF;
    }
    return image[x + 1][y]; // else return the char "below"
}

char LeftNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    // the left neighbor can be put on top by bit shifting to the right
    if (y == 0) { // if its the left most edge, add 1 as most siginficant bit
        return ((image[x][y] >> 1) | 0x80);
    }
    return ((image[x][y] >> 1) | (image[x][y - 1] << 7)); // else add the least significant bit of the last char
}

char RightNeighbor(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    // the right neighbor can be put on top by bit shifting to the left
    if (y == BIT_WIDTH-1) {
        // if its the left most edge, add 1 as least significant bit
        return ((image[x][y] << 1) | 0x01);
    }
    return ((image[x][y] << 1) | (image[x][y + 1] >> 7)); // else add the most siginficant bit of the last char
}

// Check all neighbors in one function, makes erode more readable
unsigned char erodeChar(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y)
{
    return ((aboveNeighbor(image, x, y) & belowNeighbor(image, x, y)) & (LeftNeighbor(image, x, y) & RightNeighbor(image, x, y)));
}

unsigned char erodeModeOne(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y){
    return(aboveNeighbor(image, x, y) & RightNeighbor(image, x, y));
}

unsigned char erodeModeTwo(){
    
}

char erode(unsigned char image[BMP_WIDTH][BIT_WIDTH],
    unsigned char control[BMP_WIDTH][BIT_WIDTH])
{
    copyArray(image, control); //Makes control image be equal to image
    char boolean = 0; // a boolean to return, is 1 if something was removed

    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BIT_WIDTH; y++) {
            if (control[x][y]) {   //if there is a pixel in the char
                image[x][y] &= erodeChar(control, x, y);
                boolean = 1;
            }
        }
    }
    return boolean; //returns 1 or 0
}