// ################################### SETUP ######################################

void setup() {
  pinMode(R_STEP_PIN              , OUTPUT);
  pinMode(R_DIR_PIN               , OUTPUT);
  pinMode(R_ENABLE_PIN            , OUTPUT);
  pinMode(R_MIN_PIN               , INPUT_PULLUP);
  pinMode(R_MAX_PIN               , INPUT_PULLUP);

  pinMode(Z_STEP_PIN              , OUTPUT);
  pinMode(Z_DIR_PIN               , OUTPUT);
  pinMode(Z_ENABLE_PIN            , OUTPUT);
  pinMode(Z_MIN_PIN               , INPUT_PULLUP);
  pinMode(Z_MAX_PIN               , INPUT_PULLUP);

  pinMode(THETA_LEFT_PIN          , OUTPUT);
  pinMode(THETA_RIGHT_PIN         , OUTPUT);
pinMode(THETA_MIN_PIN             , INPUT_PULLUP);
  pinMode(THETA_MAX_PIN            , INPUT_PULLUP);

  pinMode(PUMP_PIN                , OUTPUT);
  pinMode(VAC_PIN                 , OUTPUT);

  pinMode(LED_PIN                 , OUTPUT);

  digitalWrite(R_ENABLE_PIN       , LOW);
  digitalWrite(Z_ENABLE_PIN       , LOW);

  R_STEPPER.setMaxSpeed(rMaxSpeed);
  R_STEPPER.setAcceleration(rMaxAcceleration);

  Z_STEPPER.setMaxSpeed(zMaxSpeed);
  Z_STEPPER.setAcceleration(zMaxAcceleration);
  
//  R_Stepper_Home();
//  delay(500);
//  R_Stepper_End();
//     
//  Z_Stepper_Bottom();
//  delay(500);
//  Z_Stepper_Top();
    DC_Motor_Counterclockwise();
}
