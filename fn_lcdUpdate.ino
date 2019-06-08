void lcdUpdate()
{
  lcd.clear();

  // Feed rounds
  if (posPointer != 0)
  {
    lcd.setCursor(12, 0);
    lcd.print("F");
    lcd.print(posPointer);
    lcd.print(" ");
    lcd.print(settings_arr[posPointer][2]);
  }

  // Hour
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

  // Minutes
  if (settings_arr[posPointer][1] < 10)
  {
    lcd.print("0");
  }
  lcd.print(settings_arr[posPointer][1]);
}