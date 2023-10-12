#DEFINE SOLO_ARDUINO


#include <Servo.h>

// 🤖 Classy Servo! 🎩
// This class helps keep track of each servo's mood (positions and capabilities) and allows them to dance
class ServoInfo {
public:
  Servo servo;  
  uint8_t arduinoControlPin;  // The PWM pin on the servo control board pin on the PWM
  int restPos;  // A good starting, or resting position for each Servo 💤
  int maxDegForward;   // The forward limit.
  int maxDegBackward;  // The backward limit, safeguarding from straining/bumping against screws or other obsticals in our build. 
  int currentPos, lastPos;  
                // Lets us compute whether the servo moved or not. Since Servos are an "open loop" system, 
                // meaning they can't tell us anything about their position, with these two variables we can keep
                // track of where the servo is *and* know whether it moved since its last update. This will come in
                // handy when we're doing more advanced projects in a few weeks.


  // Constructor magic! ✨ We "construct" each instance of the servo in the section just below the robot arms,
  // which is where you'll make each object different if needs to be
  ServoInfo(uint8_t pin, int rPos, int maxF, int maxB, int cPos) :
    arduinoControlPin(pin), restPos(rPos), maxDegForward(maxF),
    maxDegBackward(maxB), currentPos(cPos), lastPos(cPos) {}

  // When asked to move, we ensure the destination is within safe bounds.
  void updatePosition(int newPos) {
    if(newPos >= maxDegBackward && newPos <= maxDegForward) {
      lastPos = currentPos;  // Remembrance of the place we leave.
      currentPos = newPos;  // Embracing the new position.
      servo.write(newPos);  // Commanding the physical to alter its state.
      //Serial.print here -- let user know it moved 
    } else {
      Serial.println("Error: Invalid Position");  //Oopsie! Can't move  there. 
    }
  }
};


//🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾
//based on the pecularities of your particular build, you can set some safety limits here.
ServoInfo spine(12, 90, 60, 120, 90);
ServoInfo shoulder(13, 90, 60, 120, 90);   
ServoInfo elbow(14, 90, 60, 120, 90);       
ServoInfo claw(15, 90, 60, 120, 90);        

ServoInfo servos[] = {spine, shoulder, elbow, claw};

void setup() {
  Serial.begin(9600);  // Opening the channel for chit-chat with Processing! 📡
  for(int i=0; i < 4; i++) {
    servos[i].servo.attach(servos[i].arduinoControlPin); // Connecting nerves to muscles! 💪
    servos[i].updatePosition(servos[i].restPos);  // Start from a restful place! 🧘
  }
}


// 🔄 The Never-Ending Party! 🎊
// loop() keeps our Arduino’s party going, checking for new messages and reacting to them!

// #ifdef SOLO_ARDUINO
//   long fakePosition = 90;
// #endif

void loop() {


// #ifdef SOLO_ARDUINO

//   fakePosition +=10;

//   for(int i=0; i < 4; i++) { // A message? For me?! 💌
//       int newPos = fakePosition % (i*45);  // read out a number
//       servos[i].updatePosition(newPos);  // Each servo, in turn, contemplates whether to move.
//       delay(300);
// }

// #endif


  for(int i=0; i < 4; i++) { // A message? For me?! 💌
      int newPos = Serial.parseInt();    // read out a number
      servos[i].updatePosition(newPos);  // Each servo, in turn, contemplates whether to move.
      delay(300);


  if (Serial.available() > 0) {  // When whispers of serial data flutter by...

    for(int i=0; i < 4; i++) { // A message? For me?! 💌
      int newPos = Serial.praseFloat();    // read out a number
      servos[i].updatePosition(newPos);  // Each servo, in turn, contemplates whether to move.
    }
    

    char command = Serial.read();  // Awaiting a single letter’s command from the serial stream.
    switch (command) {  
      case 'd':
        detachAllServos();  // cut power so we can move around the arm easily
        Serial.println("Servos Detached");  
        break;
      case 'r':
        attachAllServos();  
        Serial.println("Servos Reattached");  
        break;
      case '\n':
        // listening for The 'newline' character, which signals the end of the message  📜
        break;
      default:
        Serial.println("Error: Invalid Command Received");  // Something's not right... ❓
        break;
    }
  }
}



void detachAllServos() {
  for(int i=0; i < 4; i++) {
    servos[i].servo.detach();  // Encouraging each servo to rest from its duties.
  }
}

void attachAllServos() {
  for(int i=0; i < 4; i++) {
    servos[i].servo.attach(servos[i].arduinoControlPin);  // Urging the spirit back into the physical form.
  }
}
