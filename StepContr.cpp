#include "StepContr.h"


Stepper stepperM(200, 0, 1, 2, 3);
static TimeTo timeUpdate;


bool StepContr::safeQueue(long steps)
{
	return (position + steps >= 0 && position + steps <= maxpos);
}

void StepContr::varUpdate()
{
	if (stepQueue > 0) 
	{ 
		stepQueue -= 1; 
		position += 1;
	}
	else if (stepQueue < 0) 
	{
		stepQueue += 1;
		position -= 1;
	}
}

int stepdir;

void StepContr::direction()
{
	if (stepQueue > 0) 
	{ 
		stepdir = 1;
	}
	else if (stepQueue < 0) 
	{ 
		stepdir = -1;
	}
}

const int speed = 80; // rpm
const unsigned long interval = 60000000 / (200 * speed);

StepContr::StepContr()
{
	timeUpdate.runRepeatMicros(interval);
	stepperM.setSpeed(1000u);
}

StepContr::~StepContr()
{
}

void StepContr::stepperUpdate()
{
	if (timeUpdate.timetorun())
	{
		if (stepQueue != 0)
		{
			direction();
			stepperM.step(stepdir);
			varUpdate();
			
		}
		else 
		{
			release();
		}
			
		
	}
}

void StepContr::doStepSafe(long steps)
{
	if (safeQueue(steps))
	{
		doStep(steps);
	}
}

void StepContr::doStep(long steps)
{
	stepQueue = steps;
}

void StepContr::doStepToMax()
{
	doStep(maxpos - position);
}

void StepContr::doStepToMin()
{
	doStep(-position);
}

void StepContr::calibrateMax()
{
	maxpos = position;
}

void StepContr::calibrateMin()
{
	position = 0;
}

void StepContr::setUpdateInterval(unsigned long interval)
{
	timeUpdate.runRepeatMicros(interval);
}

void StepContr::stop()
{
	stepQueue = 0;
}

void StepContr::release()
{
	digitalWrite(0, LOW);
	digitalWrite(1, LOW);
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
}


