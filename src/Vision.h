#pragma once

#include "vex.h"

class VisionSubsystem {
public:
  VisionSubsystem() {}

  void takeSnapshot(vex::vision::signature& visionSignature) {
    Vision.takeSnapshot(visionSignature);
  }

  void takeSnapshot(vex::vision::code& code)
 {
   Vision.takeSnapshot(code);
 }
  // vex::vision::object getClosestObject()  {
  //   vex::vision::object object;


  //   return object;
  // }
  bool seesObjects() {
    return Vision.objectCount != 0;
  }

  vex::vision::object getNearestObject() {
    auto object = Vision.objects[0];
    for(int i = 0; i < Vision.objectCount; i++){
      if((object.centerY - (object.height / 2)) > (Vision.objects[i].centerY - (Vision.objects[i].height / 2))) {
        object = Vision.objects[i];
      }
    }
    return object;
  }
  
private:

};