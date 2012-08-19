/* 
 * This source use DFRduino
 * 
 */
 
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

//const define
int PITCH_CURRENT_LIMIT = 10;
int YAW_CURRENT_LIMIT = 10;


//pinout define
int GRIPPER_PWM_PIN = 9;
int GRIPPER_DIR = 8;
int PITCH_PWM_PIN = 11;
int PITCH_DIR = 13;
int YAW_PWM_PIN = 10;
int YAW_DIR = 12;
int HEAD_LIGHT = 3;

int MOTOR_DRIVER_POWER = 2;
int GRIPPER_CURRENT = A0;
int PITCH_CURRENT = A1;
int YAW_CURRENT = A2;

//variable
int pitchCurrent=0;
int yawCurrent=0;

ros::NodeHandle  nh;

std_msgs::Int32 pitchCurrentMsg,yawCurrentMsg;

ros::Publisher pitchCurrent_pub("pitchCurrent", &pitchCurrentMsg);
ros::Publisher yawCurrent_pub("yawCurrent", &yawCurrentMsg);

void motorControlKeyboardCb( const std_msgs::String& motorInstruction){
  switch(motorInstruction.data[0])//read instruction from keyboard
  {
    case'q': 
      analogWrite(PITCH_PWM_PIN, 125);
      break;
    case'a':
      analogWrite(PITCH_PWM_PIN, 0);
      break;
    case'w':
      analogWrite(YAW_PWM_PIN, 125);      
      break;
    case's':
      analogWrite(YAW_PWM_PIN, 0);
      break;
    case'z':
      digitalWrite(PITCH_DIR, HIGH-digitalRead(PITCH_DIR));
      break;
    case'x':
      digitalWrite(YAW_DIR, HIGH-digitalRead(YAW_DIR));
      break;
    case'e':
      analogWrite(HEAD_LIGHT,120);
      break;
                                                                                                                                                                                                                    break;
    default:
      analogWrite(PITCH_PWM_PIN, 0);
      analogWrite(YAW_PWM_PIN, 0);
      break;
  }
}


void motorSpeedKeyboardCb( const std_msgs::UInt16& motorSpeed){
  analogWrite(HEAD_LIGHT,motorSpeed.data);
}
  
ros::Subscriber<std_msgs::String> motorControlKeyboard("motorControl", &motorControlKeyboardCb );
ros::Subscriber<std_msgs::UInt16> motorSpeedKeyboard("motorSpeed", &motorSpeedKeyboardCb );

void current_sensing()
{
  pitchCurrentMsg.data=analogRead(PITCH_CURRENT);
  yawCurrentMsg.data=analogRead(YAW_CURRENT);
  if (pitchCurrentMsg.data>PITCH_CURRENT_LIMIT)
    analogWrite(PITCH_PWM_PIN,0);
  if (yawCurrentMsg.data>YAW_CURRENT_LIMIT)
    analogWrite(YAW_PWM_PIN,0);
  pitchCurrent_pub.publish( &pitchCurrentMsg );//need custom message
  yawCurrent_pub.publish( &yawCurrentMsg );//need custom message
 
}

void setup()
{ 
  pinMode(HEAD_LIGHT, OUTPUT);
  pinMode(PITCH_PWM_PIN, OUTPUT);
  pinMode(PITCH_DIR, OUTPUT);
  pinMode(YAW_PWM_PIN, OUTPUT);
  pinMode(YAW_DIR, OUTPUT); 
  pinMode(MOTOR_DRIVER_POWER,OUTPUT);
  digitalWrite(MOTOR_DRIVER_POWER,HIGH);
  nh.initNode();
  nh.advertise(pitchCurrent_pub); //publish ros topic
  nh.advertise(yawCurrent_pub);  //publish
  nh.subscribe(motorControlKeyboard); //subscribe
  nh.subscribe(motorSpeedKeyboard); //subscribe
  analogWrite(HEAD_LIGHT,0);
}
 
void loop()
{ 
//  current_sensing();

  pitchCurrentMsg.data=analogRead(PITCH_CURRENT);
  yawCurrentMsg.data=analogRead(YAW_CURRENT);
  //sensing code error, check it later
  if (pitchCurrentMsg.data>PITCH_CURRENT_LIMIT)
    analogWrite(PITCH_PWM_PIN,0);
  if (yawCurrentMsg.data>YAW_CURRENT_LIMIT)
    analogWrite(YAW_PWM_PIN,0);
  pitchCurrent_pub.publish( &pitchCurrentMsg );//need custom message
  yawCurrent_pub.publish( &yawCurrentMsg );//need custom message
 
  nh.spinOnce();
  delay(1);
}

