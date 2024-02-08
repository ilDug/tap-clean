//  PIN POMPE (al relay)
const int PUMP_DET_PIN = 9;   // pin relay pompa detersivo
const int PUMP_ANC_PIN = 10;  // pin relay pompa anticlacare
const int PUMP_IGZ_PIN = 11;  // pin relay pompa igienizzante
const int PUMP_AMM_PIN = 12;  // pin relay pompa ammorbidente

// PIN SHIFT REGISTER SN74HC595 per i LED (ingresso seriale uscita parllela)
const int latch = 6;  // ST_CP
const int clock = 7;  // SH_CP
const int data = 8;   // DS pin dati
// const int enable = 0 ; // abilita tutto il registro (quando è al GND)

// BUTTONS
const int RUN_BTN_PIN = 2;  // pin del bottone RUN
const int PRG_BTN_PIN = 3;  // pin del bottone PRG
const int LVL_BTN_PIN = 4;  // pin del bottone LVL

//SCALE
const int SCALE_DATA_PIN = A4;
const int SCALE_CLOCK_PIN = A5;