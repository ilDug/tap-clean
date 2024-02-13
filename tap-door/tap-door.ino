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

const int STP = 200; // step per revolution del motore stepper.
const int DOOR_LIM_SWITCH = 0;

Stepper motor(STP, MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3, MOTOR_PIN_4);
Door door(&motor);


void setup() {


  
  pinMode(DOOR_LIM_SWITCH, INPUT_PULLUP);
  door.init(DOOR_LIM_SWITCH);

}


void loop() {
}
