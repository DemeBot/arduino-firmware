// ################################### CONSTANTS ######################################

AccelStepper R_STEPPER(1, R_STEP_PIN, R_DIR_PIN);
AccelStepper Z_STEPPER(1, Z_STEP_PIN, Z_DIR_PIN);


const long interval = 5000;     // interval in milliseconds
unsigned long previousMillis = 0;
bool flag = true;

// AccelStepper Constants
const int homingStepCount   = 10;
int rMaxSpeed               = 1500;
int zMaxSpeed               = 1000;
int r_default_speed         = 1500;
int z_default_speed         = 1000;
const int rMaxAcceleration  = 1000;
const int zMaxAcceleration  = 1000;

int z_top = 0;
int R_End = 0;
float theta_max = 1;
volatile float theta_current = 0; // This variable will increase or decrease depending on the rotation of encoder
String dc_direction = "STOP";

float locations[][2] = {
  // { r, theta }
  {1000, 0},
  {1000, 45},
  {1000, 90},
  {1000, 135},
  {1000, 180},
  {3000, 180},
  {3000, 157.5},
  {3000, 135},
  {3000, 112.5},
  {3000, 90},
  {3000, 67.5},
  {3000, 45},
  {3000, 22.5},
  {3000, 0}
};
