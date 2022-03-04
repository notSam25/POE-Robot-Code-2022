// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    20, 11, 9       
// rearBumper           bumper        B               
// powerLED             led           A               
// Vision               vision        19              
// ClawMotor            motor         18              
// rightRangeFinder     sonar         C, D            
// leftRangeFinder      sonar         E, F            
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

void autonomous() {
  
      double rightDistance = rightRangeFinder.distance(inches);
      double leftDistance  = leftRangeFinder.distance(inches);
      if(leftDistance <= 10 && rightDistance <=10)  { // Check for 1meter distance
          Drivetrain.stop();                          // Stop the Drivetrain and reverse
          Drivetrain.driveFor(reverse, 15, inches);
          Drivetrain.turnFor(right, 130, deg);
          Drivetrain.stop(); 
      } else if(leftDistance <= 10) {
        Drivetrain.stop();
        Drivetrain.driveFor(reverse, 10, inches);
        Drivetrain.turnFor(right, 55, deg);
      } else if(leftDistance <= 10) {
        Drivetrain.stop();
        Drivetrain.driveFor(reverse, 10, inches);
        Drivetrain.turnFor(left, 55, deg);
      }
      Drivetrain.drive(forward);
}

int main() {
  
  vexcodeInit();
  vex::thread(PeriodicLED).detach();
  vex::thread(Callbacks::estopFunc).detach();

  VisionSubsystem camera;
  ClawSubsystem clawSubsystem{ EClassState::open }; // Init our contructor for the claw subsystem

  while(true) {

    while(true) {
      
      camera.takeSnapshot(Vision__COLORRED);
      if(camera.seesObjects()) {
        auto object = camera.getNearestObject();

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
          
        if((object.centerY - (object.height / 2)) <= 2) {
          Drivetrain.stop();
          clawSubsystem.SetPosition(EClassState::closed);
          break;
          }
        else
          clawSubsystem.SetPosition(EClassState::open);
      } 
      else {
        autonomous();
      }
    }
    //we have a block. return to base
    Brain.Screen.print("we have a block!");
    Brain.Screen.newLine();
    while(true) {
      camera.takeSnapshot(Vision__COLORYELLOW);
      if(!camera.seesObjects())
        autonomous();
      else {
        Brain.Screen.print("sees BASE");
        Brain.Screen.newLine(); 
        if(rightRangeFinder.distance(inches) <= 5 || leftRangeFinder.distance(inches) <= 5) {
          Drivetrain.stop();
          clawSubsystem.SetPosition(EClassState::open);
          break;
        }
        auto object = camera.getNearestObject();
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
    Drivetrain.driveFor(reverse, 12.0, inches);
    Drivetrain.turnFor(180, degrees);
  }


  exit(-1);
}