#include "TFT_ILI9225_SPI.h"

//#define TFT_RST 8
//#define TFT_RS  9
//#define TFT_CS  10  // SS
//#define TFT_SDI 11  // MOSI
//#define TFT_CLK 13  // SCK
//#define TFT_LED 3   // 0 if wired to +5V directly
//#define TFT_BRIGHTNESS 255

#define TFT_RST 4
#define TFT_RS  2
#define TFT_CS  15  // SS
#define TFT_SDI 23  // MOSI
#define TFT_CLK 18  // SCK
#define TFT_LED 32   // 0 if wired to +5V directly
#define TFT_BRIGHTNESS 60

// sacrifice precision to speed up
//#define double float

// map scalar to rainbow map
typedef struct
{
  int r, g, b;
} COLOUR;

uint16_t
rgbTo565 (uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

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

COLOUR c;

int colorList[6] =
  { 0xFC9F, COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREENYELLOW,
      COLOR_OLIVE };

TFT_ILI9225_SPI tft = TFT_ILI9225_SPI (TFT_RST, TFT_RS, TFT_CS, TFT_LED,
TFT_BRIGHTNESS);
int width;
int height;

double zm = 1;
double x0 = -0.21503361460851339;
double y0c = 0.67999116792639069;
double x1, x2, y1c, y2, x, y;
int i, j;

double cR, cI, R, I, iteration, Rtemp;

void
setup (void)
{
  randomSeed (
  analogRead (A5));

tft  .begin ();
  width = tft.maxX ();  //320
  height = tft.maxY ();//240
  tft.setOrientation(1);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(32, 0);
  ledcWrite(0, 80);
  Serial.begin (9600);
  Serial.println (F("TFT LCD test"));
  Serial.print ("TFT size is ");
  Serial.print (width);
  Serial.print ("x");
  Serial.println (height);

}

void
loop ()
{

  double dx, dy;
  int maxIte = 50;
  dx = 1;
  tft.setFont (Terminal12x16);
  char str[50];

  while (dx > 1e-8)
    {

      x1 = x0 - 2. * exp (-zm / 20.);
      x2 = x0 + 2. * exp (-zm / 20.);
      y1c = y0c - 2.4 * exp (-zm / 20.);
      y2 = y0c + 2.4 * exp (-zm / 20.);
      dx = (x2 - x1) / width;
      dy = (y2 - y1c) / height;
      for (i = 0; i < width; i++)
  for (j = 0; j < height; j++)
    {

      if (i == 20 && j == 10)
        {
    sprintf (str, "%5.5f", dx);
    tft.drawText (0, 0, "dx=", COLOR_BLACK);
    tft.drawText (100, 0, str, COLOR_BLACK);
        }
      cR = j * dx + x1;
      cI = i * dy + y1c;
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

      c = GetColour (iteration, 1, maxIte);
      tft.drawPixel (j, i, rgbTo565 (c.r, c.g, c.b));
    }
//      delay (5000000);
//      tft.fillScreen (COLOR_BLACK);
      zm = zm + 5.5;
    }
}
