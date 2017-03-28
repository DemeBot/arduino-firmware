// ################################### FUNCTIONS ######################################
float theta_to_degrees(){
  return floor((theta_current/theta_max)*180);
}

void Pump_On(){
  digitalWrite(PUMP_PIN           , LOW);
}

void Pump_Off(){
  digitalWrite(PUMP_PIN           , HIGH);
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
  dc_direction = "CLOCKWISE";
}

void DC_Motor_Counterclockwise(){
  digitalWrite(THETA_LEFT_PIN     , HIGH);
  digitalWrite(THETA_RIGHT_PIN    , LOW);
  dc_direction = "COUNTER CLOCKWISE";
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
  int radius = -1, destination_theta = -1, z = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'R'){
      radius = getParameterValue(paramArray[i]);
    }
    else if (paramArray[i][0] == 'T'){
      destination_theta = getParameterValue(paramArray[i]);
    }
    else if (paramArray[i][0] == 'Z'){
      z = getParameterValue(paramArray[i]);
    }
  }

  // Movement of Radius
  if (radius != -1){
    if (radius > 3100) {
      Z_Stepper_Top();
    }
    if (radius > R_End){
      R_STEPPER.runToNewPosition(R_End);
    }
    else if (radius < 0){
      R_STEPPER.runToNewPosition(0);
    }
    else {
      R_STEPPER.runToNewPosition(radius);
    }
  }

  // Movement of Theta
  if (destination_theta != -1){
    if (destination_theta > 180) destination_theta = 180;
    if (destination_theta < 0) destination_theta = 0;
    if (destination_theta < theta_current){
      DC_Motor_Counterclockwise();
      while (destination_theta != theta_to_degrees()){}
      DC_Motor_Stop();
    }
    else if (destination_theta > theta_current){
      DC_Motor_Clockwise();
      while (destination_theta != theta_to_degrees()){}
      DC_Motor_Stop();
    }
  }

  // Movement of Z
  if (z != -1){
    if (z > z_top) {
      Z_STEPPER.runToNewPosition(z_top);
    }
    else {
      if (z < 0){
        z = 0;
      }
      if (R_STEPPER.currentPosition() > 3100){
        Z_STEPPER.runToNewPosition(4500);
      }
      else{
        Z_STEPPER.runToNewPosition(z);
      }
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
      Pump_On();
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
      Pump_Off();
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

//void r_min(){
//  R_STEPPER.stop();
//  R_STEPPER.setCurrentPosition(0);
//}
//
//void r_max(){
//  R_STEPPER.stop();
//}
//
//void z_min(){
//  Z_STEPPER.stop();
//}
//
//void z_max(){
//  Z_STEPPER.stop();
//  Z_STEPPER.setCurrentPosition(0);
//}

void set_speed(String paramArray[]){
  int r_speed = -1;
  int z_speed = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'Z'){
      z_speed = getParameterValue(paramArray[i]);
    }
    else if (paramArray[i][0] == 'R'){
      r_speed = getParameterValue(paramArray[i]);
    }
  }
  if (r_speed != -1){
    if (r_speed > 0 and r_speed <= 100){
      rMaxSpeed = r_default_speed * (r_speed/100);
    }
  }
  if (z_speed != -1){
    zMaxSpeed = z_speed;
  }
  Serial.println();
  Serial.print("R Speed = ");
  Serial.println(rMaxSpeed);
  Serial.print("Z Speed = ");
  Serial.println(zMaxSpeed);
  Serial.println();
}

// clockwise rotation
void clockwise() {
  // Check pin 21 to determine the direction
  if (digitalRead(ENCODER_B) == LOW) {
    theta_current++;
  } else {
    if (theta_current > 0) theta_current--;
  }
}

// counter-clockwise rotation
void counter_clockwise() {
  // Check with pin 20 to determine the direction
  if (digitalRead(ENCODER_A) == LOW) {
    if (theta_current > 0) theta_current--;
  } else {
    theta_current++;
  }
}

void Home_Theta(){
  Serial.print("Homing Theta. ");
  while(!digitalRead(THETA_MIN_PIN)){
    DC_Motor_Counterclockwise();
  }

  theta_current = 0;

  while(!digitalRead(THETA_MAX_PIN)){
    DC_Motor_Clockwise();
  }

  theta_max = theta_current;
  Serial.println("[COMPLETE]");
}

void Home_Radius(){
  Serial.print("Homing Radius. ");
  R_Stepper_Home();
  delay(1000);
  R_Stepper_End();
  delay(500);
  R_STEPPER.runToNewPosition(0);
  Serial.println("[COMPLETE]");
}

void Home_Z(){
  Serial.print("Homing Z. ");
  Z_Stepper_Bottom();
  delay(1000);
  Z_Stepper_Top();
  Serial.println("[COMPLETE]");
}

void Run_Water(String paramArray[]){
  int waterTime = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'T'){
      waterTime = getParameterValue(paramArray[i]);
    }
  }
  Serial.print("Water time: ");
  Serial.println(waterTime);
  if (waterTime != -1){
    Serial.println("Pump on");
    Pump_On();
    delay(waterTime);
    Serial.println("Pump off");
    Pump_Off();
  }
  
}

void Print_Settings(){
  Serial.println();
  Serial.print("Radial length: ");
  Serial.println(R_End);
  Serial.print("Theta length: ");
  Serial.println(theta_max);
  Serial.print("Z length: ");
  Serial.println(z_top);
  Serial.print("R Speed: ");
  Serial.println(rMaxSpeed);
  Serial.print("Z Speed: ");
  Serial.println(zMaxSpeed);
  Serial.print("R Acceleration: ");
  Serial.println(rMaxAcceleration);
  Serial.print("Z Acceleration: ");
  Serial.println(zMaxAcceleration);
  Serial.println();
}

void Print_Status(){
  Serial.println();
  Serial.print("Radius Position: ");
  Serial.println(R_STEPPER.currentPosition());
  Serial.print("Theta Position: ");
  Serial.println(theta_to_degrees());
  Serial.print("Z Position: ");
  Serial.println(Z_STEPPER.currentPosition());
  Serial.println();
}

void Print_Commands(){
  Serial.println("G00 [r#] [t#] [z#]: Move Command.");
  Serial.println("G28: Full Homing Sequence");
  Serial.println("G29: Home just radius");
  Serial.println("G30: Home just theta");
  Serial.println("G31: Home just z axis");
  Serial.println("G98: Return Z-Axis to top.");
  Serial.println("G04 P#: Delay command where P# is time in milliseconds");
  Serial.println("M126 T#: Close Relay where T# is relay number");
  Serial.println("M127 T#: Open Relay where T# is a relay number");
  Serial.println("D00: DC motors stop");
  Serial.println("D01: DC motors counter-clockwise");
  Serial.println("D01: DC motors clockwise");
  Serial.println("M136: Print current setting values");
  Serial.println("M114: Print current positions");
  Serial.println("W00 [t#]: Run water pump where t is time in milliseconds.");
  Serial.println("HELP: Print list of commands");
}

void homing(){
  Z_Stepper_Top();// move z to top to ensure it doesn't hit anything while moving radius
  Home_Radius();  // home the radial axis
  Home_Z();       // home the z axis
  Home_Theta();   // home theta
  Print_Settings();
  Print_Status();
}


