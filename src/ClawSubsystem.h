#pragma once
#include "vex.h"
enum EClassState {
  none = 0,
  open,
  closed
};

class ClawSubsystem {
  public:
  ClawSubsystem(EClassState state) {
    SetPosition(state);
    

  }

  void SetPosition(EClassState state) {
    ClawState = state;
    if(state == EClassState::open)
    ClawMotor.spin(forward);
    else
    ClawMotor.spin(reverse);
  }


  bool isClawOpen() {
    printf("claw state -> %d\n", (int)ClawState);
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