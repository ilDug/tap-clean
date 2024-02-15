#ifndef DAG_DOOR_H
#define DAG_DOOR_H

#include "Arduino.h"
#include <Stepper.h>
// #include "pins.h"

class Door
{
private:
    int limitPin;
    Stepper *motor;
    int CW;         // rotation side. cambiare con -1 per farlo girare dalláltra parte
    int CCW;        // l'inverso di cw
    int SPEED = 1;  // velocità motore
    int track = 60; // corsa di apertura totale

public:
    Door(Stepper *motor);
    void init(int limitSwitchPin);
    void close(void);
    void open(void);
    void toggle(void);
    bool closed(void);
};

#endif
