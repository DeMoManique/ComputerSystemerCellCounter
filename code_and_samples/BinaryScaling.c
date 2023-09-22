#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

// GreyScaling??
void imageGreyScaling(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS] unsigned char greyScaled[BMP_WIDTH][BMP_HEIGTH])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            int greyValue = ((int)image[x][y][0] + (int)image[x][y][1] + (int)image[x][y][2]) / 3;
            greyScaled[x][y] = (char)greyValue;
        }
    }
}
// Otsu
unsigned char otsuThreshold(unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
    int pixels = BMP_HEIGTH * BMP_WIDTH; // amount of pixels in image
    int histogram[256] = {0};            // initialized histogram

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            histogram[image[x][y]]++; // adds 1 pixel to histogram of the pixels value
        }
    }

    unsigned int intensity = 0; // Intensity of Image (pixelValue * amount of pixels with that value)
                                // Example: 4 pixels with greyValue 255 adds intensity (255*4)
    for (int i = 0; i < 256; i++)
    {
        intensity += i * histogram[i];
    }

    double BackgroundIntensity = 0.0;
    int weightBackground = 0;   // The amount of pixels in background
    int weightForeground = 0;   //----------||----------- foreground
    double meanBackround = 0.0; // the average intensity of background
    double meanForeground = 0.0;
    double variance = 0.0;    // current variance, (reCalculated each iteration, could be declared in for loop)
    double maxVariance = 0.0; // keeps track of the best variance
    int threshold = 0;        //----------||----------- threshold

    for (int i = 0; i < 256; i++)
    {
        weightBackground += histogram[i]; // Adds more pixels to background
        if (weightBackground == 0)
            continue; // If there isnt any pixels in background there is no reason to calculate threshold

        weightForeground = pixels - weightBackground;
        if (weightForeground == 0)
            break; // If there are no pixels left in foreground, best threshold should already be found as calculations woulnt change

        BackgroundIntensity += (double)(i * histogram[i]);

        meanBackround = BackgroundIntensity / weightBackground;
        meanForeground = (intensity - BackgroundIntensity) / weightForeground; // we can find Foreground intensity by (totalIntensity-backgroundIntensity)

        variance = ((double)weightBackground * (double)weightForeground *
                    (meanBackround - meanForeground) * (meanBackround - meanForeground)); // wB*wF*(muB*muF)^2

        if (variance > maxVariance) // if a better threshold was found, save it
        {
            maxVariance = variance;
            threshold = i;
        }
    }
    return (threshold);
}

// Convert pixels to bits
// Function to turn image into black and white bit array with a true bit all around the image so it is 952x119
void imageToBits(unsigned char image[BMP_WIDTH][BMP_HEIGTH],
                 unsigned char output[950][119],
                 int threshold)
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < 119; y++)
        {
            if (image[x][y] >= threshold) //If pixel should be white
            {
                output[x][y >> 3] |= 1 << (7-(y & 0x06));   //output[x][y/8] | (1 << (7-y%8))
                                                        //placemnt of pixel 
            }
        }
    }
}

// convert bits to pixels
void printBits(char image[BMP_WIDTH][119], char *path)
{
    unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS] = {0};
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < 119; y++)
        {
            if (image[x][y]) //if there is atleast one 1 in char
            {
                for (int i = 0; i < 8; i++)
                {
                    if ((char >> (7 - i)) & 0x01) //if the current bit is 1
                    {
                        output_image[x][y * 8 + i][0] = 255;
                        output_image[x][y * 8 + i][1] = 255;
                        output_image[x][y * 8 + i][2] = 255;
                    }
                }
            }
        }
    }
    write_bitmap(output_image, path);
}