// IMPORTANT: ELEGOO_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Elegoo_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <SD.h>
#include <SPI.h>

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define PIN_SD_CS 10 // Elegoo SD shields and modules: pin 10

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define MYCOLOR  8080ff

Elegoo_TFTLCD tft (LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;

#define MAX_BMP         4                      // bmp file num

const int __Gnbmp_height = 320;                 // bmp hight
const int __Gnbmp_width = 240;                 // bmp width

unsigned char __Gnbmp_image_offset = 0;        // offset

File bmpFile, root;

/*********************************************/
// This procedure reads a bitmap and draws it to the screen
// its sped up by reading many pixels worth of data at a time
// instead of just one pixel at a time. increading the buffer takes
// more RAM but makes the drawing a little faster. 20 pixels' worth
// is probably a good place
#define BUFFPIXEL       60                      // must be a divisor of 240
#define BUFFPIXEL_X3    180                     // BUFFPIXELx3

void
plotBmp (char* file);
void
printDirectory (File dir, int numTabs);

void
bmpdraw (File f, int x, int y);
boolean
bmpReadHeader (File f);
uint16_t
read16 (File f);
uint32_t
read32 (File f);
void
drawSetense (int randomSentence);
int bmpFiles;
File myFile;

void
setup (void)
{
  Serial.begin (9600);
  tft.reset ();
  uint16_t identifier = 0x9341;
  tft.begin (identifier);
  tft.fillScreen (tft.color565 (150, 123, 182));  // Tiffany : 50, 180, 140
//  delay (1111111111111111);
				//Init SD_Card
  pinMode (PIN_SD_CS, OUTPUT);
  if (!SD.begin (PIN_SD_CS))
    {
      Serial.println ("initialization failed!");
      return;
    }

  bmpFiles = MAX_BMP;
  tft.setRotation (1);
  randomSeed (analogRead (0));

}

void
loop (void)
{
  int randomFile;
  char fileName[10];
  while (1)
    {
      randomFile = random (1, bmpFiles);
      sprintf (fileName, "%d", randomFile);
      strcat (fileName, ".bmp");

      Serial.println (fileName);
      int randomSentence = random (40);
      plotBmp (fileName);

      tft.setCursor (0, 0);
      tft.setTextColor (WHITE);
      tft.setTextSize (1);
      tft.println (fileName);

      drawSetense (22);
      delay (5000);
    }

}

void
drawSetense (int randomSentence)
{
  tft.setCursor (0, 0);
//  tft.setTextColor (MAGENTA);
  tft.setTextColor (CYAN);
  tft.setTextSize (2);
  myFile = SD.open ("1.txt");
  char temp[50];
  if (myFile)
    {
      Serial.println ("1.txt:");
      // read from the file until there's nothing else in it:
      while (myFile.available ())
	{
	  tft.print ((char) myFile.read ());
	  delay (50);
	}
      // close the file:
      myFile.close ();
    }
  else
    {
      Serial.println ("error opening test.txt");
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
      tft.setTextColor (WHITE);
      tft.setTextSize (1);
      tft.println ("didnt find BMPimage");
      while (1)
	;
    }
  if (!bmpReadHeader (bmpFile))
    {
      Serial.println ("bad bmp");
      tft.setTextColor (WHITE);
      tft.setTextSize (1);
      tft.println ("bad bmp");
      return;
    }
  bmpdraw (bmpFile, 0, 0);
  bmpFile.close ();
}

void
printDirectory (File dir, int numTabs)
{
  while (true)
    {
      File entry = dir.openNextFile ();
      if (!entry)
	{
	  // no more files
	  //Serial.println("**nomorefiles**");
	  break;
	}
      Serial.println (entry.name ());
      if (entry.isDirectory ())
	{
	  Serial.println ("/");
	  printDirectory (entry, numTabs + 1);
	}
      else
	{
	  // files have sizes, directories do not
	  Serial.print ("\t\t");
	  Serial.println (entry.size (), DEC);
	}
      entry.close ();
    }
}

void
bmpdraw (File f, int x, int y)
{
  bmpFile.seek (__Gnbmp_image_offset);

  uint32_t time = millis ();

  uint8_t sdbuffer[BUFFPIXEL_X3];                 // 3 * pixels to buffer

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
	      __color[k] = sdbuffer[buffidx + 2] >> 3;                   // read
	      __color[k] = __color[k] << 6 | (sdbuffer[buffidx + 1] >> 2); // green
	      __color[k] = __color[k] << 5 | (sdbuffer[buffidx + 0] >> 3); // blue

	      buffidx += 3;
	    }

	  for (int m = 0; m < BUFFPIXEL; m++)
	    {
	      tft.drawPixel (i, m + offset_x, __color[m]);
	    }
	}
    }

  Serial.print (millis () - time, DEC);
  Serial.println (" ms");
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
  Serial.print ("size 0x");
  Serial.println (tmp, HEX);

  // read and ignore creator bytes
  read32 (f);

  __Gnbmp_image_offset = read32 (f);
  Serial.print ("offset ");
  Serial.println (__Gnbmp_image_offset, DEC);

  // read DIB header
  tmp = read32 (f);
  Serial.print ("header size ");
  Serial.println (tmp, DEC);

  int bmp_width = read32 (f);
  int bmp_height = read32 (f);

  if (bmp_width != __Gnbmp_width || bmp_height != __Gnbmp_height)
    {    // if image is not 320x240, return false
      return false;
    }

  if (read16 (f) != 1)
    return false;

  bmpDepth = read16 (f);
  Serial.print ("bitdepth ");
  Serial.println (bmpDepth, DEC);

  if (read32 (f) != 0)
    {
      // compression not supported!
      return false;
    }

  Serial.print ("compression ");
  Serial.println (tmp, DEC);

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
