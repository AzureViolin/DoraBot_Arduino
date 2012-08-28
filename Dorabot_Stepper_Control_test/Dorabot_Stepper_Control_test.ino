/*
  Dorabot Stepper Controller
  To init a stepper, use
    DoraStepper _stepperName_(step_pin,dir_pin,step_number);
  
 */

#include <DoraStepper.h>

DoraStepper wristPitch(13,12,200);
DoraStepper wristRotate(9,8,200);
//DoraStepper armExtend(5,6,200);

void setup(){


//  Serial.begin(9600);

// init stepper

//set Speed

wristPitch.setSpeed(90);
wristRotate.setSpeed(90);

wristPitch.setDest(wristPitch.position+200); 
};

void loop(){


  //timeNow = millis();
  wristPitch.turn();
  wristRotate.turn();
  //armExtend.turn();

}

