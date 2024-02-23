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
#include "lcd.h"
#include "dag-potentiometer.h"
#include "shift-register.h"

/** Display LCD*/
LCD_I2C _lcd(0x27, 16, 2);          // SDA => A4: SCL => A5
DagLCD lcd(&_lcd);                  // istanza del display LCD
Dispenser dispenser;                // dispenser di detersivi
HX711 scale;                        // bilancia
DagButton runBtn(RUN_BTN_PIN, LOW); // bottone RUN
DagButton prgBtn(PRG_BTN_PIN, LOW); // bottone PROGRAMMI
DagPot lvl(LVL_POT_PIN);            // potenziometro per il livello di dosaggio
ShiftRegister pumpCtrl(clock, data, latch);

uint8_t prg = P1; // programma di lavaggio
uint8_t pumpCode; // la pompa attiva impostata dal dispenser

void beep(int n, int duration = 300);

String version = "[v1.2.3]";
void setup()
{
    Serial.begin(9600);
    Serial.print("Inizializzazione TAP-CLEAN ");
    Serial.println(version);

    pinMode(BUZZ_PIN, OUTPUT); // buzzer

    lvl.init(0, 15); // inizializza il potenziometro per il livello di dosaggio
    pumpCtrl.init(); // inizializza lo shift register

    scale.begin(SCALE_DATA_PIN, SCALE_CLOCK_PIN); // inizializza la bilancia
    scale.wait_ready();                           // attende che la bilancia sia pronta
    Serial.println("Bilancia inizializzata");     // messaggio di avvenuta inizializzazione della bilancia

    lcd.init(version); // inizializza il display LCD
    beep(1);
}

void loop()
{
    delay(50);
    prgBtn.onPress(loopPrograms);          // loop dei programmi alla presione del tasto
    runBtn.onPress(executeProgram);        // bottone del RUN
    runBtn.onLongPress(stopProgram, 1000); // STOP PROGRAMMA

    if (dispenser.running())
    {                                           // quando il dispenser è attivato
        pumpCode = dispenser.run(scale.read()); // passa il peso al dispenser e ottiene il codice di pompa attiva
        pumpCtrl.join(pumpCode, pumpCode);      // accende la pompa corrispondente
        lcd.programProgress(&dispenser);        // visualizza il progresso del programma attivo sul display LCD
    }
    else
    {
        pumpCtrl.set(OFF);             // spegne le pompe
        lcd.mainPage(prg, lvl.read()); // visualizza il programma selezionato e il livello di dosaggio
    }
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
    beep(1);
}

// esegue il programma selezionato
void executeProgram()
{
    beep(1, 750);
    dispenser.start(prg, lvl.read(), scale.read());
}

// ferma il dispenser
void stopProgram()
{
    pumpCtrl.set(dispenser.stop()); // spegne le pompe
    beep(3);
}

// fa suonare il Buzzer per 200 ms e attende 200 ms per un numero di volte passato come argomento
// @param n numero di volte che il buzzer suona
// @param duration durata del suono
void beep(int n, int duration = 300)
{
    int delayTime = duration;
    for (int i = 0; i < n; i++)
    {
        digitalWrite(BUZZ_PIN, HIGH);
        delay(delayTime);
        digitalWrite(BUZZ_PIN, LOW);
        delay(delayTime);
    }
}
