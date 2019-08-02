ISR(TIMER1_COMPA_vect)
{
  // Serial.println(millis());
  PORTB ^= (1 << led_pin);
  // if (timeSetFlag)
  // {
  seconds++;
  lcd.setCursor(10, 1);
  if (seconds < 10)
  {
    lcd.print("0");
  }
  lcd.print(seconds);

  if (seconds > 59)
  {
    seconds = 0;
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

    // Check the feed secondstions arr
    for (int i = 1; i < 5; i++)
    {
      if ((arr[i][2] > 0) && (arr[0][0] == arr[i][0]) && (arr[0][1] == arr[i][1]))
      {
        // Feeding n-times
        stepper.newMoveDegrees(moveClockwise, degree * arr[i][2]);
      }
    }

    // indica q debe ser refresh el display
    goneFlag = true;
  }
  // }
}
