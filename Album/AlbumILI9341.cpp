#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SD.h"
//#include "SPI.h"

// For the Adafruit shield, these are the default.

#define TFT_CS 5
#define TFT_DC 2
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 4
#define TFT_MISO 19
#define TFT_LED 32

Adafruit_ILI9341 tft = Adafruit_ILI9341 (TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK,
TFT_RST,
           TFT_MISO);

#define MAX_BMP         4                      // bmp file num

const int __Gnbmp_height = 320;                 // bmp hight
const int __Gnbmp_width = 240;                 // bmp width

unsigned char __Gnbmp_image_offset = 0;        // offset

File bmpFile, root;

#define BUFFPIXEL       60                      // must be a divisor of 240
#define BUFFPIXEL_X3    180                     // BUFFPIXELx3

void
plotBmp (char* file);

void
drawSetense (char* fileName, int fontSize, int refreshRate, uint16_t color =
     ILI9341_RED,
       int startLine = 0);

void
bmpdraw (File f, int x, int y);

boolean
bmpReadHeader (File f);
uint16_t
read16 (File f);
uint32_t
read32 (File f);

uint16_t
rgbTo565 (uint8_t r, uint8_t g, uint8_t b);
int bmpFiles, txtFiles;
File myFile;

void
setup ()
{
  randomSeed (analogRead (36));
  Serial.begin (115200);
  Serial.println (F ("TFT LCD test"));
  SPI.begin (14, 13, 12);
  SPI.setFrequency (40000000);
  tft.begin ();
  tft.setRotation (2);
  ledcAttachPin (TFT_LED, 0);
  ledcSetup (0, 5000, 8);
  ledcWrite (0, 70);
  tft.fillScreen (ILI9341_BLACK);
  if (!SD.begin (27))
    Serial.println ("Card Mount Failed");
  else
    Serial.println ("Card SUCCESS");

  drawSetense (
      "/first.txt", 1, 30,
      tft.color565 (random (100, 255), random (100, 255), random (100, 255)),
      0);
  delay (2000);
  tft.fillScreen (ILI9341_BLACK);
  drawSetense (
      "/second.txt", 1, 20,
      tft.color565 (random (100, 255), random (100, 255), random (100, 255)),
      0);
  delay (3000);
  bmpFiles = 164;
  txtFiles = 300;

}
void
loop (void)
{
  int randomFile;
  char fileName[10];
  while (1)
    {
      randomFile = random (bmpFiles);
      sprintf (fileName, "/%d", randomFile);
      strcat (fileName, ".bmp");
      int randomSentence = random (40);
      Serial.println (fileName);
      plotBmp (fileName);

//      tft.fillScreen (BLACK);

// Draw Txt file
      randomFile = random (txtFiles);  // start.txt and starts form 0;
      sprintf (fileName, "/%d", randomFile);
      strcat (fileName, ".txt");
      Serial.println (fileName);
      if (randomFile >= 66)   // for ASCII Art
  drawSetense (
      fileName, 1, 20,
      rgbTo565 (random (100, 255), random (100, 255), random (100, 255)),
      0);
      else
  drawSetense (
      fileName, 2, 80,
      rgbTo565 (random (100, 255), random (100, 255), random (100, 255)),
      80);
      delay (3000);
    }
}

void
plotBmp (char* file)
{
  bmpFile = SD.open (file);
  if (!bmpFile)
    {
      Serial.println ("didnt find image");
      Serial.println (file);
      while (1)
  ;
    }
  if (!bmpReadHeader (bmpFile))
    {
      Serial.println ("bad bmp");
      return;
    }
  bmpdraw (bmpFile, 0, 0);
  bmpFile.close ();
}

void
drawSetense (char* fileName, int fontSize, int refreshRate, uint16_t color,
       int startLine)
{
  tft.setCursor (0, startLine);
  tft.setTextColor (color);
  tft.setTextSize (fontSize);
  myFile = SD.open (fileName);
  char temp[50];
  if (myFile)
    {
      // read from the file until there's nothing else in it:
      while (myFile.available ())
  {
    tft.print ((char) myFile.read ());
    delay (refreshRate);
  }
      // close the file:
      myFile.close ();
    }
  else
    {
      Serial.print ("error opening  ");
      Serial.println (fileName);
    }
  myFile.close ();
}

void
bmpdraw (File f, int x, int y)
{
  bmpFile.seek (__Gnbmp_image_offset);
  uint32_t time = millis ();
  uint8_t sdbuffer[BUFFPIXEL_X3];                // 3 * pixels to buffer
  for (int i = 0; i < __Gnbmp_height; i++)
    {
      for (int j = 0; j < (240 / BUFFPIXEL); j++)
  {
    bmpFile.read (sdbuffer, BUFFPIXEL_X3);

    uint8_t buffidx = 0;
    int offset_x = j * BUFFPIXEL;
    unsigned int __color[BUFFPIXEL];

    for (int k = 0; k < BUFFPIXEL; k++)
      {
        __color[k] = sdbuffer[buffidx + 2] >> 3;           // read
        __color[k] = __color[k] << 6 | (sdbuffer[buffidx + 1] >> 2); // green
        __color[k] = __color[k] << 5 | (sdbuffer[buffidx + 0] >> 3); // blue
        buffidx += 3;
      }

    for (int m = 0; m < BUFFPIXEL; m++)
      {
        tft.drawPixel (m + offset_x, i, __color[m]);
      }
  }
    }
}

boolean
bmpReadHeader (File f)
{
  // read header
  uint32_t tmp;
  uint8_t bmpDepth;

  if (read16 (f) != 0x4D42)
    {
      // magic bytes missing
      return false;
    }

  // read file size
  tmp = read32 (f);

  // read and ignore creator bytes
  read32 (f);

  __Gnbmp_image_offset = read32 (f);

  // read DIB header
  tmp = read32 (f);

  int bmp_width = read32 (f);
  int bmp_height = read32 (f);

  if (bmp_width != __Gnbmp_width || bmp_height != __Gnbmp_height)
    {    // if image is not 320x240, return false
      return false;
    }

  if (read16 (f) != 1)
    return false;

  bmpDepth = read16 (f);
  if (read32 (f) != 0)
    {
      // compression not supported!
      return false;
    }
  return true;
}

/*********************************************/
// These read data from the SD card file and convert them to big endian
// (the data is stored in little endian format!)
// LITTLE ENDIAN!
uint16_t
read16 (File f)
{
  uint16_t d;
  uint8_t b;
  b = f.read ();
  d = f.read ();
  d <<= 8;
  d |= b;
  return d;
}

// LITTLE ENDIAN!
uint32_t
read32 (File f)
{
  uint32_t d;
  uint16_t b;

  b = read16 (f);
  d = read16 (f);
  d <<= 16;
  d |= b;
  return d;
}

uint16_t
rgbTo565 (uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
