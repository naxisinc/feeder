void backlight_TurnOn()
{
  backlightTimeOut = millis();
  lcd.backlight();
  lcdIsOn = true;
}