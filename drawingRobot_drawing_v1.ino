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

double yMax = 8.2;
double yMin = 3.5;
double xMax = 6.5;
double xMin = -6.5;

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

  servo_lift.write(theta_lift+30);
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
    LiftDown();
    Move2Point(2,4); delay(500);
    Move2Point(2,6); delay(500);
    Move2Point(-2,6); delay(500);
    Move2Point(-2,4); delay(500);
    Move2Point(2,4); delay(500);
    LiftUp();

    /* Drawing Circle */
    /*
    LiftDown();
    for (int i=0 ; i<=360 ; i=i+5)
      {
        tempX = cos(deg2rad(i));
        tempY = sin(deg2rad(i));
        Move2Point(tempX, 6 + tempY);
        delay(100);
      }
    LiftUp();
    */
  }

  if(cStateSW2 < lStateSW2) // if switch1 pushed
  {
    LiftDown();
    Move2Point(0,8); delay(500);
    Move2Point(2,4); delay(500);
    Move2Point(-2,4); delay(500);
    Move2Point(0,8); delay(500);
    LiftUp();
  }
}

double calDistance(double X0, double Y0, double X1,double Y1)
{
  return sqrt((X0-X1)*(X0-X1) + (Y0-Y1)*(Y0-Y1));
}

double cos2nd_Deg(double A, double B, double C) // for angle A
{
  double angle = acos((B*B + C*C - A*A)/(2*B*C));
  return rad2deg(angle);
}

double rad2deg(double rad) // rad to degree
{
  return rad/PI*180;
}
double deg2rad(double deg)  // degree to rad
{
  return deg/180*PI;
}

void LiftUp(void)
{
  servo_lift.write(theta_lift+30);
  digitalWrite(LED1, LOW); // active low
  digitalWrite(LED2, LOW);
  delay(1000);
}

void LiftDown(void)
{
  servo_lift.write(theta_lift);
  digitalWrite(LED1, LOW); // active low
  digitalWrite(LED2, HIGH);
  delay(1000);
}
void Move2Point(double Xt, double Yt)
{
  double a = l_2;
  double c = l_1;
  double b = calDistance(Xt,Yt,x_l0,y_l0);
  double alpha = cos2nd_Deg(a,b,c);

  double d = a;
  double e = calDistance(Xt,Yt,x_r0,y_r0);
  double delta = cos2nd_Deg(d,e,c);

  double alpha_ = rad2deg(asin(Yt/b));
  double delta_ = rad2deg(asin(Yt/e));

  theta_1 = alpha + alpha_;
  theta_2 = 180 - (delta + delta_);

  if (Yt <= yMax && Yt >= yMin && Xt <= xMax && Xt >= xMin)
  {
    if( isnan(alpha) || isnan(delta) )
    {
      Serial.println("That point is out of workspace");
    } 
    else 
    {
      if(theta_1 >= theta_2)
      {
        servo_l.write((int)theta_1);
        servo_r.write((int)theta_2);
      }
      else 
      {
        Serial.println("This drawing robot cannot move to that point");
      }
    }
  }
  else Serial.println("That point is out of workspace");
}
