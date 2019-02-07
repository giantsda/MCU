#include <Arduino.h>

byte statusLed = 13;
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin = 2;
//#define LEDOUTPUT
#define TOUCHSCREEN

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per L/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;

#ifdef  LEDOUTPUT
#include "LiquidCrystal.h"
LiquidCrystal lcd (7, 8, 9, 10, 11, 12);
#endif

#ifdef TOUCHSCREEN
#include "Elegoo_GFX.h"    // Core graphics library
#include "Elegoo_TFTLCD.h" // Hardware-specific library
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

class __FlashStringHelper;
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft (LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))
int lineNumber;
int width = tft.width ();
int height = tft.height ();
#endif

float flowRate;
float v; //m/s
double s;

unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
float totalTime;
float vAverage;
float totalDistance;

unsigned long oldTime;
int i;

void
pulseCounter ();

void
setup ()
{
  // Initialize a serial connection for reporting values to the host
  Serial.begin (9600);

#ifdef  LEDOUTPUT
  lcd.begin (16, 2);
#endif

  pinMode (sensorPin, INPUT);


  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  oldTime = 0;
  v = 0.;
  totalTime = 0;
  vAverage = 0;
  totalDistance = 0;

  float d = 0.01905; //meter, 3/4 inch
  float r = d / 2.;
  s = 3.14159265358 * r * r;
//  attachInterrupt (sensorInterrupt, pulseCounter, FALLING);

#ifdef TOUCHSCREEN

  tft.reset ();
  uint16_t identifier = tft.readID ();
  identifier = 0x9341;
  tft.begin (identifier);
  tft.fillScreen (BLACK);
//  delay(50000);
  tft.setRotation (1);
  tft.setTextColor (WHITE);
  tft.setTextSize (1);
  lineNumber = 0;
#endif

  i = 0;
}

void
loop ()
{
//  Serial.print ("s= ");
//  Serial.println (s, 17);  // Print the integer part of the variable

  if ((millis () - oldTime) > 1000)    // Only process counters once per second
    {
      double t = millis () - oldTime;
      detachInterrupt (sensorInterrupt);

      flowRate = ((1000.0 / t) * pulseCount) / calibrationFactor;  // L/min
      v = flowRate / 1000. / s / 60.;
      totalTime = totalTime + t / 1000.;
      totalDistance = totalDistance + t / 1000. * v;
      vAverage = totalDistance / totalTime;
      Serial.print ("Time= ");
      Serial.print (totalTime, 5);  // Print the integer part of the variable
      Serial.print ("s;");
      Serial.print ("pulseCount= ");
      Serial.print (pulseCount);  // Print the integer part of the variable
//      Serial.print (";flowRate= ");
//      Serial.print (flowRate, 7);  // Print the integer part of the variable
//      Serial.print ("L/min");
      Serial.print (";v= ");
      Serial.print (v, 7);  // Print the integer part of the variable
      Serial.print ("m/s;");
      Serial.print ("vAverage= ");
      Serial.print (vAverage, 7);  // Print the integer part of the variable
      Serial.print ("m/s;\n");

#ifdef  LEDOUTPUT
      lcd.setCursor (0, 0);
      lcd.print ("t=");
      lcd.print (totalTime, 1);
      lcd.print (";v=");
      lcd.print (v, 5);  // Print the integer part of the variable
      lcd.setCursor (0, 1);
      lcd.print ("meanV= ");
      lcd.print (vAverage, 5);// Print the integer part of the variable

#endif

#ifdef TOUCHSCREEN
      tft.setCursor (0, lineNumber);
      tft.fillRect (0, lineNumber, 240, 7, BLACK);
      tft.print ("t=");
      tft.print (totalTime, 1);
      tft.print ("; v=");
      tft.print (v, 5);  // Print the integer part of the variable
      tft.print ("; meanV= ");
      tft.print (vAverage, 5);  // Print the integer part of the variable
//      delay (5000);
      lineNumber = lineNumber + 9;
      if (lineNumber >= width) // if cursor of next attempt is out of screen
	lineNumber = 0;

#endif

      pulseCount = 0;
      oldTime = millis ();
      attachInterrupt (sensorInterrupt, pulseCounter, FALLING);
    }
}

void
pulseCounter ()
{
  // Increment the pulse counter
  pulseCount++;
}
