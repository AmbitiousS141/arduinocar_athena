#include <Arduino.h>
#include <IRremote.h>

const int RECIEVER_PIN = 8;

const int in1 = 3;
const int in2 = 5;
const int in3 = 6;
const int in4 = 9;

enum MovementState {  // defines states car can be in
  STOPPED, 
  FORWARD, 
  BACKWARD, 
  LEFT, 
  RIGHT, 
  FORWARD_LEFT, 
  FORWARD_RIGHT, 
  BACKWARD_LEFT, 
  BACKWARD_RIGHT 
};
MovementState currentState = STOPPED;

// The next part is a C++ convention. We declare variables before we use them, but we also want setup() and loop() to be as high up in the lines as possible, so they come next and THEN we define the functions. My used-to-Python brain hates this TT
void Forward();
void Backward();
void Left();
void Right();
void Stop();
void ForwardLeft();
void ForwardRight();
void BackwardLeft();
void BackwardRight();

void setup() {
  Serial.begin(9600); // 9600 baud, or bits per second, it's how fast commands are transmitted
  IrReceiver.begin(RECIEVER_PIN, ENABLE_LED_FEEDBACK);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Stop();
}

void loop() {      
// this next part is here to figure out the codes of buttons - used this when I added FORWARD_s and BACKWARD_s           
//  if (IrReceiver.decode()) { 
//  uint8_t cmd = IrReceiver.decodedIRData.command;
//  Serial.print("Received IR command: 0x");
//  Serial.println(cmd, HEX);  // Print the command in HEX
//  IrReceiver.resume();
//}

  if (IrReceiver.decode()) {
    uint8_t cmd = IrReceiver.decodedIRData.command;
    Serial.print("Received IR command: 0x");
    Serial.println(cmd, HEX);

    switch (cmd) { // switch is a shorter way to calculate if else statements. Processes user input and goes to statement that matches with it
      case 0x18: // 2 . "case" is like if. think detectives.
        Forward();
        currentState = FORWARD;
        break;
      case 0x52: // 8 
        Backward();
        currentState = BACKWARD;
        break;
      case 0x08: // 4 
        Left();
        currentState = LEFT;
        break;
      case 0x5A: // 6 
        Right();
        currentState = RIGHT;
        break;
      case 0x1C: // 5
        Stop();
        currentState = STOPPED;
        break;
      case 0xC: // 1
        ForwardLeft();
        currentState = FORWARD_LEFT;
        break;
      case 0x0: // 3
        ForwardRight();
        currentState = FORWARD_RIGHT;
        break;
      case 0x42: // 7
        BackwardLeft();
        currentState = BACKWARD_LEFT;
        break;
      case 0x4A: // 9
        BackwardRight();
        currentState = BACKWARD_RIGHT;
        break;
      default:
        Serial.println("Unknown command");
        break;
    }
    IrReceiver.resume();
  }

  // Keep motors running in current state
  switch (currentState) {
    case FORWARD: Forward(); break;
    case BACKWARD: Backward(); break;
    case LEFT: Left(); break;
    case RIGHT: Right(); break;
    case FORWARD_LEFT: ForwardLeft(); break;
    case FORWARD_RIGHT: ForwardRight(); break;
    case BACKWARD_LEFT: BackwardLeft(); break;
    case BACKWARD_RIGHT: BackwardRight(); break;
    case STOPPED: Stop(); break;
  }

  delay(10); // to not overload components
}

void Forward() {
  digitalWrite(in1, 0);
  digitalWrite(in2, 255);
  digitalWrite(in3, 0);
  digitalWrite(in4, 255);
}

void Backward() {
  digitalWrite(in1, 255);
  digitalWrite(in2, 0);
  digitalWrite(in3, 255);
  digitalWrite(in4, 0);
}

void Right() {
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 255);
}

void Left() {
  digitalWrite(in1, 0);
  digitalWrite(in2, 255);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
}

void Stop() {
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
}

void ForwardRight() {
  digitalWrite(in1, 0);
  digitalWrite(in2, 255);
  digitalWrite(in3, 0);
  analogWrite(in4, 128);
}

void ForwardLeft() {
  digitalWrite(in1, 0);
  analogWrite(in2, 128);
  digitalWrite(in3, 0);
  digitalWrite(in4, 255);
}

void BackwardRight() {
 analogWrite(in1, 128);
 digitalWrite(in2, 0);
 digitalWrite(in3, 255);
 digitalWrite(in4, 0);
}

void BackwardLeft() {
  digitalWrite(in1, 255);
  digitalWrite(in2, 0);
  analogWrite(in3, 128);
  digitalWrite(in4, 0);
}

// pins on arduino with tilde beside them support analogWrite()