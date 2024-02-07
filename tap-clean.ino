/**
* Distributore di detersivi per lavatrice
*/

#include "dag-timer.h"
#include "dag-button.h"
#include "tap-clean-led.h"

//  PIN POMPE (al relay)
const int PUMP_DET_PIN = 8;   // pin relay pompa detersivo
const int PUMP_ANC_PIN = 9;   // pin relay pompa anticlacare
const int PUMP_IGZ_PIN = 10;  // pin relay pompa igienizzante
const int PUMP_AMM_PIN = 11;  // pin relay pompa ammorbidente

// PIN SHIFT REGISTER SN74HC595 per i LED (ingresso seriale uscita parllela)
const int data = 0;   // DS pin dati
const int latch = 0;  // ST_CP
const int clock = 0;  // SH_CP
// const int enable = 0 ; // abilita tutto il registro (quando è al GND)

const int RUN_PIN = 0;  // pin del bottone RUN
DagButton runBtn(RUN_PIN);

const int PRG_PIN = 0;  // pin del bottone PRG
DagButton prgBtn(PRG_PIN);

const int LVL_PIN = 0;  // pin del bottone LVL
DagButton lvlBtn(LVL_PIN);

uint8_t programs[] = { P1, P2, P3, CLN };
uint8_t levels[] = { SM, MD, LG };
uint8_t activeProgram = P1;
int prg = 1; // 

uint8_t activeLevel = MD;
uint8_t defaultBatch;

String version = "[v0.0.1]";
void setup() {
  Serial.begin(9600);
  Serial.print("Inizializzazione TAP-CLEAN ");
  Serial.println(version);

  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);

  // inizializza i led
  defaultBatch = createBatch(activeProgram, activeLevel);
  ledCtrl(defaultBatch);
}

void loop() {
  delay(50);
}


void selectProgram() {
  for (int i = 0; i < sizeof programs; i++) {
    
  }
}


/** Aggrega le informazioni sul programma di lavaggio e sul livello di dosaggio, restituendo un byte */
uint8_t createBatch(uint8_t program, uint8_t level) {
  uint8_t sx = level << 4;       // passa i primi 4 bit a destra del byte: LEVEL.
  uint8_t value = sx + program;  //  aggiunge al byte le cifre di sinistra: PROGRAM
  return value;
}


// scrive il valore nello SHIFT REGISTER per accendere i led
uint8_t ledCtrl(uint8_t batch) {
  digitalWrite(latch, LOW);                // attiva la scrittura dei dati (quando è LOW)
  shiftOut(data, clock, MSBFIRST, batch);  // scrive i dati  (MSBFIRST è l'ordine dei dati dalla cifra che pesa di più)
  digitalWrite(latch, HIGH);               // disabilita la scrittura dei dati (quando è HIGH)
}
