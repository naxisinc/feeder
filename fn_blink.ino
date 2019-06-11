void blink()
{
  if ((posPointer == 0) && (!HH_state && !mm_state && !feed_state))
  {
    // lcd.setCursor(13, 1);
    lcd.noBlink();
    return;
  }
  else if ((posPointer != 0) && (!HH_state && !mm_state && !feed_state))
  {
    lcd.noBlink();
    return;
  }
  else if (HH_state)
  {
    lcd.setCursor(5, 1);
  }
  else if (mm_state)
  {
    lcd.setCursor(8, 1);
  }
  else if (feed_state)
  {
    lcd.setCursor(15, 0);
  }

  lcd.blink();
}