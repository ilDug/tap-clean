#include "Arduino.h"
#include "dispenser.h"

Dispenser::Dispenser(int PUMP_DET_PIN, int PUMP_ANC_PIN, int PUMP_IGZ_PIN, int PUMP_AMM_PIN) {
  pumps[0] = PUMP_DET_PIN;
  pumps[1] = PUMP_ANC_PIN;
  pumps[2] = PUMP_IGZ_PIN;
  pumps[3] = PUMP_AMM_PIN;
}

bool Dispenser::running() {
  return active;
}

void Dispenser::start(uint8_t program, uint8_t level, float _tare) {
  prg = program;
  lvl = level;
  active = true;
  setMultiplier();
  tare = _tare;

  // legge dal programma da quale flask deve pompare
  flasks[0] = bitRead(prg, DETERSIVO);
  flasks[1] = bitRead(prg, ANTICALCARE);
  flasks[2] = bitRead(prg, IGIENIZZANTE);
  flasks[3] = bitRead(prg, AMMORBIDENTE);
  activeFlask = 0;  // inizializza con la prima flask
}

void Dispenser::stop() {
  active = false;

  //ferma tutte le pompe
  for (int i = 0; i < sizeof pumps; i++) {
    digitalWrite(pumps[i], LOW);
  }
}

int Dispenser::run(float weight) {
  if (!active) return -1;

  /** 
    seleziona ed incrementa la flask attiva di volta in volta
    la flask diventa 0 se è esclusa dal programma
    oppure può essere impostata 0 quando è stata versata tutta dal loop precedente
  */
  if (flasks[activeFlask] == 0) {
    activeFlask = activeFlask + 1;
  }

  // quando ragginge il limite ferma il programma
  if (activeFlask >= sizeof flasks) {
    stop();
    return -1;
  }

  float amount = weight - tare;
  bool full = ((dosages[activeFlask] * multiplier) - amount) <= 0;

  if (full) {
    flasks[activeFlask] = 0;                // segna il flask come completato
    digitalWrite(pumps[activeFlask], LOW);  // disattiva la pompa
    tare = -1;
  } else {
    digitalWrite(pumps[activeFlask], HIGH);  // attiva la pompa
    return activeFlask;
  }
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
