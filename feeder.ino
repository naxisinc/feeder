#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CheapStepper.h>

#ifdef F_CPU
#undef F_CPU
#define F_CPU 16000000L
#endif

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
// Backlight Time Out
unsigned long backlightTimeOut;
const int interval = 15000; // 15 sec. on

CheapStepper stepper(8, 9, 10, 11); // connect pins 8,9,10,11 to IN1,IN2,IN3,IN4 on ULN2003 board

const int feedBtn_pin = 4;
byte feedBtn_currState, feedBtn_prevState = LOW;

const int modeBtn_pin = 5;
byte modeBtn_currState, modeBtn_prevState = LOW;

const int plusBtn_pin = 6;
byte plusBtn_currState, plusBtn_prevState = LOW;

const int setBtn_pin = 7;
byte setBtn_currState, setBtn_prevState = LOW;

byte arr[5][3] = {0}; // row; col
byte posPointer = 0;  // default position (current hour view)
bool setFlag = false;

bool HH_state, mm_state, feed_state = false;

// TIMER
// Counter and compare values
const uint16_t t1_load = 0;
const uint16_t t1_comp = 62499; // for a 16 MHz Crystal. 256 prescler

bool timeSetFlag = false;
byte seconds = 0; // seconds

bool moveClockwise = true;
byte degree = 360; // grados por feeding section

bool goneFlag = false; // saber si paso un segundo para refresh the LCD

const byte led_pin = PB5; // just for testing purpose.

void setup()
{
  // Save power setting all IO pins to LOW
  for (int i = 0; i < 20; i++)
  {
    if (i != 4 || i != 5 || i != 6 || i != 7) // input pins of the buttons
    {
      pinMode(i, OUTPUT);
    }
  }

  // setup timer interrupt
  cli(); //disable global interrupts

  // Reset Timer1 Control Reg. A
  TCCR1A = 0;

  // Set CTC mode for auto reset to 0 when the compare is reach
  TCCR1B &= ~(1 << WGM13);
  TCCR1B |= (1 << WGM12);

  // Set prescaler of 256
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);

  // Reset Timer1 and set compare value
  TCNT1 = t1_load;
  OCR1A = t1_comp;

  // Enable Timer1 compare interrupt
  TIMSK1 = (1 << OCIE1A);

  // Enable global interrupts
  sei();

  // Serial.begin(115200);

  lcd.init();      // initialize the lcd
  lcd.backlight(); // turn on the LED
  backlightTimeOut = millis();
  lcdInit(); // setting data on lcd

  stepper.setRpm(12); // initialize RPM
}

void loop()
{
  // Apagando el backlight del LCD
  if ((millis() - backlightTimeOut) > interval)
  {
    lcd.noBacklight(); // turn off backlight
  }

  stepper.run(); // keep running the rest of code while motor is rotating

  int stepsLeft = stepper.getStepsLeft(); // let's check how many steps are left in the current move:
  if (stepsLeft == 0)                     // if the current move is done...
  {
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }

  blink(); // indica donde esta el cursor

  feedBtn_currState = digitalRead(feedBtn_pin);
  if (feedBtn_currState != feedBtn_prevState)
  {
    if (feedBtn_currState == LOW)
    {
      // Direct feed
      stepper.newMoveDegrees(moveClockwise, degree);
    }
  }
  feedBtn_prevState = feedBtn_currState;

  // Mode Button
  modeBtn_currState = digitalRead(modeBtn_pin);
  if (modeBtn_currState != modeBtn_prevState)
  {
    if (modeBtn_currState == LOW)
    {
      backlight_TurnOn();
      if (setFlag)
      {
        // desplazate en la misma section
        if (HH_state)
        {
          HH_state = false;
          mm_state = true;
        }
        else if (mm_state && (posPointer != 0))
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
        // desplazate a la siguiente section
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
      backlight_TurnOn();
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
          timeSetFlag = true; // comienza a contar el tiempo
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
      backlight_TurnOn();
      if (HH_state)
      {
        // Hour
        arr[posPointer][0] = (arr[posPointer][0] < 23) ? arr[posPointer][0] + 1 : 0;
        lcd.setCursor(11, 1);
        if (arr[posPointer][0] < 10)
          lcd.print("0");
        lcd.print(arr[posPointer][0]);
      }
      else if (mm_state)
      {
        // Minutes
        arr[posPointer][1] = (arr[posPointer][1] < 59) ? arr[posPointer][1] + 1 : 0;
        lcd.setCursor(14, 1);
        if (arr[posPointer][1] < 10)
          lcd.print("0");
        lcd.print(arr[posPointer][1]);
      }
      else
      {
        // Qty of rotations
        arr[posPointer][2] = (arr[posPointer][2] < 2) ? arr[posPointer][2] + 1 : 0;
        lcd.setCursor(15, 0);
        lcd.print(arr[posPointer][2]);
      }
    }
  }
  plusBtn_prevState = plusBtn_currState;

  if (goneFlag)
  {
    lcdUpdate();
    goneFlag = false;
  }
}
