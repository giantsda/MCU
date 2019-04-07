
#include <chenStepper.h>
const int ena = 4;
const int dir = 5;
const int pul = 14;
chenStepper stepper;  

void setup()
{  
  Serial.begin(115200);
}

void loop()
{  

int high=1200,low=200;
int steps1=stepper.findAccelerationSteps(high,low);
int delay1[steps1];
stepper.getDelayArray(high,low,steps1, delay1);

stepper.Accelerate (delay1,steps1);
stepper.moveI(low,2000-steps1*2);
stepper.Deaccelerate (delay1,steps1);

//for (int i=0;i<steps2;i++)
//{
//Serial.print(i);
//Serial.print(":  ");
//Serial.println(delay2[i]);
//}

  delay(200);
}
