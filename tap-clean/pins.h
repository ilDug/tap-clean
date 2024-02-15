// LCD
// SDA => A4
// SCL => A5

// PIN SHIFT REGISTER SN74HC595 per i LED (ingresso seriale uscita parllela)
const int latch = 0; // ST_CP
const int clock = 0; // SH_CP
const int data = 0;  // DS pin dati

// BUTTONS
const int RUN_BTN_PIN = 0; // pin del bottone RUN
const int PRG_BTN_PIN = 0; // pin del bottone PRG
const int LVL_POT_PIN = 0; // pin del potenziometro LVL

// SCALE (BILANCIA)
const int SCALE_DATA_PIN = 0;  // pin dei dati della bilancia
const int SCALE_CLOCK_PIN = 0; // pin del clock della bilancia


// BUZZER
const int BUZZ_PIN = 0; // pin del buzzer