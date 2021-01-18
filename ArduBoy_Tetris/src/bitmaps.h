
#include <Arduino.h>

// Bitmaps
// The first two bytes are the width and height. The width can be any 
// value. The height should be a multiple of 8 pixels.
// Each byte represents a vertical column with LSB at the top.

const unsigned char PROGMEM background[] = {
8, 8,
0b00000000,
0b01000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000010,
0b00000000
};

// I tetrimino 0°
const unsigned char PROGMEM tetrimino_I0[] = {
3, 16,

// upper
0b11110000,
0b11010000,
0b11110000,
// lower
0b11111111,
0b10110110,
0b11111111,
};

// I tetrimino 90°
const unsigned char PROGMEM tetrimino_I90[] = {
12, 8,

0b01110000,
0b01010000,
0b01110000,
0b01110000,
0b01010000,
0b01110000,
0b01110000,
0b01010000,
0b01110000,
0b01110000,
0b01010000,
0b01110000 
};

// L-shape 0°
const unsigned char PROGMEM tetrimino_L0[] = {
6, 16,
// upper
0b11000000,
0b01000000,
0b11000000,
0b00000000,
0b00000000,
0b00000000,
// lower
0b01111111,
0b01011011,
0b01111111,
0b01110000,
0b01010000,
0b01110000
}; 

// L-shape 90°
const unsigned char PROGMEM tetrimino_L90[] = {
9, 8,
0b01111110, 
0b01011010, 
0b01111110,
0b00001110,
0b00001010,
0b00001110,
0b00001110,
0b00001010, 
0b00001110
}; 

// L-shape 180°
const unsigned char PROGMEM tetrimino_L180[] = {
6, 16,
// upper
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
// lower
0b00000011,
0b00000010,
0b00000011,
0b11111111,
0b10110110,
0b11111111
};

// L-shape 270°
const unsigned char PROGMEM tetrimino_L270[] = {
9, 8,
0b00111000,
0b00101000,
0b00111000,
0b00111000,
0b00101000,
0b00111000,
0b00111111,
0b00101101,
0b00111111
};

// J-shape 0°
const unsigned char PROGMEM tetrimino_J0[] = {
6, 16,
// upper
0b00000000,
0b00000000,
0b00000000,
0b11000000,
0b01000000,
0b11000000,
// lower
0b01110000,
0b01010000,
0b01110000,
0b01111111,
0b01011011,
0b01111111
}; 

// J-shape 90°
const unsigned char PROGMEM tetrimino_J90[] = {
9, 8,
0b01111110, 
0b01011010, 
0b01111110,
0b01110000,
0b01010000,
0b01110000,
0b01110000,
0b01010000, 
0b01110000
}; 

// J-shape 180°
const unsigned char PROGMEM tetrimino_J180[] = {
6, 16,
// upper
0b11000000,
0b01000000,
0b11000000,
0b11000000,
0b01000000,
0b11000000,
// lower
0b01111111,
0b01011011,
0b01111111,
0b00000001,
0b00000001,
0b00000001
};

// J-shape 270°
const unsigned char PROGMEM tetrimino_J270[] = {
9, 8,
0b00001110,
0b00001010,
0b00001110,
0b00001110,
0b00001010,
0b00001110,
0b01111110,
0b01011010,
0b01111110
};

// O-shape 0°
const unsigned char PROGMEM tetrimino_O0[] = {
6, 8,
0b01111110,
0b01011010,
0b01111110,
0b01111110,
0b01011010,
0b01111110,
};

// T-shape 0°
const unsigned char PROGMEM tetrimino_T0[] = {
9, 8,
0b00001110,
0b00001010,
0b00001110,
0b01111110,
0b01011010,
0b01111110,
0b00001110,
0b00001010,
0b00001110,
};

// T-shape 90°
const unsigned char PROGMEM tetrimino_T90[] = {
6, 16,
0b00000000,
0b00000000,
0b00000000,
0b11100000,
0b10100000,
0b11100000,

0b00000111,
0b00000101,
0b00000111,
0b00111111,
0b00101101,
0b00111111
};

// T-shape 180°
const unsigned char PROGMEM tetrimino_T180[] = {
9, 8,
0b01110000,
0b01010000,
0b01110000,
0b01111110,
0b01011010,
0b01111110,
0b01110000,
0b01010000,
0b01110000
};

// T-shape 270°
const unsigned char PROGMEM tetrimino_T270[] = {
6, 16,
0b10000000,
0b10000000,
0b10000000,
0b00000000,
0b00000000,
0b00000000,

0b11111111,
0b10110110,
0b11111111,
0b00011100,
0b00010100,
0b00011100
};

// Z tetrimino 0°
const unsigned char PROGMEM tetrimino_Z0[] = {
6, 16,

// upper
0b11000000,
0b01000000,
0b11000000,
0b11111000,
0b01101000,
0b11111000,
// lower
0b00001111,
0b00001011,
0b00001111,
0b00000001,
0b00000001,
0b00000001,
};

// Z tetrimino 90°
const unsigned char PROGMEM tetrimino_Z90[] = {
9, 8,
0b00000111,
0b00000101,
0b00000111,
0b00111111,
0b00101101,
0b00111111,
0b00111000,
0b00101000,
0b00111000
};

// S tetrimino 0°
const unsigned char PROGMEM tetrimino_S0[] = {
6, 16,

// upper
0b11111000,
0b01101000,
0b11111000,
0b11000000,
0b01000000,
0b11000000,
// lower
0b00000001,
0b00000001,
0b00000001,
0b00001111,
0b00001011,
0b00001111,
};

// S tetrimino 90°
const unsigned char PROGMEM tetrimino_S90[] = {
9, 8,
0b11100000,
0b10100000,
0b11100000,
0b11111100,
0b10110100,
0b11111100,
0b00011100,
0b00010100,
0b00011100
};

const unsigned char *tetriminos[] = {   tetrimino_I0, tetrimino_I90, tetrimino_I0, tetrimino_I90, 
                                        tetrimino_L0, tetrimino_L90, tetrimino_L180, tetrimino_L270,
                                        tetrimino_J0, tetrimino_J90, tetrimino_J180, tetrimino_J270,
                                        tetrimino_O0, tetrimino_O0, tetrimino_O0, tetrimino_O0,
                                        tetrimino_T0, tetrimino_T90, tetrimino_T180, tetrimino_T270,
                                        tetrimino_Z0, tetrimino_Z90, tetrimino_Z0, tetrimino_Z90,
                                        tetrimino_S0, tetrimino_S90, tetrimino_S0, tetrimino_S90 };


