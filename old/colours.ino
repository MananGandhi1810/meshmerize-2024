void magenta ()

{ /*
    analogWrite (3, 200); //BLUE
    analogWrite(5, 0); //Green      // MAGENTA
    analogWrite(6, 150); //red
  */
}

void yellow ()
{
  /*
    analogWrite (3, 0); //BLUE
    analogWrite(5, 200); //Green      // Yellow
    analogWrite(6, 150); //red
  */
}
void cyan()
{
  /*
    analogWrite (3, 200); //BLUE
    analogWrite(5, 200); //Green      // Cyan
    analogWrite(6, 0); //red
  */
}
void green ()

{
  /*
    digitalWrite (3, LOW);
    digitalWrite(5, HIGH);          // GREEN
    digitalWrite(6, LOW);
  */
}

void red ()
{
    digitalWrite(13,HIGH);
  /*
    digitalWrite (3, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);           //RED
  */
}
void blue ()
{
  digitalWrite(2,HIGH);

  /*
    digitalWrite (3, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);           //BLUE
  */
}
void lightsoff()
{
  digitalWrite(2,LOW);
  digitalWrite(8,LOW);
  digitalWrite(13,LOW);
  /*
    digitalWrite (3, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  */
}
