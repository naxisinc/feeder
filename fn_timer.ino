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
          hours = 0;
      }
      // indica q paso un minuto y debo chequear si alguna alarm match
      minuteGone = true;
    }
    // indica q paso un segundo y debe ser refresh el display
    secondGone = true;
  }
}
