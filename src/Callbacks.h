#pragma once
#include "vex.h"

namespace Callbacks {

  void estopFunc()  {
    while(true){
      if(rearBumper.pressing()) {
        Brain.Screen.clearScreen();
        std::cout << "Emergency Stop Triggered" << std::endl;
        vexSystemExitRequest();
      }
    }
  }
}