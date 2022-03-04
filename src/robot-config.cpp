#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor LeftDriveSmart = motor(PORT20, ratio18_1, false);
motor RightDriveSmart = motor(PORT11, ratio18_1, true);
inertial TurnGyroSmart = inertial(PORT9);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, TurnGyroSmart, 398.98, 320, 40, mm, 1);
bumper rearBumper = bumper(Brain.ThreeWirePort.B);
led powerLED = led(Brain.ThreeWirePort.A);
/*vex-vision-config:begin*/
signature Vision__COLORRED = signature (1, 10239, 10969, 10604, -1349, -637, -993, 7.3, 1);
signature Vision__COLORYELLOW = signature (2, 1719, 1985, 1852, -4537, -4165, -4351, 4.8, 0);
signature Vision__COLORGREEN = signature (3, 0, 0, 0, 0, 0, 0, 3, 1);
signature Vision__COLORORANGE = signature (4, 0, 0, 0, 0, 0, 0, 3, 1);
signature Vision__COLORBLUE = signature (5, 0, 0, 0, 0, 0, 0, 3, 1);
signature Vision__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
code Vision__CCNPIC = code (Vision__COLORRED, Vision__COLORGREEN );
code Vision__ORANGE_AND_BLUE = code (Vision__COLORBLUE, Vision__COLORORANGE );
vision Vision = vision (PORT19, 50, Vision__COLORRED, Vision__COLORYELLOW, Vision__COLORGREEN, Vision__COLORORANGE, Vision__COLORBLUE, Vision__SIG_6);
/*vex-vision-config:end*/
motor ClawMotor = motor(PORT18, ratio18_1, false);
sonar rightRangeFinder = sonar(Brain.ThreeWirePort.C);
sonar leftRangeFinder = sonar(Brain.ThreeWirePort.E);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor next time the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain gyro
  wait(200, msec);
  TurnGyroSmart.calibrate();
  Brain.Screen.print("Calibrating Gyro for Drivetrain");
  // wait for the gyro calibration process to finish
  while (TurnGyroSmart.isCalibrating()) {
    wait(25, msec);
  }
  // reset the screen now that the calibration is complete
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
  wait(50, msec);
  Brain.Screen.clearScreen();
}