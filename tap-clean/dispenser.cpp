#include "Arduino.h"
#include "dispenser.h"

Dispenser::Dispenser()
{
}

bool Dispenser::running()
{
    return active;
}

void Dispenser::start(uint8_t program, uint8_t level, float _tare)
{
    prg = program;
    active = true;
    multiplier = setMultiplier(level);
    tare = _tare;
    completion = 0;

    // legge dal programma da quale flask deve pompare
    flasks[DETERSIVO] = bitRead(prg, DETERSIVO);
    flasks[ANTICALCARE] = bitRead(prg, ANTICALCARE);
    flasks[IGIENIZZANTE] = bitRead(prg, IGIENIZZANTE);
    flasks[AMMORBIDENTE] = bitRead(prg, AMMORBIDENTE);
    activeFlask = 0; // inizializza con la prima flask
}

uint8_t Dispenser::stop()
{
    active = false;

    // ferma tutte le pompe
    return OFF;
}

// esegue il dosaggio
// @param weight peso letto dalla bilancia
uint8_t Dispenser::run(float weight)
{
    if (!active)
        return OFF; // se non è attivo non fa nulla

    /**
      seleziona ed incrementa la flask attiva di volta in volta
      la flask diventa 0 se è esclusa dal programma
      oppure può essere impostata 0 quando è stata versata tutta dal loop precedente
    */
    if (flasks[activeFlask] == 0)
    {                                  // se la flask è esclusa dal programma
        activeFlask = activeFlask + 1; // passa alla flask successiva
    }

    // quando ragginge il limite ferma il programma
    if (activeFlask >= sizeof flasks)
    {                  // se ha raggiunto l'ultima flask
        return stop(); // ferma il programma
    }

    activeFlaskLabel = flaskLabels[activeFlask]; // imposta il label della flask attiva

    float amount = weight - tare;                                    // calcola la quantità da dosare
    bool full = ((dosages[activeFlask] * multiplier) - amount) <= 0; // verifica se la flask è piena

    completion = int(100 * amount / (dosages[activeFlask] * multiplier)); // calcola la percentuale di completamento

    if (full)
    {
        flasks[activeFlask] = 0; // segna il flask come completato
        return OFF;              // disattiva la pompa
        tare = weight;           // azzera la tara
    }
    else
    {
        return pumps[activeFlask]; // attiva la pompa corrispondente alla flask attiva
    }
}

// calcola il moltiplicatore per il dosaggio fino ad un massimo di 2
// @param level livello di dosaggio FROM 0 TO 15
float Dispenser::setMultiplier(uint8_t level)
{
    return level * (2 / 15) || 0.1;
}