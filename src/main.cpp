// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    20, 11, 9       
// rearBumper           bumper        B               
// powerLED             led           A               
// Vision               vision        19              
// ClawMotor            motor         18              
// RangeFinder          sonar         C, D            
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "common.h"
#include <iostream>
using namespace vex;

static const std::pair<double, double> drivetrainTurnErrorBounds = { 106, 213 };

void PeriodicLED()  {

  while(true != false && 1 + 1 != 69420) {  //Redundency Checks, yes needed.
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
  ClawSubsystem clawSubsystem{EClassState::open}; // Init our contructor for the claw subsystem
  rearBumper.pressed(Callbacks::EmergencyStopCallback);

  while(true) {
    
    camera.takeSnapshot(Vision__COLORRED);
    if(camera.seesObjects()) {
      auto object = camera.getNearestObject();
        
      if((object.centerY - (object.height / 2)) <= 2) {
        clawSubsystem.SetPosition(EClassState::closed);
        Drivetrain.stop();
        }
      else
        clawSubsystem.SetPosition(EClassState::open);

      if(clawSubsystem.isClawOpen())  {         // Search for mrs.Behrs marbles(she lost them)

      if(object.centerX < drivetrainTurnErrorBounds.first) {
        Drivetrain.turn(right);
      } else if(object.centerX > drivetrainTurnErrorBounds.second) {
        Drivetrain.turn(left);
      } else {
        if(Drivetrain.isTurning())
          Drivetrain.stop();

        Drivetrain.drive(forward);            // Go towards the colored cube
      }
        
      }
    } 
    else {
      Brain.Screen.print(RangeFinder.distance(inches));
    if(RangeFinder.distance(inches) <= 10)  { // Check for 1meter distance
        Drivetrain.stop();                          // Stop the Drivetrain and reverse
        Drivetrain.driveFor(reverse, 15, inches);
        Drivetrain.turnFor(right, 110, deg);
        Drivetrain.stop(); 
      }
      Drivetrain.drive(forward);
      
    }
  }
  exit(-1);
}