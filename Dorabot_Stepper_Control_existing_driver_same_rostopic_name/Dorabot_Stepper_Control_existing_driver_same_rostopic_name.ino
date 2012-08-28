/*
  Dorabot Stepper Controller
  To init a stepper, use
    DoraStepper _stepperName_(step_pin,dir_pin,step_number);
  
 */
#include <ros.h>
#include <std_msgs/String.h>
#include <DoraStepper.h>

//class
DoraStepper armExtend(13,12,200);//step_pin, dir_pin, step_number
DoraStepper wristRotate(9,8,200);
ros::NodeHandle  nh;


void lowerArmControlKeyboardCb( const std_msgs::String& motorInstruction){
  switch(motorInstruction.data[0])//read instruction from keyboard
  {
    case'y': 
      armExtend.setDest(armExtend.position+40);
      break;
    case'n':
      armExtend.setDest(armExtend.position-40);
      break;
    case'u':
      wristRotate.setDest(wristRotate.position+40);      
      break;
    case'm':
      wristRotate.setDest(wristRotate.position-40);
      break;
    default:
      break;
  }
}

ros::Subscriber<std_msgs::String> lowerArmControlKeyboard("motorControl", &lowerArmControlKeyboardCb );


void setup(){


// init stepper

//set Speed

armExtend.setSpeed(90);
wristRotate.setSpeed(90);
//ToBeDeleted.setSpeed(360);
//set Destination
//armExtend.setDest(150);
//wristRotate.setDest(-100);
//ToBeDeleted.setDest(500);


  //Set up ROS node
  nh.initNode();
  nh.subscribe(lowerArmControlKeyboard); //subscribe
};

void loop(){
      
  //timeNow = millis();
  
  armExtend.turn();
  wristRotate.turn();
  nh.spinOnce();
  delay(1);

}

