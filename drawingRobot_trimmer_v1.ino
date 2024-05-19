#include <Servo.h>

#define TRIM_L 0
#define TRIM_R 1

#define SW1    2
#define SW2    4

#define LED1   10
#define LED2   11

#define SERVO_LIFT  3
#define SERVO_L     5
#define SERVO_R     6

Servo servo_lift;
Servo servo_l;
Servo servo_r;

// angles
int theta_1 = 90;
int theta_2 = 90;
int theta_lift = 95;

// other variables
bool lift = false;
bool cStateSW1 = false;
bool lStateSW1 = false;
bool cStateSW2 = false;
bool lStateSW2 = false;

bool MOVE_L = false;
bool MOVE_R = false;

void setup() 
{
  Serial.begin(9600);

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  servo_lift.attach(SERVO_LIFT);
  servo_l.attach(SERVO_L);
  servo_r.attach(SERVO_R);
  
  Serial.println("Pin assignment complete.");
  delay(1000);

  servo_lift.write(theta_lift);
  servo_l.write(theta_1);
  servo_r.write(theta_2);
  Serial.println("Servo motors aligned(90 degree).");
  delay(1000);
}

void loop() 
{
  lStateSW1 = cStateSW1;
  lStateSW2 = cStateSW2;
  cStateSW1 = digitalRead(SW1);
  cStateSW2 = digitalRead(SW2);
  delay(10);
  
  if(cStateSW1 < lStateSW1) // if switch1 pushed
  {
    servo_lift.write(theta_lift);
    lift = !lift;
    if(lift)
    {
      servo_lift.write(theta_lift+30);
      digitalWrite(LED1, LOW); // active low
      digitalWrite(LED2, HIGH);
    }
    else
    {
      servo_lift.write(theta_lift);
      digitalWrite(LED1, LOW); // active low
      digitalWrite(LED2, LOW);
    }
  }
  
  int valTrimL = analogRead(TRIM_L);
  int valTrimR = analogRead(TRIM_R);
  valTrimL = map(valTrimL, 0,1023,170,10); // 10 degree margin
  valTrimR = map(valTrimR, 0,1023,170,10);

  if (!MOVE_L && valTrimL > 85 && valTrimL < 95) MOVE_L = true;
  if (!MOVE_R && valTrimR > 85 && valTrimR < 95) MOVE_R = true;

  if (MOVE_L) servo_l.write(valTrimL);
  if (MOVE_R) servo_r.write(valTrimR);
}

