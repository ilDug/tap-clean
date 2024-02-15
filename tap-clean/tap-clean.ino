/**
 * Distributore di detersivi per lavatrice
 */

#include <Wire.h>
#include <LCD_I2C.h>

// #include <HX711.h> //https://github.com/RobTillaart/HX711
#include "HX711.h"
#include "dag-timer.h"
#include "dag-button.h"
#include "cleans.h"
#include "pins.h"
#include "dispenser.h"

/** Display LCD*/
LCD_I2C lcd(0x27, 16, 2); // SDA => A4: SCL => A5
Dispenser dispenser;
HX711 scale;
DagButton runBtn(RUN_BTN_PIN, LOW);
DagButton prgBtn(PRG_BTN_PIN, LOW);

uint8_t prg = P1;
uint8_t lvl;
uint8_t pumpCode; // la pompa attiva impostata dal dispenser
float weight;

String version = "[v1.0.1]";
void setup()
{
    Serial.begin(9600);
    Serial.print("Inizializzazione TAP-CLEAN ");
    Serial.println(version);

    pinMode(clock, OUTPUT); // SHIFT REGISTER
    pinMode(latch, OUTPUT); // SHIFT REGISTER
    pinMode(data, OUTPUT);  // SHIFT REGISTER

    pinMode(RUN_BTN_PIN, INPUT_PULLUP); // button
    pinMode(PRG_BTN_PIN, INPUT_PULLUP); // button
    pinMode(LVL_POT_PIN, INPUT);        // potenziometro;

    scale.begin(SCALE_DATA_PIN, SCALE_CLOCK_PIN);
    scale.wait_ready();
    Serial.println("Bilancia inizializzata");

    // LCD
    lcd.begin();                // inizializza LCD
    lcd.backlight();            // accende lo sfondo
    lcd.home();                 // si posrta in prima riga
    lcd.print("DAG TAP-CLEAN"); // messaggio di benvenuto
    lcd.setCursor(0, 1);        // passa alla seconda riga
    lcd.print(version);         // stampa
    delay(3000);                // pausa
    lcd.clear();                // pulisce tutto lo schermo
}

void loop()
{
    delay(50);
    lvl = map(analogRead(LVL_POT_PIN), 0, 1024, 1024, 0); // legge il valore del potenziometro
    prgBtn.onPress(loopPrograms);                         // loop dei programmi alla presione del tasto
    runBtn.onPress(executeProgram);                       // bottone del RUN
    runBtn.onLongPress(stopProgram, 1000);                // STOP PROGRAMMA

    if (dispenser.running())
    {                                     // quando il dispenser è attivato
        weight = scale.read();            // legge il peso
        pumpCode = dispenser.run(weight); // passa il peso al dispenser e ottiene il codice di pompa attiva
        pumpController(pumpCode);         // gestione dele pompe
    }
    else
    {
        pumpController(OFF); // spegne tuttto
    }

    display_lcd();
}

// cambia a rotazione il programma da attivare  e lo salva nella variabile globale prg
void loopPrograms()
{
    int n = sizeof programs;
    for (int i = 0; i < n; i++)
    {
        if (programs[i] == prg)
        {
            prg = i >= (n - 1) ? programs[0] : programs[i + 1];
            break;
        }
    }
}

void executeProgram()
{
    weight = scale.read();
    dispenser.start(prg, lvl, weight);
}

void stopProgram()
{
    pumpController(dispenser.stop());
}

// scrive il valore nello SHIFT REGISTER per accendere i led e le pompe.
// @param code 4bit codice della pompa attiva 0x0000
uint8_t pumpController(uint8_t code)
{
    /** Aggrega le informazioni sul programma di lavaggio e sul livello di dosaggio, restituendo un byte */
    uint8_t led = code << 4;    // passa i primi 4 bit a destra del byte: LEVEL.
    uint8_t value = led + code; //  aggiunge al byte le cifre di sinistra: PROGRAM

    digitalWrite(latch, LOW);               // attiva la scrittura dei dati (quando è LOW)
    shiftOut(data, clock, MSBFIRST, value); // scrive i dati  (MSBFIRST è l'ordine dei dati dalla cifra che pesa di più)
    digitalWrite(latch, HIGH);              // disabilita la scrittura dei dati (quando è HIGH)

    return value;
}

void display_lcd()
{
    lcd.clear();                          // cancella tutto
    lcd.home();                           // prima riga
    lcd.print(info_lcd(prg));             // stampa la descrizione
    lcd.setCursor(0, 1);                  // va a capo
    int lungh = map(lvl, 0, 1024, 1, 16); // calcola la lunghezza della barra
    char bar[16];                         // definisce i caratteri barra
    for (int i = 0; i < lungh; i++)
    { // riempie il caratteri barra
        bar[i] = '+';
    }
    lcd.print(bar);
}

String info_lcd(uint8_t program)
{
    switch (program)
    {
    case P1:
        return "DET + ANC + IGZ";
        break;
    case P2:
        return "AMMORBIDENTE";
        break;
    case P3:
        return "DET + ANTICALC";
        break;
    case CLN:
        return "CLEAN ALL";
        break;
    default:
        return "nessun PRG";
        break;
    }
}

void progres_lcd()
{
}
