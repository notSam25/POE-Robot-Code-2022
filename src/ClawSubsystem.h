#pragma once
#include "vex.h"
enum EClassState {
  none = 0,
  open,
  closed
};

class ClawSubsystem {
public:
  ClawSubsystem(EClassState state) { SetPosition(state); }  // Set the default state of the claw on robot startup

  void SetPosition(EClassState state) { //set position of claw
    ClawState = state;
    if(state == EClassState::open)
      ClawMotor.spinTo(195, degrees, true);
    else
      ClawMotor.spinTo(0.0, degrees, true);
  }

  bool isClawOpen() { // check claw state
      switch(ClawState) {
        case EClassState::none:
        return false;
        break;
        case  EClassState::open:
        return true;
        break;
        case  EClassState::closed:
        return false;
        break;
      }
  }
private:
  EClassState ClawState = EClassState::none;
};