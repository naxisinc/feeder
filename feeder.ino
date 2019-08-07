#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CheapStepper.h>
#include <EEPROM.h>

/* #ifdef F_CPU
#undef F_CPU
#define F_CPU 16000000L
#endif */

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
// Backlight Time Out
unsigned long backlightTimeOut;
const int interval = 15000; // 15 sec. the backlight ON

CheapStepper stepper(8, 9, 10, 11); // connect pins 8,9,10,11 to IN1,IN2,IN3,IN4 on ULN2003 board

const int feedBtn_pin = 4;
byte feedBtn_currState, feedBtn_prevState = LOW;

const int modeBtn_pin = 5;
byte modeBtn_currState, modeBtn_prevState = LOW;

const int plusBtn_pin = 6;
byte plusBtn_currState, plusBtn_prevState = LOW;

const int setBtn_pin = 7;
byte setBtn_currState, setBtn_prevState = LOW;

byte viewId = 0;      // indica el identificador de la vista (clock, F1, F2, F3, F4)
bool setFlag = false; // variable para controlar el estado del set_btn

bool HH_state, mm_state, feed_state = false;

byte rotations = 0; // para almacenar el numero de rotaciones async
bool moveClockwise = true;
word degrees = 360; // grados q va a rotar el tambor

// TIMER
// Counter and compare values
const uint16_t t1_load = 0;
const uint16_t t1_comp = 62499; // for a 16 MHz Crystal. 256 prescler

bool setOnTime = false;           // indica si el clock fue setting or not
byte hours, minutes, seconds = 0; // clock

bool minuteGone = false; // cada 1 min. refresh the LCD and check the feeder alarms

// const byte led_pin = PB5; // just for testing purpose.

void setup()
{
  // Save power setting all IO pins to LOW
  for (int i = 0; i < 20; i++)
    if (i != 4 || i != 5 || i != 6 || i != 7) // input pins of the buttons
      pinMode(i, OUTPUT);

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

  lcd.init();      // initialize the lcd
  lcd.backlight(); // turn on the LED
  backlightTimeOut = millis();
  lcdInit(); // setting data on lcd

  stepper.setRpm(12); // initialize RPM

  // Setting the EEPROM bytes
  for (byte i = 0; i < 12; i++)
    if (EEPROM.read(i) == 255) // Just one time
      EEPROM.write(i, 0);
}

void loop()
{
  stepper.run(); // keep running the rest of code while motor is rotating

  // Apagando el backlight del LCD
  if ((millis() - backlightTimeOut) > interval)
    lcd.noBacklight(); // turn off backlight

  int stepsLeft = stepper.getStepsLeft(); // let's check how many steps are left in the current move:
  if (stepsLeft == 0)                     // if the current move is done...
  {
    if (rotations > 0)
    {
      stepper.newMoveDegrees(moveClockwise, degrees);
      rotations--;
    }
    else
      for (int i = 8; i < 12; i++) // Set stepper motor pins to LOW pq ya termino de rotar
        digitalWrite(i, LOW);
  }

  if (setFlag || setOnTime) // para transmitir la sensacion de cuando esta contando y cuando no
    blink();                // indica donde esta el cursor

  // Direct feed
  feedBtn_currState = digitalRead(feedBtn_pin);
  if (feedBtn_currState != feedBtn_prevState)
    if (feedBtn_currState == LOW)
      stepper.newMoveDegrees(moveClockwise, degrees);
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
        else if (mm_state && (viewId != 0))
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
        viewId = viewId < 4 ? viewId + 1 : 0;
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
      if (setFlag) // dale el foco a la hora
        HH_state = true;
      else // reloj o feed time fue configurado
      {
        HH_state = false;
        mm_state = false;
        feed_state = false;
        if (viewId == 0)
        {
          seconds = 0;      // reset the clock
          setOnTime = true; // comienza a contar el tiempo al poner el clock en hora
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
      if (viewId == 0) /* Fijando hora del reloj */
      {
        if (HH_state)
          hours = (hours < 23) ? hours + 1 : 0;
        else if (mm_state)
          minutes = (minutes < 59) ? minutes + 1 : 0;
      }
      else /* Fijando feed alarms */
      {
        if (HH_state) // Hour
        {
          byte hour_byte = (viewId - 1) * 3; // hour_byte
          byte hh = EEPROM.read(hour_byte);
          byte hh_inc = (hh < 23) ? hh + 1 : 0;
          EEPROM.write(hour_byte, hh_inc); // actualiza la hora del feed_alarm en la EEPROM
        }
        else if (mm_state) // Minutes
        {
          byte minutes_byte = ((viewId - 1) * 3) + 1;
          byte mm = EEPROM.read(minutes_byte);
          byte mm_inc = (mm < 59) ? mm + 1 : 0;
          EEPROM.write(minutes_byte, mm_inc); // actualiza la hora del feed_alarm en la EEPROM
        }
        else // Qty of rotations
        {
          byte rotation_byte = ((viewId - 1) * 3) + 2;
          byte rot = EEPROM.read(rotation_byte);
          byte rot_inc = (rot < 2) ? rot + 1 : 0;
          EEPROM.write(rotation_byte, rot_inc);
        }
      }
      lcdUpdate();
    }
  }
  plusBtn_prevState = plusBtn_currState;

  if (minuteGone) // Check the feed alarms
  {
    lcdUpdate();
    for (byte i = 1; i < 5; i++)
    {
      bool matchHour = false;
      bool matchMinute = false;
      byte rotNumPos = (i * 3) - 1; // posicion en EEPROM referente al numero de rotaciones
      byte rotValue = EEPROM.read(rotNumPos);
      if (rotValue == 0)
        break;
      if (EEPROM.read(rotNumPos - 1) == minutes)
        matchMinute = true;
      if (EEPROM.read(rotNumPos - 2) == hours)
        matchHour = true;
      if (matchHour && matchMinute) // rota el numero de veces n
      {
        stepper.newMoveDegrees(moveClockwise, degrees);
        rotations = rotValue - 1; // pq ya rotaste en la linea anterior
      }
    }
    minuteGone = false;
  }
}
