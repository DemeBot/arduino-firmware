// ################################### PINS ######################################

#define R_STEP_PIN         54   // X STEPPER PORT
#define R_DIR_PIN          55   // X STEPPER PORT
#define R_ENABLE_PIN       38   // X STEPPER PORT
#define R_MAX_PIN           3   // X STEPPER PORT
#define R_MIN_PIN           2   // X STEPPER PORT

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define THETA_MIN_PIN      14   // Y Min Pin
#define THETA_MAX_PIN      15   // Y Max Pin

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define ENCODER_A             20
#define ENCODER_B     21

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define Q_STEP_PIN         36
#define Q_DIR_PIN          34
#define Q_ENABLE_PIN       30

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN         13   // ANALOG NUMBERING
#define TEMP_1_PIN         14   // ANALOG NUMBERING

#define AUX18              16
#define AUX17              17
#define AUX16              23
#define AUX15              25
#define AUX14              27
#define AUX13              29
#define RELAY_8_PIN        31   // aux 12
#define VAC_PIN            33   // aux 11; relayPin7
#define PUMP_PIN           35   // aux 10; relayPin6
#define RELAY_5_PIN        37   // aux 9
#define RELAY_4_PIN        39   // aux 8
#define RELAY_3_PIN        41   // aux 7
#define RELAY_2_PIN        43   // aux 6
#define RELAY_1_PIN        45   // aux 5
#define THETA_RIGHT_PIN    47   // aux 4  
#define THETA_LEFT_PIN     32   // aux 3
