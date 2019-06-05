const int minusBtn_pin = 2;
const int setBtn_pin = 3;
int setBtn_currState = LOW;
int setBtn_prevState = LOW;
const int plusBtn_pin = 4;
boolean HH_state = false;
boolean mm_state = false;
int HH_value = 0;
int mm_value = 0;

void setup()
{
  pinMode(setBtn_pin, INPUT);
  pinMode(plusBtn_pin, INPUT);
  pinMode(minusBtn_pin, INPUT);

  Serial.begin(9600);
}

void loop()
{
  setBtn_currState = digitalRead(setBtn_pin);
  if (setBtn_currState != setBtn_prevState)
  {
    if (setBtn_currState == LOW)
    {
      if (!HH_state)
      {
        HH_state = true;
        mm_state = false;
      }
      else
      {
        HH_state = false;
        mm_state = true;
      }
    }
  }
  setBtn_prevState = setBtn_currState;

  Serial.print(HH_state);
  Serial.print(" : ");
  Serial.println(mm_state);
}
