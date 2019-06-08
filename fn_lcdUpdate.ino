void lcdUpdate()
{
  lcd.clear();

  // Feed rounds
  if (posPointer > 0)
  {
    lcd.setCursor(12, 0);
    lcd.print("F");
    lcd.print(posPointer);
    lcd.print(" ");
    lcd.print(arr[posPointer][2]);
  }

  // Hour
  if (arr[posPointer][0] < 10)
  {
    lcd.setCursor(12, 1);
  }
  else
  {
    lcd.setCursor(11, 1);
  }
  lcd.print(arr[posPointer][0]);
  lcd.print(":");

  // Minutes
  if (arr[posPointer][1] < 10)
  {
    lcd.print("0");
  }
  lcd.print(arr[posPointer][1]);
}