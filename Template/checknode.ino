void checknode ()
{
  l = 0;
  r = 0;
  s = 0;
  u = 0;
  e = 0;
  paths = 0;

  // checks whethere bot is on node and the number of exits possible


  if (analogRead(6) < threshold[6]) r = 1;
  if (analogRead(0) < threshold[0]) l = 1;
  if ((analogRead(0) > threshold[0] && (analogRead(6) > threshold[6]) && (analogRead(3) > threshold[3]))) {
    u = 1;
  }
  if ((analogRead(3) < threshold[3]) && (analogRead(0) < threshold[0]) && (analogRead(6) < threshold[6])) {
    e = 1;
  }

  if (u == 0)
  {
    for (int i = 0; i < FT; i++)
    {
      PID();
      if (analogRead (6) < threshold[6]) r = 1;
      if (analogRead (0) < threshold[0]) l = 1;
    }

    for (int i = 0; i < FT; i++)
    { 
      PID();
      if (analogRead (3) < threshold[3]) s = 1;
      if ((e == 1) && (analogRead(0) < threshold[0]) && (analogRead(6) < threshold[6]) && (analogRead(3) < threshold[3])) e = 2;
    }
    if (e == 2)
    {
      for (int i = 0; i < FT; i++)
      {
        PID();
      }
      if ((analogRead(0) < threshold[0]) && (analogRead(6) < threshold[6]) && (analogRead(3) < threshold[3])) e = 3;
    }
  }
  if (u == 1)
  {
    for (int i = 0; i < 5; i++)
    {
      botinchforward ();
    }
  }

  paths = l + s + r;

}
