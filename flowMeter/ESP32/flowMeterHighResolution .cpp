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
volatile int interruptCount;
unsigned long oldTime, startTime;
volatile unsigned long oldTimeForInterrupt;
volatile double currentRPM, sumRPMInterrupt, RPM_1s;
volatile double timeSpan = 0.;
unsigned int loopCount;

double averagedRPM, sumRPMLoop;
double add = 0.;
int x, y;

void
pulseCounter ();

void
setup ()
{
  /* Initialize */
  randomSeed (analogRead (36));
  Serial.begin (115200);
//  void SPIClass::begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
  SPI.begin (14, 13, 12);
  SPI.setFrequency (40000000);
  tft.begin ();
  tft.setRotation (3);

  if (!SD.begin (27))
    Serial.println ("Card Mount Failed");
  else
    Serial.println ("Card Mount SUCCESS");

  ledcAttachPin (TFT_LED, 0);
  ledcSetup (0, 5000, 8);
  ledcWrite (0, 130);

  tft.fillScreen (ILI9341_BLACK);
  tft.setCursor (0, 0);
  tft.setTextColor (ILI9341_WHITE);
  tft.setTextSize (2);

  pinMode (Sensor, INPUT);  // sets the digital pin as input
  interruptCount = 0;
  oldTimeForInterrupt = 0;
  sumRPMInterrupt = 0.;
  currentRPM = 0.;
  RPM_1s = 0.;
  loopCount = 0;
  sumRPMLoop = 0.;

  tft.println ("flowMeter started");
  tft.println ("--------------------------");
  tft.print ("RPM_1s=: ");
  tft.setTextSize (4);

  x = tft.getCursorX ();
  y = tft.getCursorY ();

  Serial.println ("FlowMeter Started");
  Serial.println ("/*----------------------------*/");
  oldTime = micros ();
  oldTimeForInterrupt = micros ();
  startTime = millis ();
  attachInterrupt (digitalPinToInterrupt (Sensor), pulseCounter, FALLING);
}
void
loop (void)
{
// Serial.println (currentRPM);

  if ((micros () - oldTime) >= 1000000)
    {
      detachInterrupt (digitalPinToInterrupt (Sensor)); // Detached it to do stuff
      loopCount++;
      sumRPMLoop += RPM_1s;
      averagedRPM = sumRPMLoop / loopCount;
      Serial.print ("Time:");
      Serial.print ((millis () - startTime) / 1000., 3); // seconds
      Serial.print ("  RPM_1s:");
      Serial.print (RPM_1s);
      Serial.print ("  averagedRPM:");
      Serial.println (averagedRPM);

      tft.fillRect (x, y, 220, 30, ILI9341_RED);
      tft.setCursor (x, y);
      tft.print (RPM_1s, 0);
      if (RPM_1s == 0)
  {
    tft.print (" STUCK!");
  }

      /* Restart Interrupt */
      interruptCount = 0;
      sumRPMInterrupt = 0.;
      RPM_1s = 0.;
      oldTime = micros ();
      attachInterrupt (digitalPinToInterrupt (Sensor), pulseCounter, FALLING);
    }
}

void
pulseCounter ()
{
  timeSpan = (micros () - oldTimeForInterrupt);
  oldTimeForInterrupt = micros (); // we are measuring start to start time span, we need to save oldTime as soon as possible or currentRPM is going to be larger.
  timeSpan = timeSpan / 1000000.; //Convert it to seconds;
  currentRPM = 60 / timeSpan / 3; // Took 3 Falling to complete 1 revolution and there are 60 seconds/minite
  /* average RPM */
  interruptCount++;
  if (interruptCount > 1) // The first reading is not accurate because it may spend a long time in loop.
    {
      sumRPMInterrupt += currentRPM;
      RPM_1s = sumRPMInterrupt / (interruptCount - 1);
      Serial.print ("Time:");
      Serial.print ((millis () - startTime) / 1000., 3); // seconds
      Serial.print ("  pulseCount:");
      Serial.print (interruptCount);
      Serial.print ("  currentRPM:");
      Serial.print (currentRPM, 5);
      Serial.print ("  RPM_1s:");
      Serial.println (RPM_1s, 5);
    }
}

