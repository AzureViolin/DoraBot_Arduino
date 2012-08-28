/* 
 * This source use DFRduino
 * 
 */
 
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/String.h>
#include <Stepper.h>
#include <Servo.h>

//varialble
char STEPPER_POWER = 30;
int gripperServoPos = 90; // need to use int. if use char, when type "e"twice, it will go to some random position.

//const define
const int stepsPerRevolution = 200; 

//class

Stepper wristStepper270(stepsPerRevolution,8,4);    //the stepper on wrist that can rotate 270 degrees        
Servo gripperServo;

//pinout define
char GRIPPER_SERVO_PIN = 9;

//stepper pin
char STEPPER_POWER_PWM1_PIN = 6;
char STEPPER_DIR1_PIN = 7;
char STEPPER_POWER_PWM2_PIN = 5;
char STEPPER_DIR2_PIN = 4;


ros::NodeHandle  nh;


void motorControlKeyboardCb( const std_msgs::String& motorInstruction){
  switch(motorInstruction.data[0])//read instruction from keyboard
  {
    case'r':
    for(int i=0;i<25;i++)
      {
        wristStepper270.step(1);
        delay(20);
      }
      break;
    case'v':
      for(int i=0;i<25;i++)
      {
        wristStepper270.step(-1);
        delay(20);
      }
      break;
    case't':
      analogWrite(STEPPER_POWER_PWM1_PIN,STEPPER_POWER+=10);
      analogWrite(STEPPER_POWER_PWM2_PIN,STEPPER_POWER+=10);
      break;
    case'b':
      analogWrite(STEPPER_POWER_PWM1_PIN,STEPPER_POWER-=10);
      analogWrite(STEPPER_POWER_PWM2_PIN,STEPPER_POWER-=10);
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
    default:
      break;
  }
}


void motorSpeedKeyboardCb( const std_msgs::UInt16& motorSpeed){
  gripperServo.write(motorSpeed.data);
  delay(25);
}
  
ros::Subscriber<std_msgs::String> motorControlKeyboard("motorControl", &motorControlKeyboardCb );
ros::Subscriber<std_msgs::UInt16> motorSpeedKeyboard("motorSpeed", &motorSpeedKeyboardCb );


void setup()
{ 
  //OUTPUT PINS
  gripperServo.attach(GRIPPER_SERVO_PIN); //attach Servo to a PWM pin
  pinMode(STEPPER_POWER_PWM1_PIN, OUTPUT);
  pinMode(STEPPER_POWER_PWM2_PIN, OUTPUT);
  pinMode(STEPPER_DIR1_PIN, OUTPUT);
  pinMode(STEPPER_DIR2_PIN, OUTPUT);

  //gripperServoPosition
  char gripperServoPos = 90;
  gripperServo.write(gripperServoPos);
  
  //Set Stepper Power Level 
  char STEPPER_POWER = 90;
  analogWrite(STEPPER_POWER_PWM1_PIN,STEPPER_POWER);
  analogWrite(STEPPER_POWER_PWM2_PIN,STEPPER_POWER);
    
  //Close Power Relay

  //Set up ROS node
  nh.initNode();
  nh.subscribe(motorControlKeyboard); //subscribe
  nh.subscribe(motorSpeedKeyboard); //subscribe
  
  }
 
void loop()
{ 
 
  nh.spinOnce();
  delay(1);
}

