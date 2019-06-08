#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CheapStepper.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

CheapStepper stepper(8, 9, 10, 11); // connect pins 8,9,10,11 to IN1,IN2,IN3,IN4 on ULN2003 board

const int modeBtn_pin = 2;
int modeBtn_currState, modeBtn_prevState = LOW;

const int plusBtn_pin = 3;
int plusBtn_currState, plusBtn_prevState = LOW;

const int setBtn_pin = 4;
int setBtn_currState, setBtn_prevState = LOW;

int arr[5][3] = {0};
int posPointer = -1;
bool setFlag = false;

bool HH_state, mm_state, feed_state = false;

unsigned long previousMillis = 0;
int s = 50;

bool moveClockwise = true;
int degree = 45; // grados por feeding section

void setup()
{
  pinMode(setBtn_pin, INPUT);
  pinMode(plusBtn_pin, INPUT);
  pinMode(modeBtn_pin, INPUT);

  Serial.begin(115200);

  lcd.init(); // initialize the lcd
  lcd.backlight();
  // lcdUpdate();

  stepper.setRpm(12); // initialize RPM
}

void loop()
{
  stepper.run(); // keep running the rest of code

  blink(); // indica donde esta el cursor

  // Clock running
  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) >= 1000) // each 1s
  {
    previousMillis = currentMillis; // prepare for next loop
    s++;
    // TEMP
    lcd.setCursor(0, 1);
    lcd.print(s);
    // END TEMP
    if (s > 59)
    {
      s = 0;
      arr[0][1]++; // inc minutes
      if (arr[0][1] > 59)
      {
        arr[0][1] = 0;
        arr[0][0]++; // inc hours
        if (arr[0][0] > 23)
        {
          arr[0][0] = 0;
        }
      }
      lcdUpdate();

      // Check the feed sections arr
      for (int i = 1; i < 5; i++)
      {
        if ((arr[i][2] > 0) && (arr[0][0] == arr[i][0]) && (arr[0][1] == arr[i][1]))
        {
          // Feeding n-times
          stepper.newMoveDegrees(moveClockwise, degree * arr[i][2]);
        }
      }
    }
  }

  // Mode Button
  modeBtn_currState = digitalRead(modeBtn_pin);
  if (modeBtn_currState != modeBtn_prevState)
  {
    if (modeBtn_currState == LOW)
    {
      if (setFlag)
      {
        if (HH_state)
        {
          HH_state = false;
          mm_state = true;
        }
        else if (mm_state && posPointer != 0)
        {
          mm_state = false;
          feed_state = true;
        }
        else
        {
          HH_state = true;
          mm_state = false;
          feed_state = false;
        }
      }
      else
      {
        posPointer = posPointer < 4 ? posPointer + 1 : 0;
        lcdUpdate();
      }
    }
  }
  modeBtn_prevState = modeBtn_currState;

  // Set Button
  setBtn_currState = digitalRead(setBtn_pin);
  if (setBtn_currState != setBtn_prevState)
  {
    if (setBtn_currState == LOW)
    {
      setFlag = !setFlag;
      if (setFlag)
      {
        HH_state = true;
      }
      else
      {
        HH_state = false;
        mm_state = false;
        feed_state = false;
        if (posPointer == 0)
        {
          s = 0; // reinicio el conteo
        }
      }
    }
  }
  setBtn_prevState = setBtn_currState;

  // Plus Button
  plusBtn_currState = digitalRead(plusBtn_pin);
  if ((plusBtn_currState != plusBtn_prevState) && (HH_state || mm_state || feed_state))
  {
    if (plusBtn_currState == LOW)
    {
      if (HH_state)
      {
        arr[posPointer][0] = (arr[posPointer][0] < 23) ? arr[posPointer][0] + 1 : 0;
        lcdUpdate();
      }
      else if (mm_state)
      {
        arr[posPointer][1] = (arr[posPointer][1] < 59) ? arr[posPointer][1] + 1 : 0;
        lcdUpdate();
      }
      else
      {
        arr[posPointer][2] = (arr[posPointer][2] < 2) ? arr[posPointer][2] + 1 : 0;
        lcdUpdate();
      }
    }
  }
  plusBtn_prevState = plusBtn_currState;
}
