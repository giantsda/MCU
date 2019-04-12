
// One run for different velocity code with fine intervals. It will produce one file. The velocity is predefined in the code.

#include <chenStepper.h>
const int ena = 4;
const int dir = 5;
const int pul = 14;
const int leftLimitPin = 17;
const int startPin = 16;

bool start = false;
int steps1;
int* delayArray;
int totalLength;
int high, low;

int de = 0;
chenStepper stepper;

void
startMeasure ();
int runTime=1;

void
setup ()
{
  Serial.begin (115200);
  pinMode (leftLimitPin, INPUT_PULLDOWN);  // sets the digital pin as input
  pinMode (startPin, INPUT_PULLUP);

  attachInterrupt (digitalPinToInterrupt (startPin), startMeasure, FALLING);
  // Goto left limit. Nosie is too high if using interrupt
  digitalWrite (dir, LOW); // go backwards
  while (digitalRead (leftLimitPin) == LOW)
  {
    digitalWrite (pul, HIGH);
    delayMicroseconds (700);
    digitalWrite (pul, LOW);
    delayMicroseconds (700);
  }
  Serial.println ("in position");

  digitalWrite (dir, HIGH); // go forward
  delay (200);   
  stepper.moveI (700, 40);
}

void
loop ()
{
  totalLength = 3600;
  high = 1500;
//  low =  149.8445/1.5;   // velocity==149.8445/speed m/s
delay (10000);
 for (double velocity=0.05;velocity<1.5;velocity=velocity+0.0125)
  {
    low=149.8445/velocity;
     delay (3000);
    Serial.println ("RUN");
    Serial.println (velocity,5);
    Serial.println (low);
    steps1 = stepper.findAccelerationSteps (high, low);
    delayArray = (int*) malloc (sizeof(int) * steps1);
    stepper.getDelayArray (high, low, steps1, delayArray);
    Serial.println (steps1);
    stepper.Accelerate (delayArray, steps1);
    stepper.moveI (low, totalLength - steps1 * 2);
    stepper.Deaccelerate (delayArray, steps1);
    free(delayArray);
     
// go back
    digitalWrite (dir, LOW);
    delay (5000);
    stepper.moveI (700, totalLength);
    digitalWrite (dir, HIGH);
  }
  while(1)
  ;
}

void
startMeasure ()
{
  start = true;
}