void lcdInit()
{
  lcd.setCursor(4, 1);
  lcd.print("00:00:00");
}

void lcdUpdate()
{
  // Feed rounds
  if (posPointer > 0)
  {
    lcd.setCursor(12, 0);
    lcd.print("F");
    lcd.print(posPointer);
    lcd.print(" ");
    lcd.print(arr[posPointer][2]);
  }
  else
  {
    lcd.setCursor(12, 0);
    lcd.print("    ");
  }

  // Hour
  lcd.setCursor(4, 1);
  if (arr[posPointer][0] < 10)
  {
    lcd.print("0");
  }
  lcd.print(arr[posPointer][0]);

  // Minutes
  lcd.setCursor(7, 1);
  if (arr[posPointer][1] < 10)
  {
    lcd.print("0");
  }
  lcd.print(arr[posPointer][1]);
}