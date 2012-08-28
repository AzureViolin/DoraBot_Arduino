/*
  Dorabot Stepper Controller
  To init a stepper, use
    DoraStepper _stepperName_(step_pin,dir_pin,step_number);
  
 */
#include <ros.h>
#include <std_msgs/String.h>
#include <DoraStepper.h>
#include <Servo.h>

int gripperServoPos = 90; // need to use int. if use char, when type "e"twice, it will go to some random position.

//class
DoraStepper armExtend(11,12,200);//step_pin, dir_pin, step_number
DoraStepper wristStepper270(9,8,200);
Servo gripperServo;

ros::NodeHandle  nh;

//pinout define
char GRIPPER_SERVO_PIN = 3;

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
      wristStepper270.setDest(wristStepper270.position+40);      
      break;
    case'm':
      wristStepper270.setDest(wristStepper270.position-40);
      break;
    case'e':
      gripperServoPos=gripperServoPos+29;
      if (gripperServoPos>180)
        gripperServoPos=180;
      gripperServo.write(gripperServoPos);
      delay(25);
      break;
    case'c':
      gripperServoPos-=29;
      if (gripperServoPos<15)
        gripperServoPos=15;
      gripperServo.write(gripperServoPos);
      delay(25);
      break;
    case'p':
    
      wristStepper270.setDest(wristStepper270.position-50);
      
      for(int j=1; j<70; j++)
      {
        wristStepper270.turn();
        delay(20);
      }
      
      
      for(int i=1; i<20; i++)
      {
      
      gripperServo.write(150);
      delay(1000);
      wristStepper270.setDest(wristStepper270.position-50);
      
      for(int j=1; j<70; j++)
      {
        wristStepper270.turn();
        delay(20);
      }
      
      gripperServo.write(50);
      delay(1000);
      wristStepper270.setDest(wristStepper270.position+50);  
      for(int j=1; j<70; j++)
        {
          wristStepper270.turn();
          delay(20);
        }
      }
      break;
      
    default:
      break;
  }
}

ros::Subscriber<std_msgs::String> lowerArmControlKeyboard("motorControl", &lowerArmControlKeyboardCb );


void setup(){

gripperServo.attach(GRIPPER_SERVO_PIN); //attach Servo to a PWM pin
  //gripperServoPosition
  char gripperServoPos = 90;
  gripperServo.write(gripperServoPos);
// init stepper

//set Speed

armExtend.setSpeed(90);
wristStepper270.setSpeed(90);
//ToBeDeleted.setSpeed(360);
//set Destination
//armExtend.setDest(150);
//wristStepper270.setDest(-100);
//ToBeDeleted.setDest(500);


  //Set up ROS node
  nh.initNode();
  nh.subscribe(lowerArmControlKeyboard); //subscribe
};

void loop(){
      
  //timeNow = millis();
  
  armExtend.turn();
  wristStepper270.turn();
  nh.spinOnce();
  delay(1);

}

