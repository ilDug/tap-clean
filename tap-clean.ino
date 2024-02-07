/**
* Distributore di detersivi per lavatrice
*/

#include "dag-timer.h"
#include "dag-button.h"
#include "tab-clean-led.h"

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


String version = "[v0.0.1]";
void setup() {
  Serial.begin(9600);
  Serial.print("Inizializzazione TAP-CLEAN ");
  Serial.println(version);

  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);

  // inizializza i led
  ledCtrl(P1, MD);


}


void loop() {
  delay(50);
  ledCtrl(P1, MD);
}



/**
Per sommare il risultato devo fare 
- il secondo gruppo con lo scorrimento a sinistra di quatro posizioni poi devo sommare il promo gruppo

uint8_t x = 0x0f << 4;
uint8_t y = x + 0x07;
y => 1111 0111
*/


// scrive il valore nello SHIFT REGISTER per accendere i led
uint8_t ledCtrl(uint8_t program, uint8_t level) {
  uint8_t x = level << 4;
  uint8_t value = x + program;
  digitalWrite(latch, LOW);                // attiva la scrittura dei dati (quando è LOW)
  shiftOut(data, clock, MSBFIRST, value);  // scrive i dati  (MSBFIRST è l'ordine dei dati dalla cifra che pesa di più)
  digitalWrite(latch, HIGH);               // disabilita la scrittura dei dati (quando è HIGH)
}
