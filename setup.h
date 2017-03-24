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
  pinMode(THETA_MIN_PIN           , INPUT_PULLUP);
  pinMode(THETA_MAX_PIN           , INPUT_PULLUP);

  pinMode(PUMP_PIN                , OUTPUT);
  pinMode(VAC_PIN                 , OUTPUT);

  attachInterrupt(digitalPinToInterrupt(R_MIN_PIN), r_min, RISING);
  attachInterrupt(digitalPinToInterrupt(R_MAX_PIN), r_max, RISING);
  attachInterrupt(digitalPinToInterrupt(Z_MIN_PIN), z_min, RISING);
  attachInterrupt(digitalPinToInterrupt(Z_MAX_PIN), z_max, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), clockwise, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), counter_clockwise, RISING);
  
  pinMode(LED_PIN                 , OUTPUT);

  digitalWrite(R_ENABLE_PIN       , LOW);
  digitalWrite(Z_ENABLE_PIN       , LOW);
  digitalWrite(PUMP_PIN           , HIGH);
  digitalWrite(VAC_PIN            , HIGH);

  R_STEPPER.setMaxSpeed(rMaxSpeed);
  R_STEPPER.setAcceleration(rMaxAcceleration);
  Z_STEPPER.setMaxSpeed(rMaxSpeed);
  Z_STEPPER.setAcceleration(rMaxAcceleration);
  
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  while (!Serial) { ; } // wait for serial port to connect. Needed for native USB
  Serial.println("Arduino is up and running.");
  Serial.println("READY FOR INPUT!");
}
