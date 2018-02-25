#include "StepContr.h"


Stepper stepperM(200, 4, 5, 6, 7);
TimeTo timeUpdate;


StepContr::StepContr()
{
	timeUpdate.runRepeat(5u);
}


StepContr::~StepContr()
{
}

void StepContr::doStep(int steps)
{
	if(position >= 0 && position <= maxpos)
	{
		stepsToTake += steps;
		if (stepsToTake > 0) { stepsM = 1; }
		else if (stepsToTake < 0) { stepsM = -1; }

		// partSteps = stepsToTake / speedHolder;
	}
}

void StepContr::setSpeed(int speed)
{
	stepperM.setSpeed(speed);
	// speedHolder = speed;
}

void StepContr::stepperUpdate()
{
	if (timeUpdate.timetorun() && stepsToTake != 0) 
	{
		stepperM.step(stepsM);
		if (stepsToTake > 0) { stepsToTake -= 1; }
		else if (stepsToTake < 0) { stepsToTake += 1; }
	}
}
