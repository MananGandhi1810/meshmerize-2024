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

#define LED_1 8
#define BUTTON_1 11
#define BUTTON_2 12

// Global Variables
Motor motor1 = Motor(M1_IN1, M1_IN2, M1_PWM, M1_OFFSET, STBY);
Motor motor2 = Motor(M2_IN1, M2_IN2, M2_PWM, M2_OFFSET, STBY);

int ir[7] = {A0, A1, A2, A3, A4, A5, A6};
int threshold[7];

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
    while (digitalRead(BUTTON_1))
        ;
    calibrate();
}

// MainLoop
void loop()
{
}

// Function Definitions
void calibrate()
{
    int min[7] = {1023, 1023, 1023, 1023, 1023, 1023, 1023};
    int max[7] = {0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 1500; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            int val = analogRead(ir[j]);
            if (val < min[j])
            {
                min[j] = val;
            }
            if (val > max[j])
            {
                max[j] = val;
            }
        }
        motor1.drive(50);
        motor2.drive(-50);
    }
    motor1.brake();
    motor2.brake();
    for (int i = 0; i < 1500; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            int val = analogRead(ir[j]);
            if (val < min[j])
            {
                min[j] = val;
            }
            if (val > max[j])
            {
                max[j] = val;
            }
        }
        motor1.drive(-50);
        motor2.drive(50);
    }
    motor1.brake();
    motor2.brake();
    for (int i = 0; i < 7; i++)
    {
        threshold[i] = (min[i] + max[i]) / 2;
        Serial.print(threshold[i]);
        Serial.print("  ");
    }
    Serial.println();
}