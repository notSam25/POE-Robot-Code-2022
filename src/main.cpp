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

void Autonomous() {                           // Yeah, im not annotating this.
  
      double rightDistance = rightRangeFinder.distance(inches);
      double leftDistance  = leftRangeFinder.distance(inches);
      if(leftDistance <= 10 && rightDistance <=10)  {
          Drivetrain.stop();                  // Stop the Drivetrain and reverse
          Drivetrain.driveFor(reverse, 15, inches);
          Drivetrain.turnFor(right, 110, deg);
          Drivetrain.stop(); 
      } else if(leftDistance <= 10) {
        Drivetrain.stop();
        Drivetrain.turnFor(right, 45, deg);
      } else if(leftDistance <= 10) {
        Drivetrain.stop();
        Drivetrain.turnFor(left, 45, deg);
      }
      Drivetrain.drive(forward);
}

int main() {
  //Initialize whatever Vex V5 wants.
  vexcodeInit();

  //note to self, maybe use Vex's event system instead of threads.
  vex::thread(PeriodicLED).detach();
  vex::thread(Callbacks::estopFunc).detach();
  VisionSubsystem camera;
  ClawSubsystem clawSubsystem{EClassState::closed};

  while(true) {
    
    camera.takeSnapshot(Vision__COLORRED);
    if(camera.seesObjects()) {                  // If the Camera's object count is more than 0
      auto object = camera.getNearestObject();  // Filter for the closest object relative to the camera
        
      if((object.centerY - (object.height / 2)) <= 3) {
        Drivetrain.stop();
        clawSubsystem.SetPosition(EClassState::closed);
        break;
        }
      else
        clawSubsystem.SetPosition(EClassState::open);

      if(clawSubsystem.isClawOpen())  {         // Begin the search for mrs.Behrs marbles(she lost them)

      if(object.centerX < drivetrainTurnErrorBounds.first) {
        Drivetrain.turn(right);
      } else if(object.centerX > drivetrainTurnErrorBounds.second) {
        Drivetrain.turn(left);
      } else {
        if(Drivetrain.isTurning())            // At this point the robot has locked onto the object
          Drivetrain.stop();

        Drivetrain.drive(forward);            // Go towards the colored cube(or marble)
      }
        
      }
    } 
    else {
      Autonomous();                           // If no objects were found by the camera
    }
  }

  Brain.Screen.print("we have a block!");     // Or a marble ;)
  Brain.Screen.newLine();
  while(true) {                               // while(true) inside a while(true) ;)
    camera.takeSnapshot(Vision__COLORGREEN);
    if(!camera.seesObjects())
      Autonomous();
    else {
      Brain.Screen.print("sees ORANGE_AND_BLUE");
      Brain.Screen.newLine();
      if(rightRangeFinder.distance(inches) <= 8 && leftRangeFinder.distance(inches) <= 8) {
        Drivetrain.stop();
        clawSubsystem.SetPosition(EClassState::open);
        break;
      }
      auto object = camera.getNearestObject();        // We love 'auto'
      if(object.centerX < drivetrainTurnErrorBounds.first) {
        Drivetrain.turn(right);
      } else if(object.centerX > drivetrainTurnErrorBounds.second) {
        Drivetrain.turn(left);
      } else {
        if(Drivetrain.isTurning())
          Drivetrain.stop();

        Drivetrain.drive(forward);                  // Go towards the colored cube
      }
    }
  }

  return -1;                                        // Imagine
}