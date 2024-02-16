#ifndef DAG_LCD_H
#define DAG_LCD_H

#include "Arduino.h"
#include <Wire.h>
#include <LCD_I2C.h>
#include "dispenser.h"

class DagLCD
{
private:
    LCD_I2C *lcd; // Display LCD

    // restituisce la descrizione del programma di lavaggio corrente
    String programDescription(uint8_t prg);


public:
    /**
     * Inizializza il display LCD
     * deve essere passato un istanze d LCD_I2C
     * LCD_I2C lcd(0x27, 16, 2);
     * SDA => A4: SCL => A5
     */
    DagLCD(LCD_I2C *_lcd);
    
    // inizializza il display LCD e mostra il messaggio iniziale di benvenuto
    bool init(String version); 

    // mostra il programma di lavaggio corrente e il livello di dosaggio
    void mainPage(uint8_t progam, uint8_t level);

    // visualizza il progresso del programma attivo sul display LCD
    void programProgress(Dispenser *dispenser);


};

#endif
