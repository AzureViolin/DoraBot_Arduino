/*
  Dorabot Stepper Controller
  To init a stepper, use
    DoraStepper _stepperName_(step_pin,dir_pin,step_number);
  
 */

#include <DoraStepper.h>

DoraStepper wristPitch(13,12,200);
DoraStepper wristRotate(9,8,200);
//DoraStepper armExtend(5,6,200);

//Debug:Button setup starts
char msgs[5][15] = {
  "Up Key OK    ", 
  "Left Key OK  ", 
  "Down Key OK  ", 
  "Right Key OK ", 
  "Select Key OK" };

int  adc_key_val[5] ={
  30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

int get_key(unsigned int input)
{   
  int k;
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {  
      return k;  
    }
  }
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed
  return k;
}
//Debug Button setup ends

void setup(){


  Serial.begin(9600);//debug key

// init stepper

//set Speed

wristPitch.setSpeed(90);
wristRotate.setSpeed(90);
//armExtend.setSpeed(360);
//set Destination
//wristPitch.setDest(150);
//wristRotate.setDest(-100);
//armExtend.setDest(500);
};

void loop(){
/*
Serial.print("wristPitch.lastTime=");
Serial.println(wristPitch.lastTime);
Serial.print("position=");
Serial.println(wristPitch.position);
Serial.print("dest=");
Serial.println(wristPitch.dest);
Serial.print("halfPeriod=");
Serial.println(wristPitch.halfPeriod);
Serial.print("dir=");
Serial.println(wristPitch.dir);
Serial.print("step_num=");
Serial.println(wristPitch._STEP_NUMBER);
Serial.print("step_pin=");
Serial.println(wristPitch._STEP_PIN);
Serial.print("dir_pin=");
Serial.println(wristPitch._DIR_PIN);
*/
  //debug button starts
   adc_key_in = analogRead(7);    // read the value from the sensor  
  /* get the key */
  key = get_key(adc_key_in);    // convert into key press
  if (key != oldkey) 
  {   // if keypress is detected
    delay(50);      // wait for debounce time
    adc_key_in = analogRead(7);    // read the value from the sensor  
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey) 
    {         
      oldkey = key;
      
      Serial.println(msgs[key]);
      
      switch (key)
      {
        case 0: 
          wristPitch.setDest(wristPitch.position+20);
          //Serial.print("PitchDir=");
          //Serial.println(wristPitch.dir);
          break;
        case 2: 
          wristPitch.setDest(wristPitch.position-20);
          //Serial.print("PitchDir=");
          //Serial.println(wristPitch.dir);
          break;
        case 1:
          wristRotate.setDest(wristRotate.position+20);
          //Serial.print("RotateDir=");
          //Serial.println(wristRotate.dir);

          break;
        case 3:
          wristRotate.setDest(wristRotate.position-20);
          //Serial.print("RotateDir=");
           //Serial.println(wristRotate.dir);

          break;
        default:
          break;
      }
      
          
    
    }
  }
  
  
  //debug button ends



  //timeNow = millis();
  wristPitch.turn();
  wristRotate.turn();
  //armExtend.turn();

}

