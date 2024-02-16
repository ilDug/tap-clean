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
  unsigned long start = millis();
  while (!closed()) {
    motor->step(1 * CCW);
    if (millis() - start > 1000) {
      Serial.println("Errore: non riesco a chiudere la porta.");
      break;
    }
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
