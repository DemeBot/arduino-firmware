/*! \file arduino-firmware.ino
    \brief Main loop for arduino.
*/

/* # the sensor value description

# 0-250     dry soil

# 300~500     humid soil

# 500~950     in water

*/

#include <stdio.h>
#include <string.h>
#include <Stepper.h>
#include <AccelStepper.h>
#include <math.h>
#include "pins.h"
#include "constants.h"
#include "functions.h"
#include "setup.h"

void loop() {
  // When Serial input received
  if (Serial.available() > 0) {

    // Read and Manipulate incoming data
    String input_string = Serial.readString(); // read the incoming data as string
    for(int i = 0; i < input_string.length(); i++) { input_string[i] = toupper(input_string[i]); } // capitalize input string
    Serial.println("ok " + input_string);

    // Parse out incoming data into a command, parameter 1, parameter 2, and parameter 3
    String cmd = getValue(input_string,' ',0);
    String param1 = getValue(input_string,' ',1);
    String param2 = getValue(input_string,' ',2);
    String param3 = getValue(input_string,' ',3);
    String paramArray[] = {param1, param2, param3};

    // When cmd = gcode command, perform appropriate action.
    if (cmd == "G28"){
      homing();
    }
    else if (cmd == "G29"){   // home just radius
      Home_Radius();
    }
    else if (cmd == "G30"){   // home just theta
      Home_Theta();
    }
    else if (cmd == "G31"){   // home just z
      Home_Z();
    }
    else if (cmd == "G98"){   // return z to top
      Z_Stepper_Top();
    }
    else if (cmd == "G00"){   // move tool to position. Takes in 3 parameters: Radius (R), Theta (T), Z (Z)
      move_motors(paramArray);
    }      
    else if (cmd == "G04"){   // sets delay time in milliseconds. Takes 1 Parameter: Pause (P)
      wait(paramArray);
    }
    else if (cmd == "M126"){  // close relay. Takes 1 Parameter: Relay_Number (T)
      relay_close(paramArray);
    }
    else if (cmd == "M127"){  // open relay. Takes 1 Parameter: Relay_Number (T) 
      relay_open(paramArray);
    }
    else if (cmd == "D00"){   // stop dc motors 
      DC_Motor_Stop();
    }
    else if (cmd == "D01"){   // move dc motors counter clockwise 
      DC_Motor_Counterclockwise();
    }
    else if (cmd == "D02"){   // move dc motors clockwise 
      DC_Motor_Clockwise();
    }
    else if (cmd == "M136"){  // Get current setting values 
      Print_Settings();
    }
    else if (cmd == "M114"){  // print status of robot 
      Print_Status();
    }
    else if (cmd == "W00"){   // water for t[seconds] 
      Run_Water(paramArray);
    }
    else if (cmd == "HELP"){  // print all commands 
      Print_Commands();
    }
    else if (cmd == "DEMOSINGLE"){  // run demo 
      demo_single();
    }
    else if (cmd == "DEMODOUBLE"){  // run demo 
      demo_double();
    }
    else if (cmd == "DEMO"){  // run demo 
      demo_double();
    }
    else if (cmd == "DEMOSEED"){  // run demo 
      demo_seed(paramArray);
    }
    else {                    // if not valid command, print invalid command
      Serial.println("Not a valid command. Enter Help for Commands.");
    }
    Serial.println("done " + input_string);
  }

  if(digitalRead(THETA_MIN_PIN) && dc_direction == "COUNTER CLOCKWISE"){
    DC_Motor_Stop();
  }

  if(digitalRead(THETA_MAX_PIN) && dc_direction == "CLOCKWISE"){
    DC_Motor_Stop();
  }
}
