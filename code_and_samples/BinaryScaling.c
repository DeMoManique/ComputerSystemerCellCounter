#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

// Otsu
unsigned char otsuThreshold(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    int pixels = BMP_HEIGTH * BMP_WIDTH;
    int histogram[256] = {0};

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            histogram[(image[x][y][0] + image[x][y][1] + image[x][y][2]) / 3]++;
        }
    }

    unsigned int intensity = 0; // Can be thought of as image intensity

    for (int i = 0; i < 256; i++)
    {
        intensity += i * histogram[i];
    }

    double BackgroundIntensity = 0.0;
    int weightBackground = 0; // The amount of pixels in background
    int weightForeground = 0; //----------||----------- foreground
    double meanBackround = 0.0;
    double meanForeground = 0.0;
    double variance = 0.0;
    double maxVariance = 0.0; // keeps track of the best variance
    int threshold = 0;        //----------||----------- threshold

    for (int i = 0; i < 256; i++)
    {
        weightBackground += histogram[i];
        if (weightBackground == 0)
            continue; // If there isnt any pixels in background there is no reason to calculate threshold

        weightForeground = pixels - weightBackground;
        if (weightForeground == 0)
            break; // If there are no pixels left in foreground, best threshold should already be found

        BackgroundIntensity += (double)(i * histogram[i]);

        meanBackround = BackgroundIntensity / weightBackground;
        meanForeground = (intensity - BackgroundIntensity) / weightForeground; 

        variance =  ((double)weightBackground * (double)weightForeground * 
                    (meanBackround - meanForeground) * (meanBackround - meanForeground));

        if (variance > maxVariance)
        {
            maxVariance = variance;
            threshold = i;
        }
    }
    return (threshold);
}

// Convert pixels to bits

// convert bits to pixels