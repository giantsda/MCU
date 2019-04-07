const int ena = 4;
const int dir = 5;
const int pul = 14;
int speed;
int N;

void
setup ()
{
  Serial.begin (9600);
  pinMode (ena, OUTPUT);
  pinMode (dir, OUTPUT);
  pinMode (pul, OUTPUT);
  digitalWrite (ena, LOW);
  digitalWrite (dir, HIGH);
  digitalWrite (pul, HIGH);
}

void
loop ()
{
  speed = 350;
  while (speed > -1)
  {
    if (speed > 100)
    N = 20;
    else if (speed > 40)
    N = 200;
    else if (speed > 20)
    N = 8000;
    for (int i = 0; i < N; i++)
    {
      digitalWrite (pul, HIGH); //atribui o novo estado à porta
      delayMicroseconds (speed);
      digitalWrite (pul, LOW); //atribui o novo estado à porta
      delayMicroseconds (speed);
    }
    if (speed > 27)
    {
//      Serial.println(speed);
      speed = speed - 1;
    }
  }
}
