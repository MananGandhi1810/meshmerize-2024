void linefollow()
{ //green () ;
  paths = 0;
  while ((analogRead(0) > threshold[0] ) && (analogRead(6) > threshold[6] )&& (analogRead(3) < threshold[3]))
  {
    PID();
  }
  lightsoff();
}
void PID()
{
  int error = analogRead(2) - analogRead(4);

  P = error;
  I = I + error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = lfspeed - PIDvalue;
  rsp = lfspeed + PIDvalue;

  for (int i = 0; i < 7; i++) {
    Serial.print(analogRead(i));
    Serial.print("   ");
  }
  Serial.println();

  lsp = constrain(lsp, 0, 200);
  rsp = constrain(rsp, 0, 200);

  motor1.drive(rsp);
  motor2.drive(lsp);
}
