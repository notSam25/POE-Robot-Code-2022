using namespace vex;

extern brain Brain;

using signature = vision::signature;
using code = vision::code;

// VEXcode devices
extern controller Controller1;
extern smartdrive Drivetrain;
extern bumper rearBumper;
extern led powerLED;
extern signature Vision__COLORRED;
extern signature Vision__COLORYELLOW;
extern signature Vision__COLORGREEN;
extern signature Vision__COLORORANGE;
extern signature Vision__COLORBLUE;
extern signature Vision__SIG_6;
extern signature Vision__SIG_7;
extern code Vision__CCNPIC;
extern code Vision__ORANGE_AND_BLUE;
extern vision Vision;
extern motor ClawMotor;
extern sonar rightRangeFinder;
extern sonar leftRangeFinder;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );