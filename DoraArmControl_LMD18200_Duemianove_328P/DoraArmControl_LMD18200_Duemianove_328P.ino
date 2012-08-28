/* 
 * This source use DFRduino
 * 
 */
 
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/String.h>
#include <Stepper.h>
//#include <Servo.h>

//varialble
char STEPPER_POWER = 30;
//int gripperServoPos = 90; // need to use int. if use char, when type "e"twice, it will go to some random position.

//const define
const int PITCH_CURRENT_LIMIT = 10;
const int YAW_CURRENT_LIMIT = 10;
const int stepsPerRevolution = 200; 

//class

Stepper wristStepper(stepsPerRevolution,8,4);            
//Servo gripperServo;

//pinout define
char PITCH_PWM_PIN = 11;
char PITCH_DIR_PIN = 13;
char YAW_PWM_PIN = 10;
char YAW_DIR_PIN = 12;
char HEAD_LIGHT_PIN = 3;
//char GRIPPER_SERVO_PIN = 9;

//stepper pin
char STEPPER_POWER_PWM1_PIN = 6;
char STEPPER_DIR1_PIN = 8;
char STEPPER_POWER_PWM2_PIN = 5;
char STEPPER_DIR2_PIN = 4;
char STEPPER_CURRENT1_PIN = A3;
char STEPPER_CURRENT2_PIN = A4;

char MOTOR_DRIVER_POWER_PIN = 2;
char PITCH_CURRENT_PIN = A1;
char YAW_CURRENT_PIN = A2;

ros::NodeHandle  nh;

std_msgs::UInt16 pitchCurrentMsg,yawCurrentMsg,stepperCurrent1Msg,stepperCurrent2Msg;

ros::Publisher pitchCurrent_pub("pitchCurrent", &pitchCurrentMsg);
ros::Publisher yawCurrent_pub("yawCurrent", &yawCurrentMsg);
ros::Publisher stepperCurrent1_pub("stepperCurrent1", &stepperCurrent1Msg);
ros::Publisher stepperCurrent2_pub("stepperCurrent2", &stepperCurrent2Msg);


void motorControlKeyboardCb( const std_msgs::String& motorInstruction){
  switch(motorInstruction.data[0])//read instruction from keyboard
  {
    case'q': 
      digitalWrite(PITCH_DIR_PIN, HIGH);
      analogWrite(PITCH_PWM_PIN, 125);
      break;
    case'a':
      analogWrite(PITCH_PWM_PIN, 0);
      break;
    case'w':
      digitalWrite(YAW_DIR_PIN,HIGH);
      analogWrite(YAW_PWM_PIN, 125);      
      break;
    case's':
      analogWrite(YAW_PWM_PIN, 0);
      break;
    case'z':
      digitalWrite(PITCH_DIR_PIN, LOW);
      analogWrite(PITCH_PWM_PIN, 125);
      break;
    case'x':
      digitalWrite(YAW_DIR_PIN, LOW);
      analogWrite(YAW_PWM_PIN,125);
      break;
    case'r':
    for(int i=0;i<25;i++)
      {
        wristStepper.step(1);
        delay(20);
      }
      break;
    case'v':
      for(int i=0;i<25;i++)
      {
        wristStepper.step(-1);
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
    case'p':
      for (int i=1; i<20; i++)
      {
      digitalWrite(PITCH_DIR_PIN, HIGH);
      digitalWrite(YAW_DIR_PIN, HIGH);
      
      analogWrite(PITCH_PWM_PIN, 255);
      analogWrite(YAW_PWM_PIN, 255);
      
       for(int i=0;i<25;i++)
      {
        wristStepper.step(1);
        delay(20);
      }
      
      delay(3500);
      
      digitalWrite(PITCH_DIR_PIN, LOW);
      digitalWrite(YAW_DIR_PIN, LOW);
      
      for(int i=0;i<25;i++)
      {
        wristStepper.step(-1);
        delay(20);
      }
      
      delay(3500);
      
      
      
      analogWrite(PITCH_PWM_PIN, 0);
      
      delay(3000);
      digitalWrite(YAW_DIR_PIN,HIGH);
      delay(3000);
      analogWrite(YAW_PWM_PIN, 0);

      if (i==10)
      {
        digitalWrite(PITCH_DIR_PIN,HIGH);
        analogWrite(PITCH_PWM_PIN,255);
        delay(2000);
        analogWrite(PITCH_PWM_PIN,0);
      }

        
      }
      break;
      
    default:
      analogWrite(PITCH_PWM_PIN, 0);
      analogWrite(YAW_PWM_PIN, 0);
      break;
  }
}


void motorSpeedKeyboardCb( const std_msgs::UInt16& motorSpeed){
  analogWrite(HEAD_LIGHT_PIN,motorSpeed.data);
  //gripperServo.write(motorSpeed.data);
  //delay(25);
}
  
ros::Subscriber<std_msgs::String> motorControlKeyboard("motorControl", &motorControlKeyboardCb );
ros::Subscriber<std_msgs::UInt16> motorSpeedKeyboard("motorSpeed", &motorSpeedKeyboardCb );


void setup()
{ 
  //OUTPUT PINS
  //gripperServo.attach(GRIPPER_SERVO_PIN); //attach Servo to a PWM pin
  pinMode(HEAD_LIGHT_PIN, OUTPUT);
  pinMode(PITCH_PWM_PIN, OUTPUT);
  pinMode(PITCH_DIR_PIN, OUTPUT);
  pinMode(YAW_PWM_PIN, OUTPUT);
  pinMode(YAW_DIR_PIN, OUTPUT); 
  pinMode(STEPPER_POWER_PWM1_PIN, OUTPUT);
  pinMode(STEPPER_POWER_PWM2_PIN, OUTPUT);
  pinMode(STEPPER_DIR1_PIN, OUTPUT);
  pinMode(STEPPER_DIR2_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER_POWER_PIN,OUTPUT);

  //INPUT PINS
  pinMode(PITCH_CURRENT_PIN, INPUT);
  pinMode(YAW_CURRENT_PIN, INPUT);
  pinMode(STEPPER_CURRENT1_PIN, INPUT);
  pinMode(STEPPER_CURRENT2_PIN, INPUT);

  //Set init
  digitalWrite(PITCH_DIR_PIN,HIGH);
  digitalWrite(YAW_DIR_PIN,HIGH);
  //Set headlight brightness
  analogWrite(HEAD_LIGHT_PIN,0);
  
  //gripperServoPosition
  //char gripperServoPos = 90;
  //gripperServo.write(gripperServoPos);
  
  //Set Stepper Power Level 
  char STEPPER_POWER = 30;
  analogWrite(STEPPER_POWER_PWM1_PIN,STEPPER_POWER);
  analogWrite(STEPPER_POWER_PWM2_PIN,STEPPER_POWER);
    
  //Close Power Relay

  digitalWrite(MOTOR_DRIVER_POWER_PIN,HIGH);

  //Set up ROS node
  nh.initNode();
  nh.advertise(pitchCurrent_pub); //publish ros topic
  nh.advertise(yawCurrent_pub);  //publish
  nh.advertise(stepperCurrent1_pub);  //publish
  nh.advertise(stepperCurrent2_pub);  //publish
  nh.subscribe(motorControlKeyboard); //subscribe
  nh.subscribe(motorSpeedKeyboard); //subscribe
  
  }
 
void loop()
{ 
  stepperCurrent1Msg.data=analogRead(STEPPER_CURRENT1_PIN);
  stepperCurrent2Msg.data=analogRead(STEPPER_CURRENT2_PIN);
  pitchCurrentMsg.data=analogRead(PITCH_CURRENT_PIN);
  yawCurrentMsg.data=analogRead(YAW_CURRENT_PIN);
  
  stepperCurrent1_pub.publish( &stepperCurrent1Msg );
  stepperCurrent2_pub.publish( &stepperCurrent2Msg );
  pitchCurrent_pub.publish( &pitchCurrentMsg );//need custom message
  yawCurrent_pub.publish( &yawCurrentMsg );//need custom message
  
  //current_sensing();


  //sensing code error, check it later
  /*
  if (pitchCurrentMsg.data>PITCH_CURRENT_LIMIT)
    analogWrite(PITCH_PWM_PIN,0);
  if (yawCurrentMsg.data>YAW_CURRENT_LIMIT)
    analogWrite(YAW_PWM_PIN,0);
  */

 
  nh.spinOnce();
  delay(1);
}

