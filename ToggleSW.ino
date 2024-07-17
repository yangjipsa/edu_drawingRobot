#define SW1 2
#define SW2 4
#define LED1 10
#define LED2 11

bool cState1 = true;
bool lState1 = true;
bool cState2 = true;
bool lState2 = true;

bool valLED1 = true;
bool valLED2 = true;

void setup()
{
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop()
{
  cState1 = digitalRead(SW1);
  cState2 = digitalRead(SW1);
  delay(10); // for debouncing

  if(cState1 < lState1) // when you push sw1
  {
    valLED1 = !valLED1;
    digitalWrite(LED1, valLED1);
  }
  if(cState2 < lState2) // when you push sw1
  {
    valLED2 = !valLED2;
    digitalWrite(LED2, valLED2);
  }

  lState1 = cState1;
  lState2 = cState2; // For updating switch values
}
