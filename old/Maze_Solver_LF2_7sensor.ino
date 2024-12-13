/*
   Line sensor on A0,A1,A2,A3,A4,A5,A6
   A0-left & A6 - right

*/
const int startButton = 11;

bool l = 0;
bool r = 0;
bool s = 0;
bool u = 0;
int e = 0;
int paths = 0;

bool endFound = 0;

// int blackValue = 800;
// int whiteValue = 650;
// int threshold = 600;
// int threshold = (blackValue + whiteValue) * 0.5;


int FT = 80;// how much the bot moves forward before turning

int P, D, I, previousError, PIDvalue, error;
int lsp = 100;
int rsp = 100;


int lfspeed = 40;// change this for bot average speed

int turnspeed = 30;
float Kp = 0.1;
float Kd = 0.4;
float Ki = 0;

String str;

#include <SparkFun_TB6612.h>

#define AIN1 4
#define BIN1 6
#define AIN2 3
#define BIN2 7
#define PWMA 9
#define PWMB 10
#define STBY 5

// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

int minValues[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023}, maxValues[8] = {0, 0, 0, 0, 0, 0, 0}, threshold[8];

void setup() {
  Serial.begin(9600);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(13, OUTPUT);

  // lfspeed = 60000 / analogRead(7); //arbitrary conversion to convert analogRead to speed. Need to check if this works for all voltage levels
}

void loop() {

  while (digitalRead(11)) {}
  delay(1000);
  calibrate();

  while (digitalRead(11)) {}
  delay(1000);

  while (endFound == 0) {
    linefollow();
    checknode();

    botstop();
    delay(100);

    reposition();
  }
  for (int m = 0; m < 4; m++) {
    str.replace("LUL", "S");
    str.replace("SUL", "R");
    str.replace("LUS", "R");
    str.replace("RUL", "U");
  }
  int endpos = str.indexOf('E');

  while (digitalRead(11)) {}
  delay(1000);

  for (int i = 0; i <= endpos; i++) {
    char node = str.charAt(i);
    paths = 0;
    while (paths < 2) {
      linefollow();
      checknode();
      if (paths == 1) {
        reposition();
      }
    }
    switch (node) {
      case 'L':
        botstop();
        delay(50);
        botleft();
        break;

      case 'S':
        break;

      case 'R':
        botstop();
        delay(50);
        botright();
        break;

      case 'E':
        red();
        botstop();
        delay(5000);
        break;
    }  //_________end of switch
  }    //_________end of for loop

  // for (int i = 0; i < 7; i++) {
  //   Serial.print("M");
  //   Serial.print(i);
  //   Serial.print(":");
  //   Serial.print(analogRead(i));
  //   Serial.print("   ");
  // }
  // Serial.println();
}
