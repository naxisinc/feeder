ISR(TIMER1_COMPA_vect)
{
  // PORTB ^= (1 << led_pin);
  if (setOnTime)
  {
    seconds++;
    if (seconds > 59)
    {
      seconds = 0;
      minutes++; // inc minutes
      if (minutes > 59)
      {
        minutes = 0;
        hours++; // inc hours
        if (hours > 23)
        {
          hours = 0;
        }
      }

      // Check the feed alarms
      for (int i = 1; i < 5; i++)
      {
        if ((arr[i][2] > 0) && (hours == arr[i][0]) && (minutes == arr[i][1]))
        {
          // Feeding n-times
          stepper.newMoveDegrees(moveClockwise, degree * arr[i][2]);
        }
      }
    }
    // indica q paso un segundo y debe ser refresh el display
    goneFlag = true;
  }
}
