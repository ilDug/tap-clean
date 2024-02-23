#ifndef DAG_POTENTIOMETER_H
#define DAG_POTENTIOMETER_H

#include "Arduino.h"

class DagPot {
public:
    // Constructor. Istanzia un oggetto di tipo DagPot,  ed initializza il pin del potenziometro come analog input.
    DagPot(uint8_t pin);

    // Constructor. Istanzia un oggetto di tipo DagPot,  ed initializza il pin del potenziometro come analog input. Definisce direttamente la mappa dei valori di uscita minimi e massimi. Definisce anche il delta di sensibilità per il cambio di valore.
    DagPot(uint8_t pin, long delta);

    // inizializza il potenziometro, definendo la mappa dei valori di uscita minimi e massimi.(default 0-1023). Se max è minore di min,  inverte la scala di valori.
    void init(long map_min, long map_max);

    // inizializza il potenziometro, definendo la mappa dei valori di uscita minimi e massimi.(default 0-1023). Se max è minore di min,  inverte la scala di valori. Definisce anche il delta di sensibilità per il cambio di valore.
    void init(long map_min, long map_max, long delta);

    // imposta il delta di sensibilità per il cambio di valore.
    void set_delta(long delta);

    // restituisce ISTANTANEO il valore del potenziometro, mappato tra min e max.
    long read(void);

    // restituisce ISTANTANEO il VALORE RAW del potenziometro (senza mappatura).
    long read_raw(void);

    // il valore memorizzato del potenziometro, mappato tra min e max.
    long value(void);

    // indica se il valore è cambiato rispetto all'ultimo salvat.
    bool changed(void);

    // esegue la funzione di callback per il cambio di valore del potenziometro. Esegue solo al primo camio dopo ogni salvataggio
    void onChange(void (*fun)(void));

    // imposta la funzione di callback per il cambio di valore del potenziometro. Definisce anche se la funzione deve essere ripetuta ad ogni loop o solo al cambio dopo ogni salvataggio.
    void onChange(void (*fun)(void), bool repeat);

    // salva il valore attuale del potenziometro nella memoria volatile
    void save(void);   

    // annulla il valore memorizzato del potenziometro
    void cancel(void); 

private:
    // valore memorizzato del potenziometro,  raw
    int val;

    // il pin di lettura del potentiometro
    int pin;

    // valore minimo della mappa
    long min;

    // valore massimo della mappa
    long max;

    // indica se esiste un valore memorizzato
    bool STORED;

    // indica se il valore è cambiato rispetto all'ultimo salvataggio.
    bool CHANGED;

    // indica se la call_back è già stata eseguita    
    bool FIRED;

    // indica se la call_back deve essere ripetuta ad ogni loop
    bool LOOP;

    // delta di sensibilità per il cambio di valore
    long dx;

    // imposta i valori interni della mappa
    void set_minmax(long map_min, long map_max);

};

#endif // DAG_POTENTIOMETER_H
