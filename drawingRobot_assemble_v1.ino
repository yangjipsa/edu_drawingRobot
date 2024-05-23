#include <Servo.h>

#define ServoLift 3
#define ServoL 5
#define ServoR 6

Servo servo_l;
Servo servo_r;
Servo servo_lift;

int angle = 0;

void setup()
{
  servo_l.attach(ServoL);
  servo_r.attach(ServoR);
  servo_lift.attach(ServoLift);

  delay(1000);

  angle = 90;
  servo_l.write(angle);
  servo_r.write(angle);
  servo_lift.write(angle);
}

void loop() 
{
  
}
