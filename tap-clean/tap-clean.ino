/**
 * Distributore di detersivi per lavatrice
 */

#include <Wire.h>
#include <LCD_I2C.h>
#include <HX711_ADC.h>

#include "dag-timer.h"
#include "dag-button.h"
#include "cleans.h"
#include "pins.h"
#include "dispenser.h"
#include "lcd.h"

/** Display LCD*/
LCD_I2C _lcd(0x27, 16, 2);                         // SDA => A4: SCL => A5
DagLCD lcd(&_lcd);                                 // istanza del display LCD
Dispenser dispenser;                               // dispenser di detersivi
HX711_ADC scale(SCALE_DATA_PIN, SCALE_CLOCK_PIN);  // inizializza la bilancia
DagButton runBtn(RUN_BTN_PIN, LOW);                // bottone RUN
DagButton prgBtn(PRG_BTN_PIN, LOW);                // bottone PROGRAMMI

uint8_t prg = P1;  // programma di lavaggio
uint16_t lvl;      // livello di dosaggio
uint8_t pumpCode;  // la pompa attiva impostata dal dispenser
float weight;      // peso letto dalla bilancia
bool active;       // lo stato corrente

void beep(int n, int duration = 300);
String version = "[v1.2.5]";
void setup() {
  Serial.begin(9600);
  Serial.print("Inizializzazione TAP-CLEAN ");
  Serial.println(version);

  pinMode(clock, OUTPUT);  // SHIFT REGISTER
  pinMode(latch, OUTPUT);  // SHIFT REGISTER
  pinMode(data, OUTPUT);   // SHIFT REGISTER

  pinMode(RUN_BTN_PIN, INPUT_PULLUP);  // button
  pinMode(PRG_BTN_PIN, INPUT_PULLUP);  // button
  pinMode(LVL_POT_PIN, INPUT);         // potenziometro;

  pinMode(BUZZ_PIN, OUTPUT);  // buzzer

  lcd.init(version);  // inizializza il display LCD

  //BILANCIA
  scale.begin();
  float calibrationValue;                // calibrazione della bilancia
  calibrationValue = 1025.0;             // costante di calibrazione
  unsigned long stabilizingtime = 2000;  // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true;                  //set this to false if you don't want tare to be performed in the next step
  scale.start(stabilizingtime, _tare);
  if (scale.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    _lcd.clear();
    _lcd.home();
    _lcd.print("scale error");
    while (1) {};
  } else {
    scale.setCalFactor(calibrationValue);  // set calibration value (float)
    Serial.println("Bilancia inizializzata");
  }

  active = false;
  beep(1);
}

void loop() {
  delay(300);
  lvl = analogRead(LVL_POT_PIN);          // legge il valore del potenziometro e lo inverte
  prgBtn.onPress(loopPrograms);           // loop dei programmi alla presione del tasto
  runBtn.onPress(executeProgram);         // bottone del RUN
  runBtn.onLongPress(stopProgram, 1000);  // STOP PROGRAMMA

  if (scale.update()) {
    weight = scale.getData();
  }

  if (dispenser.running()) {  // quando il dispenser è attivato
    active = true;
    if (scale.update()) {
      weight = scale.getData();  // legge il peso
    }
    pumpCode = dispenser.run(weight);  // passa il peso al dispenser e ottiene il codice di pompa attiva
    pumpController(pumpCode);          // gestione dele pompe
    lcd.programProgress(&dispenser);   // visualizza il progresso del programma attivo sul display LCD

  } else {
    if(active){
      beep(3);
      active = false;
    }
    pumpController(OFF);     // spegne tutto
    lcd.mainPage(prg, lvl);  // visualizza il programma selezionato e il livello di dosaggio
  }
}

// cambia a rotazione il programma da attivare  e lo salva nella variabile globale prg
void loopPrograms() {
  int n = sizeof programs;
  for (int i = 0; i < n; i++) {
    if (programs[i] == prg) {
      prg = i >= (n - 1) ? programs[0] : programs[i + 1];
      break;
    }
  }
  beep(1);
}

// esegue il programma selezionato
void executeProgram() {
  beep(1, 750);
  if (scale.update()) {
    weight = scale.getData();  // legge il peso
  }
  dispenser.start(prg, lvl, weight);
}

// ferma il dispenser
void stopProgram() {
  pumpController(dispenser.stop());
  beep(3);
}

// scrive il valore nello SHIFT REGISTER per accendere i led e le pompe.
// @param code 4bit codice della pompa attiva 0x0000
uint8_t pumpController(uint8_t code) {
  /** Aggrega le informazioni sul programma di lavaggio e sul livello di dosaggio, restituendo un byte */
  uint8_t led = code << 4;     // passa i primi 4 bit a destra del byte: LEVEL.
  uint8_t value = led + code;  //  aggiunge al byte le cifre di sinistra: PROGRAM

  digitalWrite(latch, LOW);                // attiva la scrittura dei dati (quando è LOW)
  shiftOut(data, clock, MSBFIRST, value);  // scrive i dati  (MSBFIRST è l'ordine dei dati dalla cifra che pesa di più)
  digitalWrite(latch, HIGH);               // disabilita la scrittura dei dati (quando è HIGH)

  return value;
}


// fa suonare il Buzzer per 200 ms e attende 200 ms per un numero di volte passato come argomento
// @param n numero di volte che il buzzer suona
// @param duration durata del suono
void beep(int n, int duration = 300) {
  int delayTime = duration;
  for (int i = 0; i < n; i++) {
    digitalWrite(BUZZ_PIN, HIGH);
    delay(delayTime);
    digitalWrite(BUZZ_PIN, LOW);
    delay(delayTime);
  }
}
