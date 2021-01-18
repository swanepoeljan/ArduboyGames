/*
 * Tetris For ArduBoy
 * Jan Swanepoel, 2020
 * 
 * Tetris for the ArduBoy using the harbaum fork of the ArduBoy library to
 * be able to run it on the Arduino Nano.
 * 
 * Notes:
 * -  Sprite drawing functions can only draw bitmaps from Flash and not RAM.
 *    I wanted to load the block from Flash to RAM, then rotate it if needed
 *    before drawing it. Since this is not possible and there is enough Flash
 *    memory I will just save all 4 orientations for each block type.
 * 
 */
#include <Arduino.h>
#include "Arduboy2.h"
#include "bitmaps.h"

const uint8_t FIELD_WIDTH = 33;
const uint8_t FIELD_LEFT  = 63 - (FIELD_WIDTH / 2);
const uint8_t FIELD_RIGHT = FIELD_LEFT + FIELD_WIDTH + 1;

void CreatePlayingField();
bool Collision();
void RowDestroy(uint8_t row);
void RowComplete();
void DisplayScore();
void GameOver();
void RestartGame();


Arduboy2 arduboy;
BeepPin1 beep;
uint16_t frames;
uint16_t framesPrev;
int8_t moveX = 0;             // Tetrimino move X request
int8_t moveR = 0;             // Tetrimino rotation request
uint8_t tetY = 0;             // Tetrimino Y position
uint8_t tetX = 63;            // Tetrimino X position
uint8_t p = 0;                // Tetrimino shape index 
uint8_t rotate = 0;           // Tetrimino rotation index (0 to 3)
uint8_t rotatePrev = 0;
uint8_t score = 0;
uint8_t level = 0;


void CreatePlayingField()
{
  // Tile background
  for (uint8_t bx = 0; bx < 128; bx+=8)
    for (uint8_t by = 24; by < 64; by+=8)
      Sprites::drawOverwrite(bx, by, background, 0);

  // Clear playing field
  arduboy.fillRect(FIELD_LEFT - 1, 0, FIELD_WIDTH + 2, 64, BLACK);

  // Draw playing field bouding box
  arduboy.drawLine(FIELD_LEFT,  0,  FIELD_LEFT, 63);
  arduboy.drawLine(FIELD_LEFT,  63, FIELD_RIGHT,63);
  arduboy.drawLine(FIELD_RIGHT, 0,  FIELD_RIGHT,63);
}


bool Collision()
{
  // Read sprite width & height
  uint8_t width = pgm_read_byte(tetriminos[p + rotate]);
  uint8_t height = pgm_read_byte(tetriminos[p + rotate] + 1);

  // Collision Check Offset, when height is 8 bits then the offset is only
  // the first 2 bytes (width & height), when it's 16 bits we only look at
  // the lower 8 bits so the offset moves by the width.
  uint8_t ccoffset = (height > 8) ? width + 2 : 2;
  uint8_t cp = (height > 8) ? 8 : 0;

  for (uint8_t ci = 0; ci < width; ci++)
  {
    // Read sprite column
    uint8_t col = pgm_read_byte(tetriminos[(p + rotate)] + ccoffset + ci);

    // Compare sprite row with playing field row + 1
    for (uint8_t ri = 0; ri < 8; ri++)
    {
      if (col & (1 << ri))
        if (arduboy.getPixel(tetX + ci, tetY + cp + ri + 1) == WHITE)
          return true;
    }
  }

  return false;
}


void RowDestroy(uint8_t row)
{
  // Move all pixels above row 3 pixels down
  for (uint8_t rowIndex = row; rowIndex > 3; rowIndex--)
  {
    for (uint8_t colIndex = FIELD_LEFT + 1; colIndex < FIELD_RIGHT; colIndex++)
    {
      uint8_t pixColour = arduboy.getPixel(colIndex, rowIndex - 3);
      arduboy.drawPixel(colIndex, rowIndex, pixColour);
    }
  } 
}


void RowComplete()
{
  uint8_t completedLinePixels = 0;

  // Check if a row of uninterrupted pixels exist with the same width as the field.
  for (uint8_t row = 62; row > 3; row -= 3)
    for (uint8_t col = FIELD_LEFT + 1; col < FIELD_RIGHT; col++)
      if (arduboy.getPixel(col, row) == WHITE)
      {
        completedLinePixels++;

        if (completedLinePixels == FIELD_WIDTH)
        {
          score++;
          // If score is a multiple of 5 the level increases
          if ((score % 5) == 0)
            if ((10 - level) > 1)
              level++;

          RowDestroy(row);
          DisplayScore();
          beep.tone(2999, 10);
          // Since a row was just deleted, increase by 3 to check same row again 
          // for next possible completed line
          row += 3;

          // Reset line pixel counter and break out of loop
          completedLinePixels = 0;
          break;
        }
      }
      else
      {
        // Reset line pixel counter and break out of loop
        completedLinePixels = 0;
        break;
      }
}


void DisplayScore()
{
  // Draw score lable
  arduboy.setCursor(0, 0);
  arduboy.print("Lines");
  arduboy.setCursor(98, 0);
  arduboy.print("Level");

  // Draw score values
  arduboy.setCursor(0, 10);
  arduboy.print("      ");
  arduboy.setCursor(0, 10);
  arduboy.print(score);

  arduboy.setCursor(98, 10);
  arduboy.print("      ");
  arduboy.setCursor(98, 10);
  arduboy.print(level);
}


void GameOver()
{
  arduboy.setCursor(38, 24);
  arduboy.print("GAME OVER");
  arduboy.display();
  arduboy.delayShort(1000);

  // Start at note A5 going down in half notes
  beep.tone(1136, 0);         
  arduboy.delayShort(500);
  beep.tone(1204, 0);
  arduboy.delayShort(500);
  beep.tone(1276, 0);
  arduboy.delayShort(500);
  beep.tone(1351, 0);
  arduboy.delayShort(1000);
  beep.noTone();

  // Wait for button press to restart game
  while (!arduboy.pressed(A_BUTTON));
  RestartGame();
}


void RestartGame()
{
  arduboy.clear();
  CreatePlayingField();
  DisplayScore();

  // Reset tetrimino position to start from the top centre
  tetY = 0;
  tetX = 63;
  rotate = 0;

  // Select random tetrimino
  p = random(0, 7);
  p *= 4;

  // Reset score
  score = 0;
  level = 0;
}


void setup() 
{
  arduboy.begin();
  beep.begin();
  beep.noTone();
  arduboy.initRandomSeed();

  arduboy.setFrameRate(60);
  
  RestartGame();

  // Beep for 10 frames (requires beep.timer() in main loop and behind nextFrame)
  // Frequency of the tone = 1000000/(count+1) thus 999 gives 1kHz tone
  beep.tone(999, 10);
}


void loop() 
{
  // Wait until ready for the next frame
  while(!arduboy.nextFrame())

  frames = arduboy.frameCount;
  arduboy.pollButtons();
  beep.timer();

  if ((frames - framesPrev) > (10U - level))
  {
    // Remove sprite at previous position. This allows the collision detection
    // to only look at the playing field.
    Sprites::drawErase(tetX, tetY, tetriminos[p + rotatePrev], 0);
      
    if (!Collision())
    { // No collision 
      
      // Check for collision if X axis movement is applied
      if (moveX != 0)
      {
        // Apply any x movement that has been requested
        tetX += moveX;

        // If move would result in a collision then undo it
        if (Collision())
          tetX -= moveX;

        // Clear move requested
        moveX = 0;
      }

      // Check for collision if rotation movement is applied
      if (moveR != 0)
      {
        rotate += moveR;
        if (rotate > 3)
          rotate = 0;

        if (Collision())
          rotate -= moveR;
        
        if (rotate > 3)
          rotate = 3;

        moveR = 0;
      }
    
      // Increase y to move tetrimino down
      tetY++;    
    }
    else
    { // Collision

      // Redraw the sprite so that it becomes part of the playing field
      Sprites::drawSelfMasked(tetX, tetY, tetriminos[p + rotatePrev], 0);
      
      // Check if game over
      if (tetY == 0)
        GameOver();

      // Reset tetrimino position to start from the top centre
      tetY = 0;
      tetX = 63;
      rotate = 0;

      // Load the next tetrimino
      p = random(0, 7);
      p *= 4;

      // Check if a completed row has been formed and remove it
      RowComplete();
    }
    
    // Draw sprite on new position
    Sprites::drawSelfMasked(tetX, tetY, tetriminos[p + rotate], 0);

    framesPrev = frames;
    rotatePrev = rotate;
  }

  // Test for button pressed
  if (arduboy.justPressed(A_BUTTON))
    moveR = 1;                              // Request to rotate to the right in the next frame
  
  if (arduboy.justPressed(RIGHT_BUTTON))
    moveX = 3;                              // Request to move to the right in the next frame
  
  if (arduboy.justPressed(LEFT_BUTTON))
    moveX = -3;                             // Request to move to the left in the next frame

  arduboy.display();
}