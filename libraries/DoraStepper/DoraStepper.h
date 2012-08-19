/*
 *DoraStepper.h - Library for control Stepper Motor
 with motor driver which need 2 inputs: step(pwm/clock)
 and direction.
 */

#ifndef DoraStepper_h
#define DoraStepper_h

#include "Arduino.h"


class DoraStepper
{
	public:
		int _STEP_PIN, _DIR_PIN;
		int _STEP_NUMBER;
		int dir;
		int position;
		int dest;
		int halfPeriod;
		long lastTime;
		DoraStepper(int STEP_PIN, int DIR_PIN, int step_number);
		void turn();
		void setSpeed(int speed);
		void setDest(int dest);
	private:
		void toggle();
};

#endif
