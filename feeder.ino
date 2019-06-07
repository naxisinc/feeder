#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

const int modeBtn_pin = 2;
int modeBtn_currState = LOW;
int modeBtn_prevState = LOW;

const int plusBtn_pin = 3;
int plusBtn_currState = LOW;
int plusBtn_prevState = LOW;

const int setBtn_pin = 4;
int setBtn_currState = LOW;
int setBtn_prevState = LOW;

int settings_arr[5][3] = {
    {0, 0, 0},
    {1, 1, 1},
    {2, 2, 2},
    {3, 3, 3},
    {4, 4, 4}};
int posPointer = -1;
bool setFlag = false;

bool HH_state = false;
bool mm_state = false;
bool feed_state = false;

int ss_counter = 0;

void setup()
{
  pinMode(setBtn_pin, INPUT);
  pinMode(plusBtn_pin, INPUT);
  pinMode(modeBtn_pin, INPUT);

  Serial.begin(115200);

  // for (int i = 0; i < 5; i++)
  // {
  //   for (int j = 0; j < 3; j++)
  //   {
  //     Serial.print(settings_arr[i][j]);
  //     Serial.print(" - ");
  //   }
  //   Serial.println();
  // }

  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome Feeder!");
}

void loop()
{
  blink(11, 1);
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
        else if (mm_state)
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

        HH_state = false;
      }
      else
      {
        posPointer = posPointer < 4 ? posPointer + 1 : 0;
        lcdPrint();
      }
    }
  }
  modeBtn_prevState = modeBtn_currState;

  // Set Time
  setBtn_currState = digitalRead(setBtn_pin);
  if (setBtn_currState != setBtn_prevState)
  {
    if (setBtn_currState == LOW)
    {
      setFlag = !setFlag;
      if (setFlag)
      {
        HH_state = true;
        blink(11, 1);
        // blink(12, 1);
      }
      else
      {
        HH_state = false;
        mm_state = false;
        feed_state = false;
      }
    }
  }
  setBtn_prevState = setBtn_currState;

  // Increase Time
  plusBtn_currState = digitalRead(plusBtn_pin);
  if ((plusBtn_currState != plusBtn_prevState) && (HH_state || mm_state || feed_state))
  {
    if (plusBtn_currState == LOW)
    {
      if (HH_state)
      {
        settings_arr[posPointer][0] = (settings_arr[posPointer][0] < 23) ? settings_arr[posPointer][0] + 1 : 0;
        lcdPrint();
      }
      else if (mm_state)
      {
        settings_arr[posPointer][1] = (settings_arr[posPointer][1] < 59) ? settings_arr[posPointer][1] + 1 : 0;
        lcdPrint();
      }
      else
      {
        settings_arr[posPointer][2] = (settings_arr[posPointer][2] < 2) ? settings_arr[posPointer][2] + 1 : 0;
        lcdPrint();
      }
    }
  }
  plusBtn_prevState = plusBtn_currState;

  // Digital Clock Running
  /*delay(1000);

  ss_counter++;
  if (ss_counter > 59)
  {
    settings_arr[0][1]++; // minutes
    if (settings_arr[0][1] > 59)
    {
      settings_arr[0][0]++; // hours
      if (settings_arr[0][0] > 23)
      {
        settings_arr[0][0] = 0;
      }
      settings_arr[0][1] = 0;
    }
    ss_counter = 0;
  }*/
}

void lcdPrint()
{
  lcd.clear();
  if (posPointer != 0)
  {
    lcd.setCursor(15, 0);
    lcd.print(settings_arr[posPointer][2]);
  }

  if (settings_arr[posPointer][0] < 10)
  {
    lcd.setCursor(12, 1);
  }
  else
  {
    lcd.setCursor(11, 1);
  }
  lcd.print(settings_arr[posPointer][0]);
  lcd.print(":");

  if (settings_arr[posPointer][1] < 10)
  {
    lcd.print("0");
  }
  lcd.print(settings_arr[posPointer][1]);
}

bool turnOff = false;
void blink(int col, int row)
{
  delay(500);
  lcd.setCursor(11, 1);
  if (turnOff)
  {
    lcd.print(" ");
  }
  else
  {
    lcd.print("2");
  }
  turnOff = !turnOff;
}
