#ifndef CBMP_CBMP_H
#define CBMP_CBMP_H

// Constants
// Array dimensions
#define BMP_WIDTH 950
#define BMP_HEIGTH 950
#define BIT_WIDTH 119
#define BMP_CHANNELS 3

// cross
#define crossLength 5
#define crossWidth 1
// Search radius
#define SEARCH_RADIUS 4


// Public function declarations
void read_bitmap(char * input_file_path, unsigned char output_image_array[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);
void write_bitmap(unsigned char input_image_array[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], char * output_file_path);

#endif // CBMP_CBMP_H
