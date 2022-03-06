#pragma once

#include "vex.h"

class VisionSubsystem {
public:
  VisionSubsystem() {}

  void takeSnapshot(vex::vision::signature& visionSignature) {
    Vision.takeSnapshot(visionSignature);
  }

  void takeSnapshot(vex::vision::code& code)  {
   Vision.takeSnapshot(code);
  }
 
  bool seesObjects() {
    return Vision.objectCount != 0;
  }

  vex::vision::object getNearestObject() {    // This function will get the closest block to the center of the camera, and return it as a vex::vision::object 
    auto object = Vision.objects[0];
    for(int i = 0; i < Vision.objectCount; i++){
      if((object.centerY - (object.height / 2)) > (Vision.objects[i].centerY - (Vision.objects[i].height / 2))) {
        object = Vision.objects[i];
      }
    }
    return object;
  }
  
};