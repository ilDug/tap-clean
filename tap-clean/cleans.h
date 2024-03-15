#ifndef TAP_CLEAN_PRG_H
#define TAP_CLEAN_PRG_H

#include <Arduino.h>

/**
Definizione di BATCH
un byte contenente le informazioni in questo ordine

                       1 0 0 0   1 0 0 0
 Q7 LED_AMM        ____| | | |   | | | |
 Q6 LED_IGZ        ______| | |   | | | |
 Q5 LED_ANC        ________| |   | | | |
 Q4 LED_DET        __________|   | | | |
                                 | | | |
 Q3 PUMP_AMM       ______________| | | |
 Q2 PUMP_IGZ       ________________| | |
 Q1 PUMP_ANC       __________________| |
 Q0 PUMP_DET       ____________________|
*/

/** 
PROGRAMMI

| PIN | LED          | P1   | P2   | P3   | CLN  | 
|-----|--------------|------|------|------|------|
| Q0  | DETERSIVO    | x    |      | x    | x    |
| Q1  | ANTICALCARE  | x    |      | x    | x    |
| Q2  | IGIENIZZANTE | x    |      |      | x    |
| Q3  | AMMORBIDENTE |      | x    |      | x    |
| --- | ---          | ---  | ---  | ---  | ---  |
| HEX |              | 0x07 | 0x08 | 0x03 | 0x0F |
| BIN |              | 0111 | 1000 | 0011 | 1111 |
--------------------------------------------------
*/

const uint8_t P1 = 0x07;   // programma classico con tutti e tre ingredienti
const uint8_t P2 = 0x08;   // programma solo ammorbidente
const uint8_t P3 = 0x03;   // programma di lavaggio che non mette l'igienizzante che può essere sostituito dalla candeggina.
const uint8_t CLN = 0x0f;  // progamma di lavaggio che attiva tutte le pompe
const uint8_t OFF = 0x00;  // tutti i led spenti

const uint8_t programs[] = { P1, P2, P3, CLN };

// array dei pin delle pompe
const int pumps[4] = { 0x01, 0x02, 0x04, 0x08 };

// elenco delle bottiglie
enum Flask {
  DETERSIVO = 0,
  ANTICALCARE = 1,
  IGIENIZZANTE = 2,
  AMMORBIDENTE = 3
};


// dosaggi tipici in grammi.
const float dosages[4] = {
  60,  // DETERSIVO
  50,  // ANTICALCARE
  60,  // IGIENIZZANTE
  50   // AMMORBIDENTE
};

#endif