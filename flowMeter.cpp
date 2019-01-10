#include <Arduino.h>

byte statusLed = 13;
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin = 2;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per L/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;

float flowRate;
float v; //m/s
double s;

unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
float totalTime;
float vAverage;
float totalDistance;

unsigned long oldTime;

void
pulseCounter ();

void
setup ()
{
  // Initialize a serial connection for reporting values to the host
  Serial.begin (9600);

  // Set up the status LED line as an output
  pinMode (statusLed, OUTPUT);
  digitalWrite (statusLed, HIGH);  // We have an active-low LED attached

  pinMode (sensorPin, INPUT);
  digitalWrite (sensorPin, HIGH);

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
  attachInterrupt (sensorInterrupt, pulseCounter, FALLING);

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
      Serial.print ("pulseCount= ");
      Serial.print (pulseCount);  // Print the integer part of the variable
      Serial.print ("; flowRate= ");
      Serial.print (flowRate, 7);  // Print the integer part of the variable
      Serial.print ("L/min; ");
      Serial.print ("v= ");
      Serial.print (v, 7);  // Print the integer part of the variable
      Serial.print ("m/s; ");
      Serial.print ("vAverage= ");
      Serial.print (vAverage, 7);  // Print the integer part of the variable
      Serial.println ("m/s; ");

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
