#include <stdio.h>
#include <string.h>
#include <Stepper.h>
#include <AccelStepper.h>
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
    Serial.println("Input received: " + input_string);

    // Parse out incoming data into a command, parameter 1, parameter 2, and parameter 3
    String cmd = getValue(input_string,' ',0);
    String param1 = getValue(input_string,' ',1);
    String param2 = getValue(input_string,' ',2);
    String param3 = getValue(input_string,' ',3);
    String paramArray[] = {param1, param2, param3};
//    Serial.println("Command: " + cmd);
//    Serial.println("Parameter 1: " + paramArray[0]);
//    Serial.println("Parameter 2: " + paramArray[1]);
//    Serial.println("Parameter 3: " + paramArray[2]);
//    Serial.println();

    // When cmd = gcode command, perform appropriate action.
    if (cmd == "G28"){
      homing();
    }
    else if (cmd == "G161"){  // move to min position
      Serial.println("HOMING MIN");
    }
    else if (cmd == "G162"){  // move to max position
      Serial.println("HOMING MAX");
    }
    else if (cmd == "G00"){   // move tool to position. Takes in 3 parameters: Radius (R), Theta (T), Z (Z)
      move_motors(paramArray);
    }      
    else if (cmd == "G04"){   // sets delay time in milliseconds. Takes 1 Parameter: Pause (P)
      wait(paramArray);
    }
    else if (cmd == "M126"){  // opens relay. Takes 1 Parameter: Relay_Number (T)
      relay_open(paramArray);
    }
    else if (cmd == "M127"){  // closes relay. Takes 1 Parameter: Relay_Number (T) 
      relay_close(paramArray);
    }
    else if (cmd == "D00"){  // stop dc motors 
      DC_Motor_Stop();
    }
    else if (cmd == "D01"){  // move dc motors counter clockwise 
      DC_Motor_Counterclockwise();
    }
    else if (cmd == "D02"){  // move dc motors clockwise 
      DC_Motor_Clockwise();
    }
    else if (cmd == "M0"){    // stops all robot functions.
      Serial.println("STOPPED, enter any string to continue.");
      while(true){
        if (Serial.available() > 0){
          break;
        }
      }
    }
    else {
      Serial.println("Not a valid command");
    }
    Serial.println("\nREADY FOR INPUT!");
  }

  if(digitalRead(THETA_MIN_PIN) && dc_direction == "COUNTER CLOCKWISE"){
    DC_Motor_Stop();
  }

  if(digitalRead(THETA_MAX_PIN) && dc_direction == "CLOCKWISE"){
    DC_Motor_Stop();
  }
}
