/**
* Distributore di detersivi per lavatrice
*/

#include "dag-timer.h"
#include "dag-button.h"

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

/** LEDS
PROGRAMMI
PIN     LED          P1    P2    P3    CLN     (blink del singolo led)
---     ---          ---   ---   ---   ---
Q0      DETERSIVO     x           x     x     (0x01)
Q1      ANTICALCARE   x           x     x     (0x02)
Q2      IGIENIZZANTE  x                 x     (0x04)
Q3      AMMORBIDENTE        x           x     (0x08)
---     ---          ---   ---   ---   ---
HEX                  0x07  0x08  0x03 0x0F
------------------------------------------
*/
uint8_t P1 = 0x07; 
uint8_t P2 = 0x08;
uint8_t P3 = 0x03;
uint8_t CLN = 0x0f;
uint8_t OFF = 0x00;
uint8_t BLK0 = 0x01;
uint8_t BLK1 = 0x02;
uint8_t BLK2 = 0x04;
uint8_t BLK3 = 0x08;

/**
LIVELLI
PIN     LED          SM    MD    LG    
---     ---          ---   ---   ---   
Q4      PWR           x     x     x
Q5      LVL1          x
Q6      LVL2          x     x
Q7      LVL3          x     x     x
---     ---          ---   ---   ---   
HEX                  0x0F  0x0D  0x09     
------------------------------------------
*/
uint8_t  SM = 0x0f;
uint8_t  MD = 0x0d;
uint8_t  LG = 0x09;


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
