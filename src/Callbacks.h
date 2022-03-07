#pragma once
#include "vex.h"

namespace Callbacks {

  void estopFunc()  { // called when bumper pressed. Stops the robot.
    while(true){
      if(rearBumper.pressing()) {
        Brain.Screen.clearScreen();
        std::cout << "Emergency Stop Triggered" << std::endl;
        vexSystemExitRequest();
      }
    }
  }
}