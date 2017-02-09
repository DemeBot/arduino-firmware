// ################################### FUNCTIONS ######################################

void Pump_On(){
  digitalWrite(PUMP_PIN           , HIGH);
}

void Pump_Off(){
  digitalWrite(PUMP_PIN           , LOW);
}

void Vac_On(){
  digitalWrite(VAC_PIN            , HIGH);
}

void Vac_Off(){
  digitalWrite(VAC_PIN            , LOW);
}

void R_Dir_In(){
  digitalWrite(R_DIR_PIN          , HIGH);
}

void R_Dir_Out(){
  digitalWrite(R_DIR_PIN          , LOW);
}

void Z_Dir_Up(){
  digitalWrite(Z_DIR_PIN          , HIGH);
}

void Z_Dir_Down(){
  digitalWrite(Z_DIR_PIN          , LOW);
}

void Step_Z(){
  digitalWrite(Z_STEP_PIN         , HIGH);
  delay(1);
  digitalWrite(Z_STEP_PIN         , LOW);
}

void Step_R(){
  digitalWrite(R_STEP_PIN         , HIGH);
  delay(1);
  digitalWrite(R_STEP_PIN         , LOW);
}

void DC_Motor_Clockwise(){
  digitalWrite(THETA_LEFT_PIN     , LOW);
  digitalWrite(THETA_RIGHT_PIN    , HIGH);
}

void DC_Motor_Counterclockwise(){
  digitalWrite(THETA_LEFT_PIN     , HIGH);
  digitalWrite(THETA_RIGHT_PIN    , LOW);
}

void DC_Motor_Stop(){
  digitalWrite(THETA_LEFT_PIN     , HIGH);
  digitalWrite(THETA_RIGHT_PIN    , HIGH);
}

void LED_On(){
  digitalWrite(LED_PIN            , HIGH);
}

void LED_Off(){
  digitalWrite(LED_PIN            , LOW);
}

void LED_Flash(){
  digitalWrite(LED_PIN            , HIGH);
  digitalWrite(LED_PIN            , LOW);
}

void Z_Stepper_Top(){
  while (!digitalRead(Z_MIN_PIN)){
      Z_STEPPER.setSpeed(-zMaxSpeed);
      Z_STEPPER.runSpeed();
      delay(2);
  }
  Z_Bottom = Z_STEPPER.currentPosition();
}

void Z_Stepper_Bottom(){
  while (!digitalRead(Z_MAX_PIN)){
      Z_STEPPER.setSpeed(zMaxSpeed);
      Z_STEPPER.runSpeed();
      delay(2);
  }
  Z_STEPPER.setCurrentPosition(0); //should set motor position to zero and go back to main routine
}

void R_Stepper_Home(){
  while((digitalRead(R_MIN_PIN) == LOW)){
      R_STEPPER.setSpeed(-rMaxSpeed);
      R_STEPPER.runSpeed();
  }
  R_STEPPER.setCurrentPosition(0); //should set motor position to zero and go back to main routine
}

void R_Stepper_End(){
  while((digitalRead(R_MAX_PIN) == LOW)){
      R_STEPPER.setSpeed(rMaxSpeed);
      R_STEPPER.runSpeed();
  }
  R_End = R_STEPPER.currentPosition();
}
