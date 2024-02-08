#ifndef TAP_CLEAN_LED_H
#define TAP_CLEAN_LED_H

/**
Definizione di BATCH
un byte contenente le informazioni in questo ordine

                       1 0 0 0   0 0 0 0
 Q7 PWR            ____| | | |   | | | |
 Q6 LVL3           ______| | |   | | | |
 Q5 LVL2           ________| |   | | | |
 Q4 LVL1           __________|   | | | |
                                 | | | |
 Q3 AMMORBIDENTE   ______________| | | |
 Q2 IGIENIZZANTE   ________________| | |
 Q1 ANTICALCARE    __________________| |
 Q0 DETERSIVO      ____________________|
*/




/** LEDS
PROGRAMMI

| PIN | LED          | P1   | P2   | P3   | CLN  | (blink) |
|-----|--------------|------|------|------|------|---------|
| Q0  | DETERSIVO    | x    |      | x    | x    | (0x01)  |
| Q1  | ANTICALCARE  | x    |      | x    | x    | (0x02)  |
| Q2  | IGIENIZZANTE | x    |      |      | x    | (0x04)  |
| Q3  | AMMORBIDENTE |      | x    |      | x    | (0x08)  |
| --- | ---          | ---  | ---  | ---  | ---  |         |
| HEX |              | 0x07 | 0x08 | 0x03 | 0x0F |         |
| BIN |              | 0111 | 1000 | 0011 | 1111 |         |
------------------------------------------------------------
*/


const uint8_t P1 = 0x07;    // programma classico con tutti e tre ingredienti
const uint8_t P2 = 0x08;    // programma solo ammorbidente
const uint8_t P3 = 0x03;    // programma di lavaggio che non mete l'igienizzante che può essere sostituito dalla candeggina.
const uint8_t CLN = 0x0f;   // progamma di lavaggio che attiva tutte le pompe
const uint8_t OFF = 0x00;   // tutti i led spenti
const uint8_t BLK0 = 0x01;  // blink del led 1
const uint8_t BLK1 = 0x02;  // blink del led 2
const uint8_t BLK2 = 0x04;  // blink del led 3
const uint8_t BLK3 = 0x08;  // link dele led 4

/**
LIVELLI

| PIN | LED  | LG   | MD   | SM   |
|-----|------|------|------|------|
| Q4  | PWR  | x    | x    | x    |
| Q5  | LVL3 | x    |      |      |
| Q6  | LVL2 | x    | x    |      |
| Q7  | LVL1 | x    | x    | x    |
| --- | ---  | ---  | ---  | ---  |
| HEX |      | 0x0F | 0x0D | 0x09 |
| BIN |      | 1111 | 1101 | 1001 |
-----------------------------------
*/
const uint8_t LG = 0x0f;
const uint8_t MD = 0x0d;
const uint8_t SM = 0x09;

#endif
