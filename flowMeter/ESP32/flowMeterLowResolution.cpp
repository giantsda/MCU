#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SD.h"
//#include "SPI.h"

#define TFT_CS 5
#define TFT_DC 2
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 4
#define TFT_MISO 19
#define TFT_LED 32

#define Sensor 33
Adafruit_ILI9341 tft = Adafruit_ILI9341 (TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK,
TFT_RST,
           TFT_MISO);
const int __Gnbmp_height = 320;                 // bmp hight
const int __Gnbmp_width = 240;                 // bmp width
File file, root;
File myFile;

/* flowMeter variables*/
volatile int pulseCount;
unsigned long oldTime;
volatile unsigned long oldTimeForInterrupt;

double currentRPM;
double averagedRPM;

double add = 0.;

void
pulseCounter ();

void
setup ()
{
  /* Initialize */
  randomSeed (analogRead (36));
  Serial.begin (115200);
  Serial.println (F ("TFT LCD test"));
//  void SPIClass::begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
  SPI.begin (14, 13, 12);
  SPI.setFrequency (40000000);
  tft.begin ();
  tft.setRotation (3);

  if (!SD.begin (27))
    Serial.println ("Card Mount Failed");
  else
    Serial.println ("Card SUCCESS");

  ledcAttachPin (TFT_LED, 0);
  ledcSetup (0, 5000, 8);
  ledcWrite (0, 130);

  tft.fillScreen (ILI9341_BLACK);
  tft.setCursor (0, 0);
  tft.setTextColor (ILI9341_WHITE);
  tft.setTextSize (2);

  pinMode (Sensor, INPUT);  // sets the digital pin as input
  pulseCount = 0;
  oldTimeForInterrupt=0;

  attachInterrupt (digitalPinToInterrupt (Sensor), pulseCounter, CHANGE);
  int val = 0;
  tft.println ("flowMeter started");
  tft.setTextSize (10);

//  while (1)
//    {
//      val = digitalRead (Sensor);   // read the input pin
//      tft.println (val);
//    }
  oldTime = micros ();
  oldTimeForInterrupt= micros ();
}
void
loop (void)
{

//  Serial.println (micros());

  if ((micros () - oldTime) >= 1000000)
    {
      detachInterrupt (digitalPinToInterrupt (Sensor)); // Detached it to do stuff
      double t = millis () - oldTime;
      currentRPM = pulseCount / 6. * 60; // /6 if using CHANGE and then *60 since there are 60 seconds/min
      Serial.println (currentRPM);

      /* Restart Interrupt */
      pulseCount = 0;
      oldTime = micros ();
      attachInterrupt (digitalPinToInterrupt (Sensor), pulseCounter, CHANGE);
    }

//  tft.fillRect (0, 20, 320, 80, ILI9341_BLUE);
//  tft.setCursor (0, 20);
//  tft.println (currentRPM);


}

void
pulseCounter ()
{
  // Increment the pulse counter
  pulseCount++;
}
