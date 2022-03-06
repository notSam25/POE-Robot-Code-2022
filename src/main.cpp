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

static const std::pair<double, double> drivetrainTurnErrorBounds = { 106, 213 };  // These positions are relative to the camera
bool stopAfterCompletion = false; 

void PeriodicLED()  {

  while(true != false && 1 + 1 != 69420) {  //Redundency Checks, yes needed.
    wait(0.5, seconds);
    powerLED.on();
    wait(0.5, seconds);
    powerLED.off();
  }
}

void autonomous() { // Not a chance im annotating this
  
      double rightDistance = rightRangeFinder.distance(inches);
      double leftDistance  = leftRangeFinder.distance(inches);
      if(leftDistance <= 10 && rightDistance <=10)  {
          Drivetrain.stop();
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

  // Init our Subsystems
  VisionSubsystem camera;
  ClawSubsystem clawSubsystem{ EClassState::closed }; // Start with the claw closed

  while(!stopAfterCompletion) {                   // Search for blocks and puts them at stations while this bool is false

    while(true) {
      
      camera.takeSnapshot(Vision__COLORRED);      // Looks for RED blocks
      if(camera.seesObjects()) {
        auto object = camera.getNearestObject();

        if(clawSubsystem.isClawOpen())  {         // this checks to see if we dont have a block
          if(object.centerX < drivetrainTurnErrorBounds.first) {
            Drivetrain.turn(right);
          } else if(object.centerX > drivetrainTurnErrorBounds.second) {
            Drivetrain.turn(left);
          } else {
            if(Drivetrain.isTurning())            // Once the robot is oriented to the block, it will stop turning
              Drivetrain.stop();

            Drivetrain.drive(forward);            // Go towards the colored block
          } 
        } 
          
        if((object.centerY - (object.height / 2)) <= 2) { // Check the blocks position on the camera
          Drivetrain.stop();
          clawSubsystem.SetPosition(EClassState::closed); // Close the claw, picking up the block
          break;                                          // Break out of the while(true) loop to start looking for the station's
          }
        else
          clawSubsystem.SetPosition(EClassState::open);
      } 
      else {                                      // If no RED blocks are found, wander around to look for them
        autonomous();
      }
    }
    
    Brain.Screen.print("we have a block!");
    Brain.Screen.newLine();
    while(true) {

      camera.takeSnapshot(Vision__COLORYELLOW);
      if(!camera.seesObjects())
        autonomous();                           // If no station's are found, wander around to look for them
      else {
        if(rightRangeFinder.distance(inches) <= 5 || leftRangeFinder.distance(inches) <= 5) { // Once the camera can see the station, it will stop moving and open the claw, releasing the block
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

          Drivetrain.drive(forward);            // Go towards the station
        }
      }
    }
    Drivetrain.driveFor(reverse, 12.0, inches);     // move 1ft backwards and turn around, then it will start the process of looking for RED blocks
    Drivetrain.turnFor(180, degrees);
  }


  exit(-1);
}