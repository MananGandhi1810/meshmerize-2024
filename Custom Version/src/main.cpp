// Include Libraries
#include <Arduino.h>
#include <SparkFun_TB6612.h>


// Define Constants
#define M1_IN1 4
#define M2_IN1 6
#define M1_IN2 3
#define M2_IN2 7
#define M1_PWM 9
#define M2_PWM 10
#define STBY 5

const int M1_OFFSET = 1;
const int M2_OFFSET = 1;

#define IR_1 A0
#define IR_2 A1
#define IR_3 A2
#define IR_4 A3
#define IR_5 A4
#define IR_6 A5
#define IR_7 A6

#define LED_1 8
#define BUTTON_1 11
#define BUTTON_2 12


// Global Variables
Motor motor1 = Motor(M1_IN1, M1_IN2, M1_PWM, M1_OFFSET, STBY);
Motor motor2 = Motor(M2_IN1, M2_IN2, M2_PWM, M2_OFFSET, STBY);


// Function Prototypes
void calibrate();


// Setup
void setup()
{
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  pinMode(LED_1, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("Line Following Started");
}


// MainLoop
void loop()
{
}


// Function Definitions
void calibrate()
{
}