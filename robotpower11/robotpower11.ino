// version 7 and on modified for inverted power transistor control
// CAUTION:  THE ARDUINO CONTROLS POWER TO THE COMPUTER.  IF YOU REBOOT
// THE ARDUINO, SUCH AS WHEN YOU ARE PROGRAMMING IT, IT COULD CUT POWER TO
// THE COMPUTER UNLESS YOU DISCONNECT THE YELLOW WIRE GOING TO THE SCREW
// TERMINAL ON THE ARDUINO.  YOU HAVE BEEN WARNED.
const int sensorPin = 0;    // select the input pin for the battery voltage
const int powerswitch = 12; // select the pin for the power on/off transistor
// IF powerswitch IS SET TO HIGH, IT WILL TURN OFF THE COMPUTER POWER.
// forward is pins 2,3 on the 25-pin connector
// backward is pins 4,5   right is pins 6,7    left pins 8,9

const int forward = 5;    // forward is inverted (0 = max)
const int backward = 6;  // backward is not inverted (255 = max)
const int right = 10;   // right IS inverted (0 = max)
const int left = 11;  // left is NOT inverted (255 = max)
const int wheelchairpower = 8; // HIGH turns on wheelchair joystick relay
const int driveswitchpin = 2;  // LOW means "drive B" and HIGH means "drive A"

const int lowvolt = 180;  // voltage below which is considered low
const int okayvolt = 190;  // voltage above which is considered okay
const int turnonvolt = 240;  // voltage above which turns power back on
const int lowcount = 25;  //readings of low voltage before turn-off
const int readrate = 1000;  //milliseconds between battery readings
const int commandexpires = 500;  //milliseconds before direction command expires
const int wheelchairpowerexpires = 5000;  //milliseconds before wheelchair power-on command expires

int voltage,count;  // variable to store the value coming from the sensor
boolean power = true;  // stores the state of main power on/off transistor
unsigned long timenow;  // stores the present time for this cycle of loop
unsigned long lastime = 0;  // stores the last time voltage was checked
byte command = 0;  // send F B L or R ordering motion, and then 0-255 value byte
// send one of the four commands followed by value, or SS for stop
byte heading = 0;  //  direction we are heading now
unsigned long lastcommand = 0;  //  last time we received a command
unsigned long lastwcpoweron = 0;  // stores last time power turned on

void setup() {
  digitalWrite(forward,HIGH);  // forward is inverted
  digitalWrite(backward,LOW);  // backward is not inverted
  digitalWrite(right,HIGH);  // right IS inverted
  digitalWrite(left,LOW);  // left is NOT inverted
  digitalWrite(wheelchairpower,LOW);  // power defaults to OFF
  digitalWrite(driveswitchpin,LOW);  // drive defaults to LOW = drive B
  pinMode(driveswitchpin,OUTPUT);
  pinMode(wheelchairpower,OUTPUT);
  pinMode(forward,OUTPUT);
  pinMode(backward,OUTPUT);
  pinMode(right,OUTPUT);
  pinMode(left,OUTPUT);
  pinMode(powerswitch, OUTPUT);
  digitalWrite(powerswitch, !power);  // turn on power switch
  digitalWrite(sensorPin,LOW);  // do not want pull-up resistor
  pinMode(sensorPin, INPUT);
  Serial.begin(57600);
}

void checkvolt()
{
  lastime = timenow;
  voltage = analogRead(sensorPin);
  if (digitalRead(wheelchairpower) == 1) Serial.print("chair ON: decivoltage ");
  else Serial.print("OFF is chair: decivoltage ");
  Serial.println(voltage);             // and tell it the voltage in decivolts
  if (power == true) {
    if (voltage < lowvolt) count -= 1;
    if (voltage > okayvolt) count = lowcount;
    if (count < 1) {
      count = 0;
      power = false;
      digitalWrite(powerswitch, !power);
    }
  }
  if (power == false)
    if (voltage > turnonvolt) {
      power = true;
      digitalWrite(powerswitch, !power);
      count = lowcount;
    }
}

void PowerON(byte value)
{
  if (value == 122) {  // LOWERCASE Z
    digitalWrite(wheelchairpower,HIGH);
    if (timenow - lastwcpoweron >= wheelchairpowerexpires) Serial.println("Wheelchair Power ON");
    lastwcpoweron = timenow;
    command = 0;
  } 
  else if (value == 0) {  // DONT PRINT ANYTHING WHEN TIMEOUT HAPPENS
    digitalWrite(wheelchairpower,LOW);
  } 
  else {
    Serial.println("Wheelchair Power DISABLED");
    digitalWrite(wheelchairpower,LOW);
    command = 0;
  }
}

void Driveset(byte value)
{
  value &= 223;
  if (value == 'A') {
    digitalWrite(driveswitchpin,HIGH);
    Serial.println("Drive A");
  }
  if (value == 'B') {
    digitalWrite(driveswitchpin,LOW);
    Serial.println("B Drive");
  }
  command = 0;
}

void Forward(byte value)
{
  analogWrite(backward,0);  // backward is not inverted (255 = max);
  analogWrite(forward,255-value);  // forward is inverted (0 = max);
  Serial.print("Forward ");
  Serial.println(value);
  lastcommand = timenow;
  heading = command;
  command = 0;
}

void Left(byte value)
{
  analogWrite(right,255);  // right IS inverted (0 = max);
  analogWrite(left,value);  // left is NOT inverted (255 = max);
  Serial.print("Left ");
  Serial.println(value);
  lastcommand = timenow;
  heading = command;
  command = 0;
}

void Backward(byte value)
{
  analogWrite(forward,255);  // forward is inverted (0 = max);
  analogWrite(backward,value);  // backward is not inverted (255 = max);
  Serial.print("Backward ");
  Serial.println(value);
  lastcommand = timenow;
  heading = command;
  command = 0;
}

void Right(byte value)
{
  analogWrite(left,0);  // left is NOT inverted (255 = max);
  analogWrite(right,255-value);  // right IS inverted (0 = max);
  Serial.print("Right ");
  Serial.println(value);
  lastcommand = timenow;
  heading = command;
  command = 0;
}

void Stop()
{
  analogWrite(forward,255);  // forward is inverted (0 = max);
  analogWrite(backward,0);  // backward is not inverted (255 = max);
  analogWrite(right,255);  // right IS inverted (0 = max);
  analogWrite(left,0);  //  left is NOT inverted (255 = max);
  Serial.println("STOP!");
  //  digitalWrite(forward,HIGH);  // forward is inverted
  //  digitalWrite(backward,LOW);  // backward is not inverted
  //  digitalWrite(right,LOW);  // right is not inverted
  //  digitalWrite(left,HIGH);  // left is inverted
  //  delay(1000);
  heading = 'S';
  command = 0;
}

void getabyte(){
  switch(command) {
  case 'F':
    Forward(Serial.read());
    break;
  case 'B':
    Backward(Serial.read());
    break;
  case 'L':
    Left(Serial.read());
    break;
  case 'R':
    Right(Serial.read());
    break;
  case 'D':
    Driveset(Serial.read());
    break;
  case 'P':
    PowerON(Serial.read());  // IF P IS FOLLOWED BY LOWERCASE Z, POWER WILL TURN ON
    break;                   // POWER-ON COMMAND EXPIRES AFTER wheelchairpowerexpires MILLISECONDS
  default:                   // P FOLLOWED BY ANY OTHER CHARACTER TURNS WHEELCHAIR POWER OFF
    command = Serial.read();
    if (command == 'S') Stop();
    break;
  }
}

void loop() {
  timenow = millis();
  if (timenow - lastime >= readrate) checkvolt(); // if x time has passed since lastime, read voltage and control power
  if (timenow - lastcommand >= commandexpires) if (heading != 'S') Stop();  // stop if x time has passed since last command
  if (timenow - lastwcpoweron >= wheelchairpowerexpires) PowerON(0); // turn off wheelchair power
  if (Serial.available() > 0) getabyte();      // if a serial character is read to be read, read and deal with it
}

