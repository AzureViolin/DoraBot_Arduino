
const int STEP_PIN = 9;
const int DIR_PIN =8;

void setup() {
  pinMode(STEP_PIN,OUTPUT);//PWM
  pinMode(DIR_PIN,OUTPUT);
  digitalWrite(DIR_PIN,HIGH);
  analogWrite(STEP_PIN,128);
  // put your setup code here, to run once:

}

void loop() {
  delay(2000);
  
  digitalWrite(DIR_PIN,LOW);
  delay(2000);
  digitalWrite(DIR_PIN,HIGH);
  
  // put your main code here, to run repeatedly: 
  
}
