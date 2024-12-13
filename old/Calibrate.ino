void calibrate() {
  for (int i = 0; i < 7; i++) {
    minValues[i] = analogRead(i);
    maxValues[i] = analogRead(i);
  }

  for (int i = 0; i < 500; i++) {
    motor1.drive(40);
    motor2.drive(-40);

    for (int i = 0; i < 7; i++) {
      if (analogRead(i) < minValues[i]) {
        minValues[i] = analogRead(i);
      }
      if (analogRead(i) > maxValues[i]) {
        maxValues[i] = analogRead(i);
      }
    }
  }

  for (int i = 0; i < 7; i++) {
    threshold[i] = (minValues[i] + maxValues[i]) / 2;
    Serial.print(threshold[i]);
    Serial.print("   ");
    Serial.print(minValues[i]);
    Serial.print("   ");
    Serial.print(maxValues[i]);
    Serial.print("   ");
  }
  Serial.println();

  motor1.drive(0);
  motor2.drive(0);
}