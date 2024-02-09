#ifndef DAG_DOOR_H
#define DAG_DOOR_H

#include "Arduino.h"
#include <Stepper.h>
#include "pins.h"


class Door{
  private:
    int limitPin;
    Stepper *motor;
    int cw; //rotation side. cambiare con -1 per farlo girare dalláltra parte
    int ccw ;
    int track = 60; // corsa di apertura totale 

  public:
   void Door(Stepper *motor);
    void init(int limitSwitchPin);
    void close();
    void open();
    bool closed();
}

#endif
