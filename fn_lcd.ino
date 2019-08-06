void lcdInit()
{
  lcd.setCursor(11, 1);
  lcd.print("00:00");
}

void lcdUpdate()
{
  // Feed views
  if (viewId > 0)
  {
    byte start_address = (viewId - 1) * 3; // byte donde comenzare a leer

    // Hour
    lcd.setCursor(11, 1);
    byte hh = EEPROM.read(start_address);
    if (hh < 10)
      lcd.print("0");
    lcd.print(hh);

    // Minutes
    lcd.setCursor(14, 1);
    byte mm = EEPROM.read(start_address + 1);
    if (mm < 10)
      lcd.print("0");
    lcd.print(mm);

    // Feed value
    lcd.setCursor(12, 0);
    lcd.print("F");
    lcd.print(viewId);
    lcd.print(" ");
    lcd.print(EEPROM.read(start_address + 2));
  }
  else // clock view
  {
    lcd.setCursor(12, 0);
    lcd.print("    ");

    // Hour
    lcd.setCursor(11, 1);
    if (hours < 10)
      lcd.print("0");
    lcd.print(hours);

    // Minutes
    lcd.setCursor(14, 1);
    if (minutes < 10)
      lcd.print("0");
    lcd.print(minutes);
  }
}