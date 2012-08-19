/* DoraStepper.cpp - Library for control Stepper Motor
 * with motor driver which need 2 inputs: step(pwm/clock)
 * and direction.
 */

#include "Arduino.h"
#include "DoraStepper.h"


DoraStepper::DoraStepper(int step_pin, int dir_pin, int step_number)
{	_STEP_NUMBER = step_number;
	_STEP_PIN = step_pin;
	_DIR_PIN = dir_pin;
	pinMode(_STEP_PIN,OUTPUT);
	pinMode(_DIR_PIN,OUTPUT);
	dir = 0;
	position = 0;
	dest = 0;
	halfPeriod = 0; //miliseconds
		lastTime = 0; //miliseconds
}


void DoraStepper::setSpeed(int speed)//in degrees per second
{
//	this->halfPeriod = 360/speed*(1000/this->_STEP_NUMBER)
	this->halfPeriod = 360/this->_STEP_NUMBER*1000/speed;
}

void DoraStepper::setDest(int dest)//in step
{
	this->dest=dest;

	if (this->_STEP_PIN==1) //make sure change direction only when step complete.
		this->turn();

	if ( this->dest > this->position )
		this->dir=1;
	else 	
	{
		if ( this->dest < this->position )
			this->dir=-1;
		else this->dir=0;
	}
}


void DoraStepper::turn()
{
	if (this->dir!=0)
	{
		if((millis()-this->lastTime)>this->halfPeriod)
		{
			if(this->position==this->dest)
			{
				this->dir=0;
			}
			else this->toggle();
		}
	}
	else
		this->lastTime=millis();
}

void DoraStepper::toggle()
{
	int pin_dir;
	
		if(this->dir==-1) pin_dir = 0;//turn this->dir to output value 0 or 1.
		else pin_dir=1;
		
		digitalWrite(this->_DIR_PIN,pin_dir);
		digitalWrite(this->_STEP_PIN, HIGH - digitalRead(this->_STEP_PIN));//toggle output to generate squre wave.
		
		if (digitalRead(this->_STEP_PIN)==0)//call toggle() twice to make one step
			this->position+=this->dir;//keep track of position
	this->lastTime+=this->halfPeriod;
}

