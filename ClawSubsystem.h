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

      switch(ClawState) {
        case 0:
        return false;
        break;
        case 1:
        return true;
        break;
        case 2:
        return false;
        break;
      }
  }
  private:
  EClassState ClawState = EClassState::none;
};