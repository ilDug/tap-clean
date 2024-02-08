#include "Arduino.h"
#include "dispenser.h"

Dispenser::Dispenser(void) {
}

bool Dispenser::running() {
  return active;
}

void Dispenser::start(uint8_t program, uint8_t level) {
  prg = program;
  lvl = level;
  active = true;
  setMultiplier();
  flasks[0] = bitRead(prg, DETERSIVO);
  flasks[1] = bitRead(prg, ANTICALCARE);
  flasks[2] = bitRead(prg, IGIENIZZANTE);
  flasks[3] = bitRead(prg, AMMORBIDENTE);
}

void Dispenser::stop(){
  active = false;
}

void Dispenser::run(float weight){
  if(!active) return;



}


void Dispenser::setMultiplier(void) {
  switch (lvl) {
    case SM:
      multiplier = 0.75;
      break;
    case MD:
      multiplier = 1.0;
      break;
    case LG:
      multiplier = 1.25;
      break;
    default:
      multiplier = 1.0;
      break;
  }
}
