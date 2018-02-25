#pragma once
#include <Stepper.h>
#include "TimeTo.h"

class StepContr
{
	int position;
	int maxpos;
	int stepQueue;
	
	int stepsM;
	int speed;

	bool safeQueue(int steps);
	int calcQueue(int steps);
	void varUpdate();
	void direction();
	
public:
	StepContr();
	~StepContr();

	void stepperUpdate();

	
	
	void doStepSafe(int steps);
	void doStep(int steps);
	void doStepToMax();
	void doStepToMin();
	void calibrateMax();
	void calibrateMin();
	void setUpdateInterval(unsigned long interval);
	void stop();

};

