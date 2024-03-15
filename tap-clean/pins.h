// LCD
// SDA => A4
// SCL => A5

// PIN SHIFT REGISTER SN74HC595 per i LED (ingresso seriale uscita parllela)
const int latch = 9;   // ST_CP
const int clock = 10;  // SH_CP
const int data = 8;    // DS pin dati

// BUTTONS
const int RUN_BTN_PIN = A3;  // pin del bottone RUN
const int PRG_BTN_PIN = A2;  // pin del bottone PRG
const int LVL_POT_PIN = A1;  // pin del potenziometro LVL

// SCALE (BILANCIA)
const int SCALE_DATA_PIN = A0;   // pin dei dati della bilancia
const int SCALE_CLOCK_PIN = 12;  // pin del clock della bilancia


// BUZZER
const int BUZZ_PIN = 2;  // pin del buzzer

// door
const int DOOR_PIN = 4; // pin dell chiusura porta