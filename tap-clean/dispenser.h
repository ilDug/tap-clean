#include <stdint.h>
#ifndef DAG_DISPENSER_H
#define DAG_DISPENSER_H

#include "Arduino.h"
#include "cleans.h"

class Dispenser
{

private:
    // stato del programma
    bool active = false;

    // programma di dosaggio
    uint8_t prg = 0x00;

    // moltiplicatore del dosaggio
    float multiplier = 1.0;

    // valore iniziale del dosaggio di ogni songolo flask
    float tare;

    // imposta il moltiplicatore in vase al level passato con start. level da 0 a 1023.
    float setMultiplier(uint8_t level);

    // array che indica quali flask devno essere utilizzati  nel programma
    bool flasks[4];

    // bottiglia attiva
    int activeFlask;

public:
    // construtor
    Dispenser();

    // Avvia il programma,  settando lo stato active
    // @param program : il programma in formato 4bit (0x0000)
    // @param level : il livello di dosaggio in formato 8bit (da 0 a 1023)
    // @param tare: la tara iniziale dela pesa.
    void start(uint8_t program, uint8_t level, float _tare);

    // ferma il programma settando lo stato active to false
    uint8_t stop(void);

    // loop controller, restituisce il codice di accensione delle pomper e dei led da passare alllo Shift Register
    uint8_t run(float weight);

    // restituisce lo stato active
    bool running(void);

    // percentuale di completamento del programma di un singolo flask
    float completion;

    // restituisce il label della flask attiva
    String activeFlaskLabel;
};

#endif