/*
 * chenStepper.h
 *
 *  Created on: Apr 6, 2019
 *      Author: chen
 */

#ifndef CHENSTEPPER_H_
#define CHENSTEPPER_H_

#include "Arduino.h"

class chenStepper
{
public:
  chenStepper ();
  chenStepper (uint8_t pul, uint8_t dir, uint8_t ena);
  void
  move (int speed);

  void
  setStepsPerRevelotion (int stepsPerRevelotion);
  void
  setStepsToGo (int setStepsToGo);
  void
  setAcceleration (float acceleration);
//   float
//   speed ();
//   long
//   distanceToGo ();
//   long
//   targetPosition ();
//   long
//   currentPosition ();
//   void
//   setCurrentPosition (long position);
//   void
//   runToPosition ();
//   bool
//   runSpeedToPosition ();
//   void
//   runToNewPosition (long position);
// protected:
//   void
//   computeNewSpeed ();
//   virtual void
//   step (uint8_t step);
//   virtual float
//   desiredSpeed ();

private:
  uint8_t _pul, _dir, _ena;
  long _currentPos;    // Steps
  long _targetPos;     // Steps
  long _stepsToGo;
  int _stepsPerRevelotion;
  float _speed;         // Steps per second
  float _maxSpeed;
  float _acceleration;
  unsigned long _stepInterval;
  unsigned long _lastStepTime;
};

#endif /* CHENSTEPPER_H_ */
