// Include Libraries
#include <Arduino.h>
#include <EEPROM.h>
#include <SparkFun_TB6612.h>
#include <PID_v1.h>

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

// Initialize Variables
double Kp = 0.1, Kd = 0.4, Ki = 0;
double Setpoint, Input, Output, rpm;

int speed = 80;
int turnspeed = 30;

String path = "";
bool endFlag = false;

// Global Variables
Motor motor1 = Motor(M1_IN1, M1_IN2, M1_PWM, M1_OFFSET, STBY);
Motor motor2 = Motor(M2_IN1, M2_IN2, M2_PWM, M2_OFFSET, STBY);
PID pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int ir[7] = {A7, A1, A2, A3, A4, A5, A6};
int threshold[7];

// Function Prototypes
void calibrate();
void dryrun();
char checknode();

void straight()
{
    Input = (threshold[2] - analogRead(ir[2])) - (threshold[4] - analogRead(ir[4]));
    pid.Compute();
    motor1.drive(speed + Output);
    motor2.drive(speed - Output);
}

void left()
{
    int startTime = millis();
    while (checknode() != 'S' || millis() - startTime < 1000)
    {
        motor2.drive(turnspeed);
        motor1.brake();
    }
}

void right()
{
    int startTime = millis();
    while (checknode() != 'S' || millis() - startTime < 1000)
    {
        motor2.brake();
        motor1.drive(turnspeed);
    }
}

// Setup
void setup()
{
    Serial.begin(115200);
    Serial.print("\nLoaded Threshold Values: ");

    // Initialize Pins
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    pinMode(LED, OUTPUT);

    // PID Controller Setup
    pid.SetMode(AUTOMATIC);
    pid.SetOutputLimits(-speed / 4, speed / 4);

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
    if (digitalRead(BUTTON_1) == LOW)
    {
        delay(1000);
        dryrun();
    }
    if (digitalRead(BUTTON_2) == LOW)
    {
        delay(1000);
        calibrate();
    }
}

// Function Definitions
void calibrate()
{
    int min[7] = {1023, 1023, 1023, 1023, 1023, 1023, 1023}, max[7] = {0, 0, 0, 0, 0, 0, 0};
    Serial.println("Calibration Sequence Starting");
    digitalWrite(LED, HIGH);

    // Blink LED
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
    }

    // Scan
    for (int i = 0; i < 1000; i++)
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
        delay(10);
    }

    // Blink LED
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
    // Print max and min
    Serial.println("Max Values: ");
    for (int i = 0; i < 7; i++)
    {
        Serial.print(max[i]);
        Serial.print("  ");
    }
    Serial.println();
    Serial.println("Min Values: ");
    for (int i = 0; i < 7; i++)
    {
        Serial.print(min[i]);
        Serial.print("  ");
    }
}

char checknode()
{
    if ((analogRead(ir[0]) > threshold[0]) && (analogRead(ir[6]) > threshold[6]) && (analogRead(ir[3]) > threshold[3]) && (analogRead(ir[2]) > threshold[2]) && (analogRead(ir[4]) > threshold[4]) && (analogRead(ir[1]) > threshold[1]) && (analogRead(ir[5]) > threshold[5]))
    {
        return 'N';
    }
    if (((analogRead(ir[0]) > threshold[0]) && (analogRead(ir[6]) > threshold[6])) && (analogRead(ir[3]) < threshold[3]))
    {
        path += 'S';
        return 'S';
    }
    if ((analogRead(ir[0]) < threshold[0]) && (analogRead(ir[6]) < threshold[6]))
    {
        path += 'J';
        return 'J';
    }
    if (analogRead(ir[6]) < threshold[6])
    {
        path += 'R';
        return 'R';
    }
    else if (analogRead(ir[0]) < threshold[0])
    {
        path += 'L';
        return 'L';
    }
    else
    {
        return 'S';
    }
    delay(1000);
}

void dryrun()
{
    Serial.println("Dry Run Mode");
    digitalWrite(LED, LOW);
    endFlag = false;
    while (!endFlag)
    {
        for (int i = 0; i < 7; i++)
        {
            if (analogRead(ir[i]) < threshold[i])
            {
                Serial.print("Line      ");
            }
            else
            {
                Serial.print("No Line       ");
            }
        }
        char node = checknode();
        Serial.println(node);
        // PID
        switch (node)
        {
        case 'S':
            straight();
            break;

        case 'L':
            left();
            break;

        case 'R':
            right();
            break;

        case 'J':
            endFlag = true;
            break;

        default:
            endFlag = true;
            break;
        }
    }
    digitalWrite(LED, HIGH);
    motor1.brake();
    motor2.brake();
    Serial.println("Exiting Dry Run Mode");
}