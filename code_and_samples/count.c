#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

#define crossLength 5
#define crossWidth 1
void paintCross(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y) {
  for (int xx = x - crossLength; xx <= x + crossLength; xx++) { // paints the vertical line of the cross
    if (xx >= 0 && xx < BMP_WIDTH) {
      for (int yy = y - crossWidth; yy <= y + crossWidth; yy++) {
        if (yy >= 0 && yy < BMP_HEIGTH) {
          image[xx][yy][0] = 255;
          image[xx][yy][1] = 0;
          image[xx][yy][2] = 0;
        }
      }
    }
  }
  for (int yy = y - crossLength; yy <= y + crossLength; yy++) { // paints the horizontal line of the cross
    if (yy >= 0 && yy < BMP_HEIGTH) {
      for (int xx = x - crossWidth; xx <= x + crossWidth; xx++) {
        if (xx >= 0 && xx < BMP_WIDTH) {
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

// if the return is 0, then there is no white pixel if not there is a white pixel

// It is the row to left of the bit
char aboveRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  if (x < 3) {  // If its to close to the left edge, return 0, because it is outside of the image
    return 0;
  }
  if (bit == 3) { // If the bit is in the middel of the char, just return the char 3 to the left
    return image[x - 3][y];
  }
  else if (bit < 3) { // If the bit is above the middel
    if (y == 0) { // There is no char above, so it will return the char, with the bits, under 4 bits below the detected bit, bitshiftet away, 3 chars to the left
      return (image[x - 3][y] >> (3 - bit));
    } // Returns the 4 bits below the bit and 3 bits above the bit, 3 chars to the left
    return ((image[x - 3][y] >> (3 - bit)) |
      (image[x - 3][y - 1] << (5 + bit)));
  }
  else { // If the bit is below the middel
    if (y == BIT_WIDTH - 1) { //There is no char below, return the char, with the bits, over 3 bits above the detected bit, bitshiftet away, 3 chars to the left
      return (image[x - 3][y] << (bit - 3));
    } // Returns the 3 bits above the detected bit and 4 bits below the detected bit, 3 chars to the left
    return ((image[x - 3][y] << (bit - 3)) | (image[x - 3][y + 1] >> (11 - bit)));
  }
}

// It is the row to the right of the bit
char belowRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  if (x >= BMP_WIDTH - 4) { // If its to close to the right edge, return 0, because it is outside of the image
    return 0;
  }
  if (bit == 3) { // If the bit is in the middel of the char, just return the char 4 chars to the right
    return image[x + 4][y];
  }
  // Same as aboveRow, but with the row to the right
  else if (bit < 3) { // If the bit is above of the middel
    if (y == 0) {
      return (image[x + 4][y] >> (3 - bit));
    }
    return ((image[x + 4][y] >> (3 - bit)) |
      (image[x + 4][y - 1] << (5 + bit)));
  }
  else {  // If the bit is below the middel
    if (y == BIT_WIDTH - 1) {
      return (image[x + 4][y] << (bit - 3));
    }
    return ((image[x + 4][y] << (bit - 3)) | (image[x + 4][y + 1] >> (11 - bit)));
  }
}

// It is the row above the bit
char leftRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  if (y == 0 && bit < 3) { // If its to close to the top edge, return 0, because it is outside of the image
    return 0;
  }
  if (bit < 3) { // The bit is above the middel
    for (int xx = x - 3; xx <= x + 4; xx++) { // goes from 3 chars to the left to 4 chars to the right
      if (xx < 0 && xx >= BMP_WIDTH) {  // If its to close to the left or the right edge, continue, because it is outside of the image
        continue;
      }
      if ((image[xx][y - 1] >> (2 - bit)) & 0x01) { // bitshift the char (2-bit) to the right, and checks if the LSB bit is 1 in the char above
        return 1;
      }
    }
  }
  else { // The bit is below the middel
    for (int xx = x - 3; xx <= x + 4; xx++) { // goes from 3 chars to the left to 4 chars to the right
      if (xx < 0 && xx >= BMP_WIDTH) {
        continue;
      }
      if ((image[xx][y] << (bit - 3)) & 0x80) { // bitshift the char (bit-3) to the left, and checks if the MSB bit is 1

        return 1;
      }
    }
  }
  return 0;
}
char rightRow(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  if (y == BIT_WIDTH - 1 && bit > 3) { // If its to close to the bottom edge, return 0, because it is outside of the image
    return 0;
  }
  if (bit > 3) { // The bit is below the middel
    for (int xx = x - 3; xx <= x + 4; xx++) { // goes from 3 chars to the left to 4 chars to the right
      if (xx < 0 && xx >= BMP_WIDTH) {  // If its to close to the left or the right edge, continue, because it is outside of the image
        continue;
      }
      if ((image[xx][y + 1] << (bit - 4)) & 0x80) { // bitshift the char (bit-4) to the left, and checks if the MSB bit is 1 in the char below
        return 1;
      }
    }
  }
  else {
    for (int xx = x - 3; xx <= x + 4; xx++) { // goes from 3 chars to the left to 4 chars to the right
      if (xx < 0 && xx >= BMP_WIDTH) { // If its to close to the left or the right edge, continue, because it is outside of the image
        continue;
      }
      if ((image[xx][y] >> (3 - bit)) & 0x01) { // bitshift the char (3-bit) to the right, and checks if the LSB bit is 1
        return 1;
      }
    }
  }
  return 0;
}

char checkBox(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  if (aboveRow(image, x, y, bit) || // If one of the 4 rows around the bit contains a white pixel, return 1 else return 0
    belowRow(image, x, y, bit) ||
    rightRow(image, x, y, bit) ||
    leftRow(image, x, y, bit)) {
    return 1;
  }
  return 0;
}
void eraseCell(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  if (y >= BIT_WIDTH || y < 0) return;
  for (int xx = x - 3; xx <= x + 4; xx++) { // goes from 3 chars to the left to 4 chars to the right
    if (xx < 0 && xx >= BMP_WIDTH) { // If its to close to the left or the right edge, continue, because it is outside of the image
      continue;
    }
    if (bit == 3) { // If the bit is in the middel of the char, just set the char to 0
      image[xx][y] = 0;

    }
    else if (bit < 3 && y >= 0) { // If the bit is above the middel
      image[xx][y] &= 0xFF >> (5 + bit);  // ands the char with 0xFF shifted 5+bit to the right, so the bits in the cell is erased and the other bits are unchanged
      if (y > 0) { // If the bit is not in the top row
        image[xx][y - 1] &= 0xFF << (3 - bit); // ands the char with 0xFF shifted 3-bit to the left, -||-
      }
    }
    else if (y >= 0) { // If the bit is below the middel
      image[xx][y] &= 0xFF << (11 - bit); // ands the char with 0xFF shifted 11-bit to the left, -||-
      if (y < BIT_WIDTH - 1) { // If the bit is not in the bottom row
        image[xx][y + 1] &= 0xFF >> (bit - 3); // ands the char with 0xFF shifted bit-3 to the right, -||-
      }
    }
  }
}
int count(unsigned char image[BMP_WIDTH][BIT_WIDTH], int counter, unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BIT_WIDTH; y++) {
      if (image[x][y]) { // checks if there is a pixel in the char, through out the hole image
        for (char bit = 0; bit < 8; bit++) {
          if ((image[x][y] >> (7 - bit)) & 0x01) { // finds the first true bit, from left to right
            if (!checkBox(image, x, y, bit)) { // checks if there can be a box around the bit with no white pixels (if there is a white pixel in the box, return 1 so it is negated)
              paintCross(input_image, x, y * 8 + bit); // paints a cross on the image
              counter++; // adds one to the counter of cells
              eraseCell(image, x, y, bit); // erases the cell
              printf("%d: (%d, %d) \n", counter, x, y * 8 + bit);
              break; // breaks out of the for loop, so it does not check the rest of the bits
            }
          }
        }
      }
    }
  }
  return counter; // returns the counter
}

char aboveRowLarge(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  return aboveRow(image, x - 8, y, bit) | aboveRow(image, x - 8, y - 1, bit) | aboveRow(image, x - 8, y + 1, bit);
}

// It is the row to the right of the bit
char belowRowLarge(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  return belowRow(image, x + 8, y, bit) | belowRow(image, x + 8, y - 1, bit) | belowRow(image, x + 8, y + 1, bit);
}

// It is the row above the bit
char leftRowLarge(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  return leftRow(image, x, y - 1, bit) | leftRow(image, x - 8, y - 1, bit) | leftRow(image, x + 9, y - 1, bit);
}
char rightRowLarge(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  return rightRow(image, x, y + 1, bit) | rightRow(image, x - 8, y + 1, bit) | rightRow(image, x + 9, y + 1, bit);
}

char checkBoxLarge(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {

  if (aboveRowLarge(image, x, y, bit) || // If one of the 4 rows around the bit contains a white pixel, return 1 else return 0
    belowRowLarge(image, x, y, bit) ||
    rightRowLarge(image, x, y, bit) ||
    leftRowLarge(image, x, y, bit)) {
    return 1;
  }
  return 0;
}

void eraseLargeCell(unsigned char image[BMP_WIDTH][BIT_WIDTH], int x, int y, char bit) {
  eraseCell(image, x, y, bit);
  eraseCell(image, x, y - 1, bit);
  eraseCell(image, x, y + 1, bit);
  eraseCell(image, x - 8, y, bit);
  eraseCell(image, x - 8, y - 1, bit);
  eraseCell(image, x - 8, y + 1, bit);
  eraseCell(image, x + 8, y, bit);
  eraseCell(image, x + 8, y - 1, bit);
  eraseCell(image, x + 8, y + 1, bit);
}

int countLarge(unsigned char image[BMP_WIDTH][BIT_WIDTH], int counter, unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {
  for (int x = 16; x < BMP_WIDTH - 16; x++) {
    for (int y = 2; y < BIT_WIDTH - 2; y++) {
      if (image[x][y]) { // checks if there is a pixel in the char, through out the hole image
        for (char bit = 0; bit < 8; bit++) {
          if ((image[x][y] >> (7 - bit)) & 0x01) { // finds the first true bit, from left to right
            if (!checkBoxLarge(image, x, y, bit)) { // checks if there can be a box around the bit with no white pixels (if there is a white pixel in the box, return 1 so it is negated)
              paintCross(input_image, x, y * 8 + bit); // paints a cross on the image
              counter++; // adds one to the counter of cells
              eraseLargeCell(image, x, y, bit);
              printf("%d: (%d, %d) \n", counter, x, y * 8 + bit);
              break; // breaks out of the for loop, so it does not check the rest of the bits
            }
          }
        }
      }
    }
  }
  return counter; // returns the counter
}