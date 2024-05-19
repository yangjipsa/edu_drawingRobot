#include <Servo.h>

#define ServoLift 3
#define ServoL 5
#define ServoR 6

Servo servo_l;
Servo servo_r;
Servo servo_lift;

int angle = 0;

void setup() {
  servo_l.attach(ServoL);
  servo_r.attach(ServoR);
  servo_lift.attach(ServoLift);

  angle = 0;
  servo_l.write(angle);
  servo_r.write(angle);
  servo_lift.write(angle);
  delay(1000);

  angle = 180;
  servo_l.write(angle);
  servo_r.write(angle);
  servo_lift.write(angle);
  delay(1000);
}

void loop() {
  for (int i = 0 ; i < 4 ; i++)
  { // angle : 30 60 90 120
    angle = 30*(i+1);
    servo_l.write(angle);
    servo_r.write(angle);
    servo_lift.write(angle);
    delay(1000);
  }
  for (int i = 0 ; i < 4 ; i++)
  { // angle : 150 120 90 60
    angle = 30*(5-i);
    servo_l.write(angle);
    servo_r.write(angle);
    servo_lift.write(angle);
    delay(1000);
  }  
}
