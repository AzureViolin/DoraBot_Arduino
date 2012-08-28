/* 
 * This source use DFRduino
 * 
 */
 
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>


//debug
//varialble
int STEPPER_POWER = 50;

//const define
int PITCH_CURRENT_LIMIT = 10;
int YAW_CURRENT_LIMIT = 10;


//pinout define
int PITCH_PWM_PIN = 11;
int PITCH_DIR_PIN = 13;
int YAW_PWM_PIN = 10;
int YAW_DIR_PIN = 12;
int HEAD_LIGHT_PIN = 3;

//stepper pin
int STEPPER_POWER_PWM1_PIN = 6;
int STEPPER_DIR1_PIN = 8;
int STEPPER_POWER_PWM2_PIN = 5;
int STEPPER_DIR2_PIN = 4;
int STEPPER_CURRENT1_PIN = A3;
int STEPPER_CURRENT2_PIN = A4;

int MOTOR_DRIVER_POWER_PIN = 2;
int PITCH_CURRENT_PIN = A1;
int YAW_CURRENT_PIN = A2;

//variable
int pitchCurrent=0;
int yawCurrent=0;

ros::NodeHandle  nh;

std_msgs::Int32 pitchCurrentMsg,yawCurrentMsg,stepperCurrent1Msg,stepperCurrent2Msg;

ros::Publisher pitchCurrent_pub("pitchCurrent", &pitchCurrentMsg);
ros::Publisher yawCurrent_pub("yawCurrent", &yawCurrentMsg);
ros::Publisher stepperCurrent1_pub("stepperCurrent1", &stepperCurrent1Msg);
ros::Publisher stepperCurrent2_pub("stepperCurrent2", &stepperCurrent2Msg);

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
      digitalWrite(PITCH_DIR_PIN, HIGH-digitalRead(PITCH_DIR_PIN));
      break;
    case'x':
      digitalWrite(YAW_DIR_PIN, HIGH-digitalRead(YAW_DIR_PIN));
      break;
    case'e':
      analogWrite(HEAD_LIGHT_PIN,120);
      break;
                                                                                                                                                                                                                    break;
    default:
      analogWrite(PITCH_PWM_PIN, 0);
      analogWrite(YAW_PWM_PIN, 0);
      break;
  }
}


void motorSpeedKeyboardCb( const std_msgs::UInt16& motorSpeed){
  analogWrite(HEAD_LIGHT_PIN,motorSpeed.data);
}
  
ros::Subscriber<std_msgs::String> motorControlKeyboard("motorControl", &motorControlKeyboardCb );
ros::Subscriber<std_msgs::UInt16> motorSpeedKeyboard("motorSpeed", &motorSpeedKeyboardCb );

void current_sensing()
{
  pitchCurrentMsg.data=analogRead(PITCH_CURRENT_PIN);
  yawCurrentMsg.data=analogRead(YAW_CURRENT_PIN);
  /*
  if (pitchCurrentMsg.data>PITCH_CURRENT_LIMIT)
    analogWrite(PITCH_PWM_PIN,0);
  if (yawCurrentMsg.data>YAW_CURRENT_LIMIT)
    analogWrite(YAW_PWM_PIN,0);
  */
  
  pitchCurrent_pub.publish( &pitchCurrentMsg );//need custom message
  yawCurrent_pub.publish( &yawCurrentMsg );//need custom message
 
}

void setup()
{ 
  pinMode(HEAD_LIGHT_PIN, OUTPUT);
  pinMode(PITCH_PWM_PIN, OUTPUT);
  pinMode(PITCH_DIR_PIN, OUTPUT);
  pinMode(YAW_PWM_PIN, OUTPUT);
  pinMode(YAW_DIR_PIN, OUTPUT); 
  pinMode(MOTOR_DRIVER_POWER_PIN,OUTPUT);
  digitalWrite(MOTOR_DRIVER_POWER_PIN,HIGH);
  nh.initNode();
  nh.advertise(pitchCurrent_pub); //publish ros topic
  nh.advertise(yawCurrent_pub);  //publish
  nh.subscribe(motorControlKeyboard); //subscribe
  nh.subscribe(motorSpeedKeyboard); //subscribe
  analogWrite(HEAD_LIGHT_PIN,0);
  analogWrite(STEPPER_POWER_PWM1_PIN,STEPPER_POWER);
  analogWrite(STEPPER_POWER_PWM2_PIN,STEPPER_POWER);
}
 
void loop()
{ 
  //current_sensing();

  pitchCurrentMsg.data=analogRead(PITCH_CURRENT_PIN);
  yawCurrentMsg.data=analogRead(YAW_CURRENT_PIN);
  stepperCurrent1Msg.data=analogRead(STEPPER_CURRENT1_PIN);
  stepperCurrent2Msg.data=analogRead(STEPPER_CURRENT2_PIN);
  
  //sensing code error, check it later
  /*
  if (pitchCurrentMsg.data>PITCH_CURRENT_LIMIT)
    analogWrite(PITCH_PWM_PIN,0);
  if (yawCurrentMsg.data>YAW_CURRENT_LIMIT)
    analogWrite(YAW_PWM_PIN,0);
  */
  stepperCurrent1_pub.publish( &stepperCurrent1Msg );
  stepperCurrent2_pub.publish( &stepperCurrent2Msg );
  pitchCurrent_pub.publish( &pitchCurrentMsg );//need custom message
  yawCurrent_pub.publish( &yawCurrentMsg );//need custom message
 
 
  nh.spinOnce();
  delay(1);
}

