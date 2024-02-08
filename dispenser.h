#include <stdint.h>
#ifndef DAG_DISPENSER_H
#define DAG_DISPENSER_H


#include "Arduino.h"
#include "pins.h"
#include "leds.h"


class Dispenser {

private:
  // stato del programma
  bool active = false;
  // programma di dosaggio
  uint8_t prg = 0x00;
  // livello di dosaggio
  uint8_t lvl = 0x00;
  // moltiplicatore del dosaggio
  float multiplier = 1.0;
  // valore iniziale del dosaggio di ogni songolo flask
  float tare;
  // imposta il moltiplicatore in vase al level passato icon start
  void setMultiplier(void);

  // elenco delle bottiglie
  bool flasks[4];
  int pumps[4] = { PUMP_DET_PIN,
                   PUMP_ANC_PIN,
                   PUMP_IGZ_PIN,
                   PUMP_AMM_PIN };

public:
  // construtor
  Dispenser(void);
  //avvia il programma ,  settando lo stato active
  void start(uint8_t program, uint8_t level);
  // ferma il programma settando lo stato active to false
  void stop(void);
  // loop controller
  void run(float weight);
  // restituisce lo stato active
  bool running(void);
};


#endif