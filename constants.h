// ################################### CONSTANTS ######################################

AccelStepper R_STEPPER(1, R_STEP_PIN, R_DIR_PIN);
AccelStepper Z_STEPPER(1, Z_STEP_PIN, Z_DIR_PIN);


const long interval = 5000;     // interval in milliseconds
unsigned long previousMillis = 0;
bool flag = true;

// AccelStepper Constants
const int homingStepCount   = 10;
const int rMaxSpeed         = 1500;
const int zMaxSpeed         = 1000;
const int rMaxAcceleration  = 500;
const int zMaxAcceleration  = 500;

int z_top = 0;
int R_End = 0;
String dc_direction = "STOP";
