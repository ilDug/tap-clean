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

// BUTTONS
const int RUN_BTN_PIN = 0;  // pin del bottone RUN
const int PRG_BTN_PIN = 0;  // pin del bottone PRG
const int LVL_BTN_PIN = 0;  // pin del bottone LVL


const int SCALE_DATA_PIN = 0;
const int SCALE_CLOCK_PIN = 0;

enum Flask {
  DETERSIVO = 0,
  ANTICALCARE = 1,
  IGIENIZZANTE = 2,
  AMMORBIDENTE = 3
};