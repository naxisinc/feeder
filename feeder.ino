const int minusBtn_pin = 2;
int minusBtn_currState = LOW;
int minusBtn_prevState = LOW;

const int setBtn_pin = 3;
int setBtn_currState = LOW;
int setBtn_prevState = LOW;

const int plusBtn_pin = 4;
int plusBtn_currState = LOW;
int plusBtn_prevState = LOW;

boolean HH_state = false;
boolean mm_state = false;
int HH_value = 23;
int mm_value = 59;

boolean timeSet = false;
int ss_counter = 0;

void setup()
{
  pinMode(setBtn_pin, INPUT);
  pinMode(plusBtn_pin, INPUT);
  pinMode(minusBtn_pin, INPUT);

  Serial.begin(115200);
}

void loop()
{
  // Select hours or minutes
  setBtn_currState = digitalRead(setBtn_pin);
  if (setBtn_currState != setBtn_prevState)
  {
    if (setBtn_currState == LOW)
    {
      if (!HH_state && !mm_state)
      {
        HH_state = true;
      }
      else if (HH_state)
      {
        HH_state = false;
        mm_state = true;
      }
      else
      {
        HH_state = mm_state = false;
        timeSet = true;
      }
    }
  }
  setBtn_prevState = setBtn_currState;

  // Decrease Time
  minusBtn_currState = digitalRead(minusBtn_pin);
  if ((minusBtn_currState != minusBtn_prevState) && (HH_state || mm_state))
  {
    if (minusBtn_currState == LOW)
    {
      if (HH_state)
      {
        HH_value = (HH_value > 0) ? HH_value - 1 : 23;
      }
      if (mm_state)
      {
        mm_value = (mm_value > 0) ? mm_value - 1 : 59;
      }
    }
  }
  minusBtn_prevState = minusBtn_currState;

  // Increase Time
  plusBtn_currState = digitalRead(plusBtn_pin);
  if ((plusBtn_currState != plusBtn_prevState) && (HH_state || mm_state))
  {
    if (plusBtn_currState == LOW)
    {
      if (HH_state)
      {
        HH_value = (HH_value < 23) ? HH_value + 1 : 0;
      }
      if (mm_state)
      {
        mm_value = (mm_value < 59) ? mm_value + 1 : 0;
      }
    }
  }
  plusBtn_prevState = plusBtn_currState;

  // Digital Clock Running
  if (timeSet)
  {
    delay(1000);

    ss_counter++;
    if (ss_counter > 59)
    {
      mm_value++;
      if (mm_value > 59)
      {
        HH_value++;
        if (HH_value > 23)
        {
          HH_value = 0;
        }
        mm_value = 0;
      }
      ss_counter = 0;
    }
  }

  Serial.print(HH_value);
  Serial.print(" : ");
  Serial.print(mm_value);
  Serial.print(" : ");
  Serial.println(ss_counter);
}
