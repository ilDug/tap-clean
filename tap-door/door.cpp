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
  while (!closed()) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
  }
}

void Door::close() {
  Serial.println("closing");
  // trova la posizione iniziale chiusa.
  unsigned long start = millis();
  while (!closed()) {
    motor->step(1 * CCW);
    if (millis() - start > 10000) {
      Serial.println("Errore: non riesco a chiudere la porta.");
      return;
      break;
      delay(10);
    } 
  }
  Serial.println("door closed");

}

void Door::open() {
  Serial.println("opening");
    motor->setSpeed(SPEED);

  motor->step(track * CW);
  Serial.println("door opened");
}

void Door::toggle() {
  if (closed()) {
    open();
  } else {
    close();
  }
}
