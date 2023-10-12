#include <Servo.h>

//for GID Cyber Physical Systems, London, Fall 2023, based off of https://www.hackster.io/benbobgray/mearm-robot-arm-your-robot-v1-0-326702#code

class ServoInfo {
public:
  Servo servo;
  uint8_t arduinoControlPin;
  int restPos;
  int maxDegForward;
  int maxDegBackward;
  int currentPos;
  int lastPos;

  ServoInfo(uint8_t pin, int rPos, int maxF, int maxB, int cPos) :
    arduinoControlPin(pin), restPos(rPos), maxDegForward(maxF),
    maxDegBackward(maxB), currentPos(cPos), lastPos(cPos) {}

  void updatePosition(int newPos) {
    if(newPos >= maxDegBackward && newPos <= maxDegForward) {
      lastPos = currentPos;
      currentPos = newPos;
      servo.write(newPos);
    } else {
      Serial.println("Error: Invalid Position");
    }
  }
};

ServoInfo spine(12, 90, 60, 120, 90);
ServoInfo shoulder(13, 90, 60, 120, 90);
ServoInfo elbow(14, 90, 60, 120, 90);
ServoInfo claw(15, 90, 60, 120, 90);

ServoInfo servos[] = {spine, shoulder, elbow, claw};

void setup() {
  Serial.begin(9600);
  for(int i=0; i < 4; i++) {
    servos[i].servo.attach(servos[i].arduinoControlPin);
    servos[i].updatePosition(servos[i].restPos);
  }
}

void loop() {
  if (Serial.available() > 0) {
    for(int i=0; i < 4; i++) {
      int newPos = Serial.parseInt();
      servos[i].updatePosition(newPos);
    }
    
    char command = Serial.read();
    switch (command) {
      case 'd':
        detachAllServos();
        Serial.println("Servos Detached");
        break;
      case 'r':
        attachAllServos();
        Serial.println("Servos Reattached");
        break;
      case '\n':
        // You might report positions here if you want
        break;
      default:
        Serial.println("Error: Invalid Command Received");
        break;
    }
  }
}

void detachAllServos() {
  for(int i=0; i < 4; i++) {
    servos[i].servo.detach();
  }
}

void attachAllServos() {
  for(int i=0; i < 4; i++) {
    servos[i].servo.attach(servos[i].arduinoControlPin);
  }
}
