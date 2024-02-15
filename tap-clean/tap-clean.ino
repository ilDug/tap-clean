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
LCD_I2C lcd(0x27, 16, 2);           // SDA => A4: SCL => A5
Dispenser dispenser;                // dispenser di detersivi
HX711 scale;                        // bilancia
DagButton runBtn(RUN_BTN_PIN, LOW); // bottone RUN
DagButton prgBtn(PRG_BTN_PIN, LOW); // bottone PROGRAMMI

uint8_t prg = P1; // programma di lavaggio
uint8_t lvl;      // livello di dosaggio
uint8_t pumpCode; // la pompa attiva impostata dal dispenser
float weight;     // peso letto dalla bilancia

String version = "[v1.1.3]";
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

    pinMode(BUZZ_PIN, OUTPUT); // buzzer

    scale.begin(SCALE_DATA_PIN, SCALE_CLOCK_PIN); // inizializza la bilancia
    scale.wait_ready();                           // attende che la bilancia sia pronta
    Serial.println("Bilancia inizializzata");     // messaggio di avvenuta inizializzazione della bilancia

    init_lcd(); // inizializza il display LCD
}

void loop()
{
    delay(50);
    lvl = map(analogRead(LVL_POT_PIN), 0, 1024, 1024, 0); // legge il valore del potenziometro e lo inverte
    prgBtn.onPress(loopPrograms);                         // loop dei programmi alla presione del tasto
    runBtn.onPress(executeProgram);                       // bottone del RUN
    runBtn.onLongPress(stopProgram, 1000);                // STOP PROGRAMMA

    if (dispenser.running())
    {                                     // quando il dispenser è attivato
        weight = scale.read();            // legge il peso
        pumpCode = dispenser.run(weight); // passa il peso al dispenser e ottiene il codice di pompa attiva
        pumpController(pumpCode);         // gestione dele pompe
        progress_lcd();                   // visualizza il progresso del programma
    }
    else
    {
        pumpController(OFF); // spegne tuttto
        display_lcd();       // visualizza il programma selezionato e il livello di dosaggio
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
    weight = scale.read();
    dispenser.start(prg, lvl, weight);
}

// ferma il dispenser
void stopProgram()
{
    pumpController(dispenser.stop());
    beep(3);
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

// visualizza il programma selezionato e il livello di dosaggio sul display LCD
void display_lcd()
{
    lcd.clear();                          // cancella tutto lo schermo
    lcd.home();                           // prima riga e prima colonna
    lcd.print(info_lcd(prg));             // stampa la descrizione del programma
    lcd.setCursor(0, 1);                  // va a capo sulla seconda riga
    int lungh = map(lvl, 0, 1024, 1, 16); // calcola la lunghezza della barra in base al valore del potenziometro
    char bar[16];                         // definisce i caratteri barra da stampare
    for (int i = 0; i < lungh; i++)
    {                 // ciclo per la lunghezza della barra in base al valore del potenziometro
        bar[i] = '+'; // riempie la barra con i caratteri '+'
    }
    lcd.print(bar); // stampa la barra
}

// restituisce la descrizione del programma selezionato in base al codice passato
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

// visualizza il progresso del programma attivo sul display LCD
void progress_lcd()
{
    lcd.clear();
    lcd.home();
    lcd.print(dispenser.activeFlaskLabel + " " + dispenser.completion + "%");
    lcd.setCursor(0, 1);
    char bar[16];
    int percentage = int(dispenser.completion / 100 * 16);
    for (int i = 0; i < percentage; i++)
    {
        bar[i] = '+';
    }
    lcd.print(bar);
}

// inizializza il display LCD con il messaggio di benvenuto
void init_lcd()
{
    lcd.begin();                // inizializza LCD
    lcd.backlight();            // accende lo sfondo
    lcd.home();                 // si posrta in prima riga
    lcd.print("DAG TAP-CLEAN"); // messaggio di benvenuto
    lcd.setCursor(0, 1);        // passa alla seconda riga
    lcd.print(version);         // stampa
    beep(1);                    // fa suonare il buzzer
    delay(3000);                // pausa
    lcd.clear();                // pulisce tutto lo schermo
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
