#ifndef DAG_TIMER_H
#define DAG_TIMER_H

#include "Arduino.h"

class DagTimer {
private:
  // variabile che salva il time per il confronto del loop successivo
  unsigned long bookmark;

  // Durata del timer in millisecondi
  unsigned long duration;

  // funzione da eseguire allo scattare del timer
  void (*call_back)(void);

  // indica se il timer è attivo (sta eseguendo la funzione di callback)
  int FIRED;

  //indica se il timer deve ripetersi ciclicament
  bool LOOP;

public:
  // constructor
  DagTimer();

  // inizializza la durata del timer e lo fa partire. In default è attivo il LOOP.
  void init(unsigned long time_duration);

  // inizializza la drat adel timer e lo fa partire, il secondo parametro indica se è attivo il LOOP
  void init(unsigned long time_duration, bool repeat);

  // restituisce un booleano che verifica se il periodo è scoccato.
  bool clock();

  // esegue la funzione passata come argomento allo scoccare del periodo
  void run(void (*fun)(void));

  // per i timer che non si ripetono ,  indica se è già scoccato almeno un volta
  bool exhausted();

};

#endif