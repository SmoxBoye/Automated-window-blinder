#include "StepContr.h"


Stepper stepperM(200, 4, 5, 6, 7);
TimeTo timeUpdate;


bool StepContr::safeQueue(int steps)
{
	return (calcQueue(steps) >= 0 && calcQueue(steps) <= maxpos);
}

int StepContr::calcQueue(int steps)
{
	return position + steps;
}

void StepContr::varUpdate()
{
	if (stepQueue > 0) 
	{ 
		stepQueue -= 1; 
	}
	else if (stepQueue < 0) 
	{
		stepQueue += 1;
	}
}

int speed = 60;

void StepContr::direction()
{
	if (stepQueue > 0) 
	{ 
		stepperM.setSpeed(speed);
	}
	else if (stepQueue < 0) 
	{ 
		stepperM.setSpeed(-speed);
	}
}

StepContr::StepContr()
{
	timeUpdate.runRepeatMicros(5u);
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
			stepperM.step(1);
			varUpdate();
			
		}
		
		
	}
}

void StepContr::doStepSafe(int steps)
{
	if (safeQueue(steps))
	{
		doStep(steps);
	}
}

void StepContr::doStep(int steps)
{
	stepQueue = calcQueue(steps);
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


