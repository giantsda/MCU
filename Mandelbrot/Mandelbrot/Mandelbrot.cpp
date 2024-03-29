// IMPORTANT: ELEGOO_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Elegoo_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com

#include "Elegoo_GFX.h"    // Core graphics library
#include "Elegoo_TFTLCD.h" // Hardware-specific library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

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
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft (LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;

// sacrifice precision to speed up
#define double float

// map scalar to rainbow map
typedef struct
{
  int r, g, b;
} COLOUR;

COLOUR
GetColour (double v, double vmin, double vmax)
{
  COLOUR c =
    { 255, 255, 255 }; // white
  double dv;
  if (v < vmin)
    v = vmin;
  if (v > vmax)
    v = vmax;
  dv = vmax - vmin;
  if (v < (vmin + 0.25 * dv))
    {
      c.r = 0;
      c.g = 4 * (v - vmin) / dv * 255;
    }
  else if (v < (vmin + 0.5 * dv))
    {
      c.r = 0;
      c.b = (1 + 4 * (vmin + 0.25 * dv - v) / dv) * 255;
    }
  else if (v < (vmin + 0.75 * dv))
    {
      c.r = 4 * (v - vmin - 0.5 * dv) / dv * 255;
      c.b = 0;
    }
  else
    {
      c.g = (1 + 4 * (vmin + 0.75 * dv - v) / dv) * 255;
      c.b = 0;
    }
//  c.r = floor (c.r * 255);
//  c.g = floor (c.g * 255);
//  c.b = floor (c.b * 255);
  return (c);
}

double zm = 1;
double x0 = -0.21503361460851339;
double y0 = 0.67999116792639069;
double x1, x2, y1, y2, x, y;
int i, j;
int width = tft.height ();  //320
int height = tft.width ();  //240
double cR, cI, R, I, iteration, Rtemp;

COLOUR c;

void
setup (void)
{
  randomSeed (analogRead (A5));

  Serial.begin (9600);
  Serial.println (F("TFT LCD test"));

#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println (F("Using Elegoo 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print ("TFT size is ");
  Serial.print (tft.width ());
  Serial.print ("x");
  Serial.println (tft.height ());

  tft.reset ();

  uint16_t identifier = tft.readID ();
  identifier = 0x9341;

  tft.begin (identifier);
  tft.fillScreen (BLACK);
  tft.setTextColor (WHITE);
  tft.setTextSize (2);
  tft.setRotation (1);
}

void
loop (void)
{
  double dx, dy;
  int maxIte = 50;
  while (1)
    {
      x1 = x0 - 2. * exp (-zm / 20.);
      x2 = x0 + 2. * exp (-zm / 20.);
      y1 = y0 - 1.13 * exp (-zm / 20.);
      y2 = y0 + 1.13 * exp (-zm / 20.);
      dx = (x2 - x1) / width;
      dy = (y2 - y1) / height;

//      tft.setCursor (0, 0);
//      tft.print (x1, 5);
//      tft.print ("  ");
//      tft.println (x2, 5);
//      tft.print ("  ");
//      tft.print (dx, 5);
//      delay (1000000);

      //      for (int p = 0; p < width * height * 0.3; p++)
//  {
//    i = random (height);
//    j = random (width);
      for (i = 0; i < height; i++)
  for (j = 0; j < width; j++)
    {
      cR = j * dx + x1;
      cI = i * dy + y1;
      R = 0;
      I = 0;
      iteration = 0;
      while (I * I + R * R <= 4. && iteration < maxIte)
        {
    Rtemp = R * R - I * I + cR;
    I = 2 * R * I + cI;
    R = Rtemp;
    iteration = iteration + 1;
        }

//      tft.fillCircle (100, 50, 20, WHITE);
//      tft.fillCircle (50, 100, 20, WHITE);
//      delay (5000000);

      if (iteration <= maxIte / 10|| iteration==maxIte)
        continue;
      else
        {
    c = GetColour (iteration, 1, maxIte);
    tft.drawPixel (j, i, tft.color565 (c.r, c.g, c.b));
        }

//    tft.fillRect(j,i,random (1,2),random (1,2),tft.color565 (c.r, c.g, c.b));

    }
      tft.fillScreen (BLACK);
      zm = zm + 2.5;
    }
}
