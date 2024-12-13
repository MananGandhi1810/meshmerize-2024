// Include Libraries
#include <Arduino.h>
#include <SparkFun_TB6612.h>

// Define Constants
#define AIN1 4
#define BIN1 6
#define AIN2 3
#define BIN2 7
#define PWMA 9
#define PWMB 10
#define STBY 5

const int offsetA = 1;
const int offsetB = 1;

// Global Variables
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void calibrate();

void setup()
{
  Serial.begin(115200);
  Serial.println("Line Following Started");
}

void loop()
{
}

void calibrate()
{
}