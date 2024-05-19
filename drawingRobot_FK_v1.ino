#include <Servo.h>

#define TRIM_L 1
#define TRIM_R 0

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

const double l_0 = 1.5; //[cm]
const double l_1 = 4.0;
const double l_2 = 4.5;

// P_l0(x_l0, y_l0)
double x_l0 = (-1) * l_0;
double y_l0 = 0.0;
// P_r0(x_r0, y_r0)
double x_r0 = l_0;
double y_r0 = 0.0;

// P_l1(x_l1, y_l1) initialize
double x_l1= 0.0;
double y_l1= 0.0;
// P_r1(x_r1, y_r1) initialize
double x_r1= 0.0;
double y_r1= 0.0;

// P_E(x_E, y_E) initialize (End-Effect)
double x_E = 0.0;
double y_E = 0.0;

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
  if (Serial.available() >= 2) 
  {
    theta_1 = Serial.parseInt();
    theta_2 = Serial.parseInt();

    if(DrawingRobot_Kinematics(theta_1, theta_2))
    {
      servo_l.write(theta_1);
      servo_r.write(theta_2); 
      Serial.println("End-effect P_E : (" + String(x_E) + ", "+ String(y_E)+")");
    }
    else
    {
      Serial.println("Out of workspace.");
    }  
  } 
}

float d2r(float deg)  // degree to rad
{
  return deg/180*PI;
}

bool DrawingRobot_Kinematics(int THETA_1, int THETA_2)
{
  x_l1 = l_1 * cos(d2r(theta_1)) - l_0;
  y_l1 = l_1 * sin(d2r(theta_1));
  x_r1 = l_1 * cos(d2r(theta_2)) + l_0;
  y_r1 = l_1 * sin(d2r(theta_2));
  
  double A = -1 * (y_l1 - y_r1) / (x_l1 - x_r1);
  double B = (x_l1*x_l1 + y_l1*y_l1 - x_r1*x_r1 - y_r1*y_r1) / (2*(x_l1 - x_r1));
  double a = A*A + 1;
  double b = 2*(A*B - x_l1*A - y_l1);
  double c = x_l1*x_l1 + y_l1*y_l1 + B*B - 2*x_l1*B - l_2*l_2;

  y_E = (-b + sqrt(b*b - 4*a*c))/(2*a);
  x_E = A*y_E + B;

  if (isnan(y_E)) 
  {
    y_E = 0.0; 
    x_E = 0.0;
    return false;
  }
  else
  {
    return true;
  }
}

