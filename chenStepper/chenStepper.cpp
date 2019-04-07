#include "chenStepper.h"

chenStepper::chenStepper ()
{
  _pul = 14;
  _dir = 5;
  _ena = 4;

  _stepsPerRevelotion = 200;
  _stepsToGo = _stepsPerRevelotion;
  _currentPos = 0;
  _targetPos = 0;
  _speed = 0.0;
  _maxSpeed = 1.0;
  _acceleration = 1.0;
  _stepInterval = 0;
  _lastStepTime = 0;

  pinMode (_ena, OUTPUT);
  pinMode (_dir, OUTPUT);
  pinMode (_pul, OUTPUT);
  digitalWrite (_ena, LOW);
}

chenStepper::chenStepper (uint8_t pul, uint8_t dir, uint8_t ena)
{
  _pul = pul;
  _dir = dir;
  _ena = ena;

  _stepsPerRevelotion = 200;
  _stepsToGo = _stepsPerRevelotion;
  _currentPos = 0;
  _targetPos = 0;
  _speed = 0.0;
  _maxSpeed = 1.0;
  _acceleration = 1.0;
  _stepInterval = 0;
  _lastStepTime = 0;

  pinMode (_ena, OUTPUT);
  pinMode (_dir, OUTPUT);
  pinMode (_pul, OUTPUT);
  digitalWrite (_ena, LOW);
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
