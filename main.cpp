// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    20, 11, 9       
// rearBumper           bumper        B               
// powerLED             led           A               
// Vision               vision        19              
// ClawMotor            motor         18              
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "common.h"
#include <iostream>
using namespace vex;

void PeriodicLED()  {

  while(1)  {
    wait(0.5, seconds);
    powerLED.on();
    wait(0.5, seconds);
    powerLED.off();
  }
}

int main() {
  vex::thread(PeriodicLED).detach();
  VisionSubsystem camera;
  vexcodeInit();
  ClawSubsystem clawSubsystem{EClassState::open};//Init our contructor for the claw subsystem
  rearBumper.pressed(Callbacks::EmergencyStopCallback);

  while(true) {
    
    camera.takeSnapshot(Vision__COLORRED);
    if(camera.seesObjects()) {
      auto object = camera.getNearestObject();
      
      printf((object.centerY - (object.height / 2)));
      Brain.Screen.newLine();
      if((object.centerY - (object.height / 2)) <= 2)
        clawSubsystem.SetPosition(EClassState::closed);
      else
        clawSubsystem.SetPosition(EClassState::open);
    }

  }
  exit(-1);
}