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
  moveI (int speed, int steps);
  void
  setStepsPerRevelotion (int stepsPerRevelotion);
  void
  setStepsToGo (int setStepsToGo);
  void
  Accelerate (int*delay, int steps);
    void
  Deaccelerate (int*delay, int steps);
  int findAccelerationSteps(int speedFrom, int speedTo);
  void getDelayArray(int speedFrom, int speedTo, int steps, int* delay);


 

private:
  uint8_t _pul, _dir, _ena;
  long _stepsToGo;
  int _stepsPerRevelotion;
  float _speed;         // Steps per second
};

#endif /* CHENSTEPPER_H_ */
