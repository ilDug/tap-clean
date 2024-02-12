#include "Arduino.h"
#include "dispenser.h"

Dispenser::Dispenser() {
}

bool Dispenser::running() {
  return active;
}

void Dispenser::start(uint8_t program, uint8_t level, float _tare) {
  prg = program;
  active = true;
  multiplier = setMultiplier(level);
  tare = _tare;

  // legge dal programma da quale flask deve pompare
  flasks[DETERSIVO] = bitRead(prg, DETERSIVO);
  flasks[ANTICALCARE] = bitRead(prg, ANTICALCARE);
  flasks[IGIENIZZANTE] = bitRead(prg, IGIENIZZANTE);
  flasks[AMMORBIDENTE] = bitRead(prg, AMMORBIDENTE);
  activeFlask = 0;  // inizializza con la prima flask
}

void Dispenser::stop() {
  active = false;

  //ferma tutte le pompe
  return 0x00;
}

uint8_t Dispenser::run(float weight) {
  if (!active) return 0x00;

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
    return stop();
  }

  float amount = weight - tare;
  bool full = ((dosages[activeFlask] * multiplier) - amount) <= 0;

  if (full) {
    flasks[activeFlask] = 0;  // segna il flask come completato
    return 0x00;              // disattiva la pompa
    tare = weight;            // azzera la tara
  } else {
    return pumps[activeFlask];  // avvia la pompa
  }
}


float Dispenser::setMultiplier(uint8_t level) {
  return level * (2 / 1023);
}
