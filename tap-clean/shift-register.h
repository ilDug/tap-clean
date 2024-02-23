#ifndef DAG_SHIFT_REGISTER_H
#define DAG_SHIFT_REGISTER_H

#include <Arduino.h>

class ShiftRegister
{
private:
    uint8_t dataPin;
    uint8_t clockPin;
    uint8_t latchPin;
    byte value;
    void write(uint8_t value);

public:
    // Inizializza lo shift register indicando i pin di connessione
    ShiftRegister(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin);

    // Inizializza i pin di connessione
    void init(uint8_t initial_value = 0x00);
    
    // scrive il valore del registro. 
    // @param value: valore da scrivere in esadecimal (0x00 - 0xFF) (0b00000000 - 0b11111111)
    void set(byte value);

    // scrive il valore di un singolo bit del registro
    void setBit(uint8_t index, bool value);
    
    // porta tutti i bit a 0
    void clear(void);

    // porta tutti i bit a 1
    void rise(void);

    // restituisce il valore memorizzato nel registro
    byte getValue(void);

    // unisce una coppia di valori da 4 bit in un unico valore da 8 bit e lo scrive nel registro
    uint8_t join(uint8_t left, uint8_t right);

};

#endif