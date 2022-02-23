#pragma once
#include "vex.h"

namespace Callbacks { 

  void EmergencyStopCallback()  {

    Brain.Screen.clearScreen();
    std::cout << "Emergency Stop Triggered" << std::endl;
    vexSystemExitRequest();
  }
}