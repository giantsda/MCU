#include "chenStepper.h"

chenStepper::chenStepper ()
{
  _pul = 14;
  _dir = 5;
  _ena = 4;

  _stepsPerRevelotion = 200;
  _stepsToGo = _stepsPerRevelotion;
  _currentPos = 0;

  pinMode (_ena, OUTPUT);
  pinMode (_dir, OUTPUT);
  pinMode (_pul, OUTPUT);
  digitalWrite (_ena, LOW);
  digitalWrite (_dir, HIGH);

}

chenStepper::chenStepper (uint8_t pul, uint8_t dir, uint8_t ena)
{
  _pul = pul;
  _dir = dir;
  _ena = ena;

  _stepsPerRevelotion = 200;
  _stepsToGo = _stepsPerRevelotion;
  _currentPos = 0;

  pinMode (_ena, OUTPUT);
  pinMode (_dir, OUTPUT);
  pinMode (_pul, OUTPUT);
  digitalWrite (_ena, LOW);
  digitalWrite (_dir, HIGH);
}

void
chenStepper::move (int speed)
{
  for (int i = 0; i < _stepsToGo; i++)
  {
    digitalWrite (_pul, HIGH); //atribui o novo estado à porta
    delayMicroseconds (speed);
    digitalWrite (_pul, LOW); //atribui o novo estado à porta
    delayMicroseconds (speed);
  }
  _currentPos += _stepsToGo;
}

void
chenStepper::moveI (int speed, int steps)
{
  for (int i = 0; i < steps; i++)
  {
    digitalWrite (_pul, HIGH); 
    delayMicroseconds (speed);
    digitalWrite (_pul, LOW);  
    delayMicroseconds (speed);
  }
  _currentPos += steps;
}

void
chenStepper::setStepsPerRevelotion (int stepsPerRevelotion)
{
  _stepsPerRevelotion = stepsPerRevelotion;
}

void
chenStepper::setStepsToGo (int setStepsToGo)
{
  _stepsToGo = setStepsToGo;
}

void
chenStepper::Accelerate (int*delay, int steps)
{
  for (int i = 0; i < steps; i++)
  {
    digitalWrite (_pul, HIGH);
    delayMicroseconds (delay[i]);
    digitalWrite (_pul, LOW);
    delayMicroseconds (delay[i]);
  }
  _currentPos += steps;
}

void
chenStepper::Deaccelerate (int*delay, int steps)
{
  for (int i = steps - 1; i >= 0; i--)
  {
    digitalWrite (_pul, HIGH);
    delayMicroseconds (delay[i]);
    digitalWrite (_pul, LOW);
    delayMicroseconds (delay[i]);
  }
  _currentPos += steps;
}

int
chenStepper::findAccelerationSteps (int speedFrom, int speedTo)
{
  float t = (speedTo + 0.) / speedFrom * 0.676;
  int steps;
  for (int i = 1; i < 4000; i = i + 10)
  {
    if ((sqrt (i + 1.) - sqrt (i)) < t)
    {
      return (i);
    }
  }
}

void
chenStepper::getDelayArray (int speedFrom, int speedTo, int steps, int* delay)
{
  float c0 = speedFrom;
  float lastDelay = 0;
  float d = c0;
  for (int i = 0; i < steps; i++)
  {
    if (i == 0)
    {
      delay[i] = d;
      lastDelay = d;
      continue;
    }
    d = lastDelay - (2 * lastDelay) / (4 * i + 1);
    if (d < speedTo)
    d = speedTo;
    delay[i] = d;
    lastDelay = d;
  }
}

long
chenStepper::getCurrentPosition ()
{
  return _currentPos;
}
