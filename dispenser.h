#include <stdint.h>
#ifndef DAG_DISPENSER_H
#define DAG_DISPENSER_H


#include "Arduino.h"
// #include "pins.h"
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
  enum Flask {
    DETERSIVO = 0,
    ANTICALCARE = 1,
    IGIENIZZANTE = 2,
    AMMORBIDENTE = 3
  };

  // array dei pin delle pompe
  int pumps[4];

  // array che indica quali flask devno essere utilizzati  nel programma
  bool flasks[4];

  // bottiglia attiva
  int activeFlask;

  float dosages[4] = {
    30,  // DETERSIVO
    40,  // ANTICALCARE
    50,  //  IGIENIZZANTE
    60   // AMMORBIDENTE
  };

public:
  // construtor
  Dispenser(int PUMP_DET_PIN, int PUMP_ANC_PIN, int PUMP_IGZ_PIN, int PUMP_AMM_PIN);
  //avvia il programma ,  settando lo stato active
  void start(uint8_t program, uint8_t level, float _tare);
  // ferma il programma settando lo stato active to false
  void stop(void);
  // loop controller, restituisce l'index della pompa attiva
  int run(float weight);
  // restituisce lo stato active
  bool running(void);
};


#endif