// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
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
void setup() 
{ 
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);
} 
 
 
void loop() 
{ 
  
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
          myservo.write(pos+=1);
          Serial.println(pos); 
          //Serial.println(wristPitch.dir);
          break;
        case 2: 
          myservo.write(pos-=1);
          Serial.println(pos); 
          //Serial.println(wristPitch.dir);
          break;
        case 1:
          myservo.write(pos+=10);
          Serial.println(pos); 
          //Serial.println(wristRotate.dir);

          break;
        case 3:
          myservo.write(pos-=1);
          Serial.println(pos); 
           //Serial.println(wristRotate.dir);

          break;
        default:
          break;
      }
      
          
    
    }
  }
  
  } 
