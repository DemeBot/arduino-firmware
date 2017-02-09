#include <AccelStepper.h>
#include <MultiStepper.h>
#include "pins.h"
#include "constants.h"
#include "functions.h"
#include "setup.h"

void loop () {
  long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    if (flag) {
      Pump_On();
      Vac_On();
      previousMillis = currentMillis;
      flag = false;
    }
    else {
      Pump_Off();
      Vac_Off();
      previousMillis = currentMillis;
      flag = true;
    }
  }

  if (digitalRead(R_MIN_PIN)){
    R_Dir_In();
    LED_Flash();
  }
  if (digitalRead(R_MAX_PIN)){
    R_Dir_Out();
    LED_Flash();
  }
  if (digitalRead(Z_MIN_PIN)){
    Z_Dir_Up();
    LED_Flash();
  }
  if (digitalRead(Z_MAX_PIN)){
    Z_Dir_Down();
    LED_Flash();
  }
  if (digitalRead(THETA_MIN_PIN)){
    DC_Motor_Counterclockwise();
    LED_Flash();
  }
  if (digitalRead(THETA_MAX_PIN)){    
    DC_Motor_Clockwise();
    LED_Flash();
  }

  Step_R();
  Step_Z();


//  if (R_STEPPER.distanceToGo() == 0){
//    if (R_STEPPER.currentPosition() == 0)
//      R_STEPPER.moveTo(R_End);
//    else
//      R_STEPPER.moveTo(0);
//  }
//  R_STEPPER.run();
//
//  if (Z_STEPPER.distanceToGo() == 0){
//    if (Z_STEPPER.currentPosition() == 0)
//      Z_STEPPER.moveTo(Z_Bottom);
//    else
//      Z_STEPPER.moveTo(0);
//  }
//  Z_STEPPER.run();
}
