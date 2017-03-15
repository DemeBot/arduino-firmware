// ################################### FUNCTIONS ######################################

volatile unsigned int counter = 0; // This variable will increase or decrease depending on the rotation of encoder

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

void Z_Stepper_Top(){
  while (!digitalRead(Z_MIN_PIN)){
      Z_STEPPER.setSpeed(zMaxSpeed);
      Z_STEPPER.runSpeed();
  }
  z_top = Z_STEPPER.currentPosition();
}

void Z_Stepper_Bottom(){
  while (!digitalRead(Z_MAX_PIN)){
      Z_STEPPER.setSpeed(-zMaxSpeed);
      Z_STEPPER.runSpeed();
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


// clockwise rotation
void clockwise() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if (digitalRead(3) == LOW) {
    counter++;
  } else {
    if (counter > 0) counter--;
  }
}

// counter-clockwise rotation
void counter_clockwise() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if (digitalRead(2) == LOW) {
    if (counter > 0) counter--;
  } else {
    counter++;
  }
}

// get values from serial input string
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// parse parameter from g-code command and return the value
int getParameterValue(String parameter){
  String substr = parameter.substring(1,parameter.length());
  int value = substr.toInt();
  return value;
}


void move_motors(String paramArray[]){
  int radius = -1, theta = -1, z = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'R'){
      radius = getParameterValue(paramArray[i]);
    }
    else if (paramArray[i][0] == 'T'){
      theta = getParameterValue(paramArray[i]);
    }
    else if (paramArray[i][0] == 'Z'){
      z = getParameterValue(paramArray[i]);
    }
  }

  // Movement of Radius
  if (radius != -1){
    Z_STEPPER.runToNewPosition(z_top);
    if (radius >= 0){
      if (radius > R_End){
        R_STEPPER.runToNewPosition(R_End);
      }
      else {
        R_STEPPER.runToNewPosition(radius);
      }
    }
    else if (radius < 0){
      R_STEPPER.runToNewPosition(0);
    }
  }

  // Movement of Theta
  if (theta != -1){
    Serial.println("Moving Theta");
  }

  // Movement of Z
  if (z != -1){
    if (z >= 0){
      if (z > z_top) {
        Z_STEPPER.runToNewPosition(z_top);
      }
      else {
        Z_STEPPER.runToNewPosition(z);
      }
    }
    else if (z < 0){
      Z_STEPPER.runToNewPosition(0);
    }
  }
}

void relay_open(String paramArray[]){
  int relay = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'T'){
      relay = getParameterValue(paramArray[i]);
    }
  }
  if (relay != -1){
    Serial.print("Closing relay: ");
    Serial.println(relay);
    if (relay == 6){
      Pump_Off();
    }
    if (relay == 7){
      Vac_Off();
    }
  }
}

void relay_close(String paramArray[]){
  int relay = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'T'){
      relay = getParameterValue(paramArray[i]);
    }
  }
  if (relay != -1){
    Serial.print("Opening relay: ");
    Serial.println(relay);
    if (relay == 6){
      Pump_On();
    }
    if (relay == 7){
      Vac_On();
    }
  }
}

void wait(String paramArray[]){
  int pause = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'P'){
      pause = getParameterValue(paramArray[i]);
    }
  }
  if (pause != -1){
    Serial.print("Delaying for ");
    Serial.print(pause);
    Serial.println(" milliseconds.");
    delay(pause);
  }
}

void homing(){
  Z_Stepper_Top();
  R_Stepper_Home();
  Serial.println("Getting radial distance.");
  R_Stepper_End();
  Serial.print("Radial length = ");
  Serial.println(R_End);
  R_STEPPER.runToNewPosition(0);
  Serial.println("Calculating radial distance complete.");
  Serial.println("Getting Z distance");
  Z_Stepper_Bottom();
  Z_Stepper_Top();
  Serial.print("Z length = ");
  Serial.println(z_top);
  Z_STEPPER.runToNewPosition(z_top);
  Serial.println("Calculating Z distance complete.");
  Serial.println("Homing Theta");
  Serial.println("Homing Theta Complete.");
}


