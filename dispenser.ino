/* Control a stepper motor with A4988 stepper motor driver. */
#include <Bounce2.h>

#define dirPin D2
#define stepPin D3
#define stepsPerRevolution 200

#define startLimitPin D5
#define endLimitPin D6
#define emptyLimitPin D7

bool dirForward = true;
bool dirHome = true;

Bounce2::Button startSwitch = Bounce2::Button();
Bounce2::Button endSwitch = Bounce2::Button();
Bounce2::Button emptySwitch = Bounce2::Button();

void dispenserSetup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  startSwitch.attach( startLimitPin, INPUT );
  startSwitch.interval(5); 
  startSwitch.setPressedState(LOW);

  endSwitch.attach( endLimitPin, INPUT );
  endSwitch.interval(5); 
  endSwitch.setPressedState(LOW); 

  emptySwitch.attach( emptyLimitPin, INPUT );
  emptySwitch.interval(5); 
  emptySwitch.setPressedState(LOW); 
}

void dispenserLoop() {
  startSwitch.update();
  endSwitch.update();
  emptySwitch.update();

  if ( startSwitch.pressed() ) {
    isDispensing = false;
    dirForward = true;
    digitalWrite(dirPin, dirForward);
  }

  if ( endSwitch.pressed() ) {
    dirForward = false;
    digitalWrite(dirPin, dirForward);
  }

  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    Serial.println(inChar);

    switch (inChar) {
      case 'd': // dispense
        isDispensing = true;
        break;
      default:
        Serial.println("no matching case");
        break;
    }
  }

  if (isDispensing) {
    // Spin the stepper motor 1 revolution slowly:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}
