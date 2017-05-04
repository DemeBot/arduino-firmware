/*! \file functions.h
    \brief This file includes all the functions the program runs.
*/

///
/// Converts global current_theta value from encoder to degrees out of 180.
///
float theta_to_degrees(){
  return round((theta_current/theta_max)*180);
}

///
/// logs clockwise rotation on rotary encoder
///
void rotation_read_clockwise() {
  // Check pin 21 to determine the direction
  if (digitalRead(ENCODER_B) == LOW) {
    theta_current++;
  } else {
    if (theta_current > 0) theta_current--;
  }
}

///
/// logs counter-clockwise rotation on rotary encoder
///
void rotation_read_counter_clockwise() {
  // Check with pin 20 to determine the direction
  if (digitalRead(ENCODER_A) == LOW) {
    if (theta_current > 0) theta_current--;
  } else {
    theta_current++;
  }
}

///
/// Get values from serial input string
///
String getValue(String data, char separator, int index){
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

///
/// Parse parameter from g-code command and return the value
///
int getParameterValue(String parameter){
  String substr = parameter.substring(1,parameter.length());
  int value = substr.toInt();
  return value;
}

///
/// Turn pump on/off based off input
///
void pump(int pumpIn){
  digitalWrite(PUMP_PIN, pumpIn);
}

///
/// Turn vac on/off based off input
///
void vac(int vacIn){
  digitalWrite(VAC_PIN, vacIn);
}

///
/// move dc motors clockwise
///
void DC_Motor_Clockwise(){
  digitalWrite(THETA_LEFT_PIN     , LOW);
  digitalWrite(THETA_RIGHT_PIN    , HIGH);
  dc_direction = "CLOCKWISE";
}

///
/// move dc motors counter clockwise
///
void DC_Motor_Counterclockwise(){
  digitalWrite(THETA_LEFT_PIN     , HIGH);
  digitalWrite(THETA_RIGHT_PIN    , LOW);
  dc_direction = "COUNTER CLOCKWISE";
}

///
/// stop dc motors
///
void DC_Motor_Stop(){
  digitalWrite(THETA_LEFT_PIN     , HIGH);
  digitalWrite(THETA_RIGHT_PIN    , HIGH);
}

///
/// move z axis to top position
///
void Z_Stepper_Top(){
  while (!digitalRead(Z_MIN_PIN)){
      Z_STEPPER.setSpeed(zMaxSpeed);
      Z_STEPPER.runSpeed();
  }
  z_top = Z_STEPPER.currentPosition();
}

///
/// move z axis to bottom position
///
void Z_Stepper_Bottom(){
  while (!digitalRead(Z_MAX_PIN)){
      Z_STEPPER.setSpeed(-zMaxSpeed);
      Z_STEPPER.runSpeed();
  }
  Z_STEPPER.setCurrentPosition(0); //should set motor position to zero and go back to main routine
}

///
/// move r axis to most inward position
///
void R_Stepper_Home(){
  while((digitalRead(R_MIN_PIN) == LOW)){
      R_STEPPER.setSpeed(-rMaxSpeed);
      R_STEPPER.runSpeed();
  }
  R_STEPPER.setCurrentPosition(0); //should set motor position to zero and go back to main routine
}

///
/// move r axis to outward position
///
void R_Stepper_End(){
  while((digitalRead(R_MAX_PIN) == LOW)){
      R_STEPPER.setSpeed(rMaxSpeed);
      R_STEPPER.runSpeed();
  }
  R_End = R_STEPPER.currentPosition();
}

///
/// move theta to destination_theta
///
void move_theta(int destination_theta){
    if (destination_theta > 180) destination_theta = 180;
    if (destination_theta < 0) destination_theta = 0;
    if (destination_theta < theta_to_degrees()){
      DC_Motor_Counterclockwise();
      while (destination_theta < theta_to_degrees()){}
      DC_Motor_Stop();
    }
    else if (destination_theta > theta_to_degrees()){
      DC_Motor_Clockwise();
      while (destination_theta > theta_to_degrees()){}
      DC_Motor_Stop();
    }
}

///
/// move r axis to radius
///
void move_radius(int radius){
  if (radius > 1750) {
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

///
/// move z axis to z
///
void move_z(int z){
  detachInterrupt(digitalPinToInterrupt(ENCODER_A));
  detachInterrupt(digitalPinToInterrupt(ENCODER_B));
  if (z >= z_top) {
    Z_STEPPER.runToNewPosition(z_top);
  }
  else if ( z <= 0 ){
    Z_STEPPER.runToNewPosition(0);
  }
  else {
    if ((R_STEPPER.currentPosition() == 3500) && (z <= 4000)){
      Z_STEPPER.runToNewPosition(4000);
    }
    else if (R_STEPPER.currentPosition() > 1750){
      Z_STEPPER.runToNewPosition(z_top);
    }
    else {
      Z_STEPPER.runToNewPosition(z);
    }
  }
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), rotation_read_clockwise, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), rotation_read_counter_clockwise, RISING);
}

///
/// Homes the theta axis
///
void Home_Theta(){
  
  Z_Stepper_Top();
  Serial.print("Homing Theta. ");
  while(!digitalRead(THETA_MIN_PIN)){
    DC_Motor_Counterclockwise();
  }

  theta_current = 0;

  while(!digitalRead(THETA_MAX_PIN)){
    DC_Motor_Clockwise();
  }

  DC_Motor_Stop();

  delay(2000);

  theta_max = theta_current;
  isHomedT = true;
  Serial.println("[COMPLETE]");
}

///
/// Homes the r axis
///
void Home_Radius(){
  Z_Stepper_Top();
  Serial.print("Homing Radius. ");
  R_Stepper_Home();
  delay(1000);
  R_Stepper_End();
  delay(500);
  R_STEPPER.runToNewPosition(0);
  isHomedR = true;
  Serial.println("[COMPLETE]");
}

///
/// Hoes the z axis
///
void Home_Z(){
  Serial.print("Homing Z. ");
  Z_Stepper_Bottom();
  delay(1000);
  Z_STEPPER.runToNewPosition(z_top);
  isHomedZ = true;
  Serial.println("[COMPLETE]");
}

///
/// move command for g-code 
///
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
    if (!isHomedR){
      Home_Radius();
    }
    move_radius(radius);
  }

  // Movement of Theta
  if (destination_theta != -1){
    if (!isHomedT){
      Home_Theta();
    }
    move_theta(destination_theta);
  }

  // Movement of Z
  if (z != -1){
    if (!isHomedZ){
      Home_Z();
    }
    move_z(z);
  }
}

///
/// opens a relay, turning specified relay device off
///
void relay_open(String paramArray[]){
  int relay = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'T'){
      relay = getParameterValue(paramArray[i]);
    }
    else if (paramArray[i][0] == 'A'){
      pump(OFF);
      vac(OFF);
    }
  }
  if (relay != -1){
    if (relay == 6){
      pump(OFF);
    }
    if (relay == 7){
      vac(OFF);
    }
  }
}

///
/// closes a relay, turning specified relay device on
///
void relay_close(String paramArray[]){
  int relay = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'T'){
      relay = getParameterValue(paramArray[i]);
    }
  }
  if (relay != -1){
    if (relay == 6){
      pump(ON);
    }
    if (relay == 7){
      vac(ON);
    }
  }
}

///
/// puts a delay in for designated time in milliseconds
///
void wait(String paramArray[]){
  int pause = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'P'){
      pause = getParameterValue(paramArray[i]);
    }
  }
  if (pause != -1){
    delay(pause);
  }
}


///
/// Waters for time given in milliseconds
///
void Run_Water(String paramArray[]){
  int waterTime = -1;
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'T'){
      waterTime = getParameterValue(paramArray[i]);
    }
  }
  if (waterTime != -1){
    pump(ON);
    delay(waterTime);
    pump(OFF);
  }
  
}

///
/// Prints the current settings of the robot
///
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

///
/// Prints the current positions of the robot
///
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

///
/// Prints a list of commands accepted by the robot
///
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
  Serial.println("DEMO: run farmbot demo");
  Serial.println("HELP: Print list of commands");
}

///
/// Homes all axies of robot
///
void homing(){
  Z_Stepper_Top();// move z to top to ensure it doesn't hit anything while moving radius
  Home_Radius();  // home the radial axis
  Home_Z();       // home the z axis
  Home_Theta();   // home theta
  Print_Settings();
  Print_Status();
}

///
/// Waters for time given in milliseconds
///
void Run_Water_For_Time(int timer){
  if (timer>0 && timer<=6000){
    pump(ON);
    delay(timer);
    pump(OFF);
  }
}

///
/// Demo code that runs to each plot position
///
void demo(){
  if(!isHomedR) Home_Radius();
  if(!isHomedZ) Home_Z();
  if(!isHomedT) Home_Theta();
  for( unsigned int i = 0; i < sizeof(locations)/sizeof(locations[0]); i+=1 ){
    move_z(z_top);
    Serial.println("ok C: z:" + String(z_top));
    move_theta(locations[i][1]);
    Serial.println("ok C: t:" + String(locations[i][1]));
    move_radius(locations[i][0]);
    Serial.println("ok C: r:" + String(locations[i][0]));
    move_z(0);
    Serial.println("ok C: z:" + String(0));
    int soil_moisture = analogRead(soilSensorPin);  // read from analog pin A3
    Serial.println("SOIL READING: " + String(soil_moisture));
    delay(1000);
    if(soil_moisture <= 250) {
      move_z(4000);
      Serial.println("ok C: z:" + String(4000));
      Run_Water_For_Time(3000);
    }
  }
  move_z(z_top);
  Serial.println("ok C: z:" + String(z_top));
}

///
/// Demo code to show planting
///
void demo_seed(String paramArray[]){
  int theta = -1, radius = -1;
  if(!isHomedR) Home_Radius();
  if(!isHomedZ) Home_Z();
  if(!isHomedT) Home_Theta();
  for (int i = 0; i < 3; i++){
    if (paramArray[i][0] == 'T'){
      theta = getParameterValue(paramArray[i]);
    }
    if (paramArray[i][0] == 'R'){
      radius = getParameterValue(paramArray[i]);
    }
  }
  if(theta != -1 && radius != -1){
    move_z(z_top);
    move_theta(theta);
    move_radius(3500);
    move_z(3700);
    vac(ON);
    delay(1000);
    move_z(z_top/2);
    move_radius(radius);
    move_z(0);
    vac(OFF);
    delay(3000);
    move_z(z_top);
    Serial.println("ok C: r:" + String(radius) + " t:" + String(theta) + " z:" + String(z_top));
  }
}

