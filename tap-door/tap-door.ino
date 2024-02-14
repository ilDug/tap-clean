
/** TAP-DOOR */
#include <Stepper.h>
#include "door.h"
#include "dag-timer.h"
#include "dag-button.h"

// stepper
const int MOTOR_PIN_1 = 0;
const int MOTOR_PIN_2 = 0;
const int MOTOR_PIN_3 = 0;
const int MOTOR_PIN_4 = 0;

const int TRIGGER_PIN = 9;
const int ECO_PIN = 10;

const int STP = 200;  // step per revolution del motore stepper.
const int DOOR_LIM_SWITCH = 0;
const int TOGGLE_BTN_PIN = 0;

const int WAIT = 1000 * 1;
const int LIMIT = 0;

Stepper motor(STP, MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3, MOTOR_PIN_4);
Door door(&motor);
DagButton btn(TOGGLE_BTN_PIN, LOW);

bool FIRED = false;  // registra quando è attivato il passaggio con la mano

void setup() {
  Serial.begin(9600);
  Serial.println("TAP-DOOR init...");

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECO_PIN, INPUT);
  pinMode(DOOR_LIM_SWITCH, INPUT_PULLUP);
  pinMode(TOGGLE_BTN_PIN, INPUT_PULLUP);

  door.init(DOOR_LIM_SWITCH);
}


void loop() {

  // misurare la distanza;
  // se la distanza è minore di una costante,
  // azionare il toggle
  // restare in attesa per il tempo che uno toglie la mano

  while (isClose()) {  // sta fermo fino a quando non tolgo la mano
    FIRED = true;
    delay(10);
  }

  if (FIRED) {
    door.toggle();  // aziona l'apertura
    delay(WAIT);    // attende er il tempo che uno ci mette a togliere la mano
    FIRED = false;
  }

  // uando si preme il pulsante usa il toggle della door.
  btn.onPress(toggle);
}


bool isClose(void) {
  // misura
  // se è sotto il limite restitusce true altrimenti false
}

void toggle(void) {
  door.toggle();
}
