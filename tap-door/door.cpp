#include "door.h"
#include "Arduino.h"

Door::Door(Stepper* _motor) {
  motor = _motor;
  CW = 1;
  CCW = CW * (-1);
  motor->setSpeed(SPEED);
}

bool Door::closed() {
  return digitalRead(limitPin) == LOW;
}

void Door::init(int limitSwitchPin) {
  limitPin = limitSwitchPin;
  close();
}

void Door::close() {
  // trova la posizione iniziale chiusa.
  while (!closed()) {
    motor->step(1 * CCW);
  }
}

void Door::open() {
  motor->step(track * CW);
}

void Door::toggle() {
  if (closed()) {
    open();
  } else {
    close();
  }
}
