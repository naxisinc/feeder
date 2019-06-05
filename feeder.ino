const int minusBtn_pin = 2;
const int setBtn_pin = 3;
const int plusBtn_pin = 4;
int timeSelect = 0; // 0 unselect, 1 hours, 2 minutes
int hours = 0;
int minutes = 0;

void setup()
{
  pinMode(setBtn_pin, INPUT);
  pinMode(plusBtn_pin, INPUT);
  pinMode(minusBtn_pin, INPUT);

  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(setBtn_pin) == HIGH)
  {
    if (timeSelect == 0)
    {
      timeSelect = 1;
    }
    else if (timeSelect == 1)
    {
      timeSelect = 2;
    }
    else
    {
      timeSelect = 0;
    }
  }

  if (digitalRead(minusBtn_pin) == HIGH)
  {
    if ((timeSelect == 1) && (hours > 0))
    {
      hours--;
    }
    else if ((timeSelect == 2) && (minutes > 0))
    {
      minutes--;
    }
  }

  if (digitalRead(plusBtn_pin) == HIGH)
  {
    if (timeSelect == 1)
    {
      hours++;
    }
    else if (timeSelect == 2)
    {
      minutes++;
    }
  }

  Serial.print(hours);
  Serial.print(":");
  Serial.println(minutes);
  delay(1000);
}
