
/**
* Distributore di detersivi per lavatrice
*/

//#include <HX711.h> //https://github.com/RobTillaart/HX711
#include "HX711.h"
#include "dag-timer.h"
#include "dag-button.h"
#include "leds.h"
#include "pins.h"
#include "dispenser.h"
#include <Stepper.h>
#include "door.h"


HX711 scale;
Dispenser dispenser(PUMP_DET_PIN, PUMP_ANC_PIN, PUMP_IGZ_PIN, PUMP_AMM_PIN);
Stepper motor(STP, MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3, MOTOR_PIN_4);
Door door(&motor);
DagButton runBtn(RUN_BTN_PIN, LOW);
DagButton prgBtn(PRG_BTN_PIN, LOW);
DagButton lvlBtn(LVL_BTN_PIN, LOW);

uint8_t prg = P1;
uint8_t lvl = MD;
bool blinking = false;
int activePump;
float weight;

String version = "[v0.1.2]";
void setup() {
  Serial.begin(9600);
  Serial.print("Inizializzazione TAP-CLEAN ");
  Serial.println(version);

  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);

  pinMode(RUN_BTN_PIN, INPUT_PULLUP);
  pinMode(PRG_BTN_PIN, INPUT_PULLUP);
  pinMode(LVL_BTN_PIN, INPUT_PULLUP);

  pinMode(PUMP_DET_PIN, OUTPUT);  // pin relay pompa detersivo
  pinMode(PUMP_ANC_PIN, OUTPUT);  // pin relay pompa anticlacare
  pinMode(PUMP_IGZ_PIN, OUTPUT);  // pin relay pompa igienizzante
  pinMode(PUMP_AMM_PIN, OUTPUT);  // pin relay pompa ammorbidente

  digitalWrite(PUMP_DET_PIN, HIGH);
  digitalWrite(PUMP_ANC_PIN, HIGH);
  digitalWrite(PUMP_IGZ_PIN, HIGH);
  digitalWrite(PUMP_AMM_PIN, HIGH);

  pinMode(DOOR_LIM_SWITCH, INPUT_PULLUP);
  door.init(DOOR_LIM_SWITCH);

  scale.begin(SCALE_DATA_PIN, SCALE_CLOCK_PIN);
  scale.wait_ready();
  Serial.println("Bilancia inizializzata");
}

void loop() {
  delay(50);

  prgBtn.onPress(loopPrograms);
  lvlBtn.onPress(loopLevels);
  runBtn.onPress(executeProgram);
  runBtn.onLongPress(stopProgram, 1000);

  if (dispenser.running()) {
    weight = scale.read();
    activePump = dispenser.run(weight);
    ledCtrl(blink(activePump), lvl);
  } else {
    ledCtrl(prg, lvl);
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
}

// cambia a rotazione il livello di dosaggio,  salvandolo nella vriabile globale lvl
void loopLevels() {
  int n = sizeof levels;
  for (int i = 0; i < n; i++) {
    if (levels[i] == prg) {
      lvl = i >= (n - 1) ? levels[0] : levels[i + 1];
      break;
    }
  }
}

void executeProgram() {
  weight = scale.read();
  dispenser.start(prg, lvl, weight);
}

void stopProgram() {
  dispenser.stop();
}

/** resituisce i bit di sequenza per il blinking dei led durante l'avviamento delle pompe */
uint8_t blink(int pump) {
  blinking = !blinking;

  if (blinking) {
    return OFF;
  }

  switch (pump) {
    case -1:
      return prg;
      break;
    case 0:
      return BLK0;
      break;
    case 1:
      return BLK1;
      break;
    case 2:
      return BLK2;
      break;
    case 3:
      return BLK3;
      break;
    default:
      return prg;
      break;
  }
}

// scrive il valore nello SHIFT REGISTER per accendere i led
uint8_t ledCtrl(uint8_t program, uint8_t level) {
  /** Aggrega le informazioni sul programma di lavaggio e sul livello di dosaggio, restituendo un byte */
  uint8_t sx = level << 4;       // passa i primi 4 bit a destra del byte: LEVEL.
  uint8_t value = sx + program;  //  aggiunge al byte le cifre di sinistra: PROGRAM

  digitalWrite(latch, LOW);                // attiva la scrittura dei dati (quando è LOW)
  shiftOut(data, clock, MSBFIRST, value);  // scrive i dati  (MSBFIRST è l'ordine dei dati dalla cifra che pesa di più)
  digitalWrite(latch, HIGH);               // disabilita la scrittura dei dati (quando è HIGH)

  return value;
}
