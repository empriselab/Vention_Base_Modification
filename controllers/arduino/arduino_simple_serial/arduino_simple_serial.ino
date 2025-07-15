#include <SoftwareSerial.h>

// RoboClaw A: controls Motor 1 & 2
SoftwareSerial roboclawA(255, 10);  // TX only

// RoboClaw B: controls Motor 3 & 4
SoftwareSerial roboclawB(255, 8);   // TX only

int speed_A = 5;
int speed_B = 6;

int speed_A_rot = 5;
int speed_B_rot = 5;

const int A1_stop = 64;   // Motor A base
const int A2_stop = 192;  // Motor B base

// Clamp values to safe bounds
int clampMotorA(int value) {
  return constrain(value, 0, 127);
}

int clampMotorB(int value) {
  return constrain(value, 128, 255);
}

void setup() {
  roboclawA.begin(38400);
  roboclawB.begin(38400);
  Serial.begin(9600);  // Serial input from PC
  delay(2000);
}

void forward() {
  roboclawA.write(clampMotorA(A1_stop + speed_A));
  roboclawA.write(clampMotorB(A2_stop + speed_A));
  roboclawB.write(clampMotorA(A1_stop + speed_B));
  roboclawB.write(clampMotorB(A2_stop + speed_B));
}

void backward() {
  roboclawA.write(clampMotorA(A1_stop - speed_A));
  roboclawA.write(clampMotorB(A2_stop - speed_A));
  roboclawB.write(clampMotorA(A1_stop - speed_B));
  roboclawB.write(clampMotorB(A2_stop - speed_B));
}

void left() {
  roboclawA.write(clampMotorA(A1_stop - speed_A_rot));
  roboclawA.write(clampMotorB(A2_stop - speed_A_rot));
  roboclawB.write(clampMotorA(A1_stop + speed_B_rot));
  roboclawB.write(clampMotorB(A2_stop + speed_B_rot));
}

void right() {
  roboclawA.write(clampMotorA(A1_stop + speed_A_rot));
  roboclawA.write(clampMotorB(A2_stop + speed_A_rot));
  roboclawB.write(clampMotorA(A1_stop - speed_B_rot));
  roboclawB.write(clampMotorB(A2_stop - speed_B_rot));
}

void stopAll() {
  roboclawA.write(A1_stop);
  roboclawA.write(A2_stop);
  roboclawB.write(A1_stop);
  roboclawB.write(A2_stop);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 'f':
        forward();
        break;
      case 'b':
        backward();
        break;
      case 'l':
        left();
        break;
      case 'r':
        right();
        break;
      case 's':
        stopAll();
        break;
      default:
        Serial.println("Invalid command.");
        break;
    }
  }
}
