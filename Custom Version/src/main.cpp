// Include Libraries
#include <Arduino.h>
#include <EEPROM.h>
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

#define LED 13
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
    Serial.begin(115200);
    Serial.print("\nLoaded Threshold Values: ");

    // Initialize Pins
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(LED, OUTPUT);

    // Load Threshold Values from EEPROM
    for (int i = 0; i < 7; i++)
    {
        threshold[i] = EEPROM.read(i * 2) + (EEPROM.read(i * 2 + 1) << 8);
        Serial.print(threshold[i]);
        Serial.print("  ");
    }

    // Blink LED
    digitalWrite(LED, HIGH);
    delay(500);
    pinMode(LED, LOW);
    Serial.println("\nLFR Initialized\n");
}

// Main Loop
void loop()
{
    if (digitalRead(BUTTON_2) == LOW)
        calibrate();

    for (int i = 0; i < 7; i++)
    {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(analogRead(ir[i]) > threshold[i] ? "Black      " : "White      ");
    }
    Serial.println();
}

// Function Definitions
void calibrate()
{
    int min[7] = {1023, 1023, 1023, 1023, 1023, 1023, 1023}, max[7] = {0, 0, 0, 0, 0, 0, 0};

    // Start Calibration Sequence after 1 second
    Serial.println("Calibration Sequence Starting");
    digitalWrite(LED, HIGH);
    delay(1000);

    // Clockwise Rotation
    for (int i = 0; i < 750; i++)
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
        motor1.drive(25);
        motor2.drive(-25);
        delay(1);
    }

    // Stop Motors and blink LED
    motor1.brake();
    motor2.brake();
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
    }

    // Anti-Clockwise Rotation
    for (int i = 0; i < 750; i++)
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
        motor1.drive(-25);
        motor2.drive(25);
        delay(1);
    }

    // Stop Motors and blink LED
    motor1.brake();
    motor2.brake();
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
    }

    // Save Threshold Values
    Serial.print("Threshold Values: ");
    for (int i = 0; i < 7; i++)
    {
        threshold[i] = (min[i] + max[i]) / 2;
        EEPROM.write(i * 2, threshold[i] & 0xFF);
        EEPROM.write(i * 2 + 1, (threshold[i] >> 8) & 0xFF);
        Serial.print(threshold[i]);
        Serial.print("  ");
    }
    Serial.println();
}