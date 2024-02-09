#include "door.h"
#include "Arduino.h"

void Door::Door(Stepper *_motor) {
  motor = _motor;
  cw = 1;
  ccw = cw * (-1);
}

bool Door::closed() { return digitalRead(limitPin) == LOW; }

void Door::init(int limitSwitchPin) {
  limitPin = limitSwitchPin;
  close();
}

void Door::close() {
  // trova la posizione iniziale chiusa.
  while (!closed()) {
    motor.step(1 * ccw);
  }
}

void Door::open() { motor.step(track * cw); }
