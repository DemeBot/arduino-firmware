/*! \file constants.h
    \brief Variable definitions and assignments.
*/

AccelStepper R_STEPPER(1, R_STEP_PIN, R_DIR_PIN);
AccelStepper Z_STEPPER(1, Z_STEP_PIN, Z_DIR_PIN);


const long interval = 5000;     // interval in milliseconds
unsigned long previousMillis = 0;
bool flag = true;

int ON = HIGH;
int OFF = LOW;

// AccelStepper Constants
const int homingStepCount   = 10;
int rMaxSpeed               = 1500;
int zMaxSpeed               = 1000;
int r_default_speed         = 1500;
int z_default_speed         = 1000;
const int rMaxAcceleration  = 1000;
const int zMaxAcceleration  = 1000;
bool isHomedR = false;
bool isHomedT = false;
bool isHomedZ = false;

int z_top = 0;
int R_End = 0;
float theta_max = 1;
volatile float theta_current = 0; // This variable will increase or decrease depending on the rotation of encoder
String dc_direction = "STOP";

// { r, theta }
float locations[][2] = {
  {100, 180}, 
  {100, 135},
  {100, 90},
  {100, 45},
  {100, 0},
  {2900, 0},
  {2900, 22.5},
  {2900, 45},
  {2900, 67.5},
  {2900, 90},
  {2900, 112.5},
  {2900, 135},
  {2900, 157.5},
  {2900, 180}
};
