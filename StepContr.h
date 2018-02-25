#pragma once
#include <Stepper.h>
#include "TimeTo.h"

class StepContr
{
	int position;
	int maxpos;
	int stepsToTake;
	int partSteps;
	int speedHolder;
	int stepsM;
public:
	StepContr();
	~StepContr();
	void doStep(int steps);
	void setSpeed(int speed);
	void stepperUpdate();
};

