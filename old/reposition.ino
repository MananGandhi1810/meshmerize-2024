void reposition()
{
  blue();
  if (e == 3)
  {
    str += 'E';
    endFound = 1;
    red();
    botstop();
    delay(2000);
  }
  else if (l == 1)
  {
    if (paths > 1) str += 'L';
    botleft(); //take left
  }
  
  else if (s == 1)
  {
    if (paths > 1) str += 'S';
  }
  else if (r == 1)
  {
    if (paths > 1) str += 'R';
    botright(); //take right
  }

  else if (u == 1)
  {
    magenta();
    str += 'U';
    botuturn(); //take left
  }
  lightsoff();
  Serial.println(str);
}
