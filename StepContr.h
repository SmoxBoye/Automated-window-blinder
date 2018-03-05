#pragma once
#include <Stepper.h>
#include "TimeTo.h"

class StepContr
{
	long position;
	long maxpos;
	long stepQueue;
	
	//int stepsM;
	//int speed;

	bool safeQueue(long steps);
	void varUpdate();
	void direction();
	
public:
	StepContr();
	~StepContr();

	void stepperUpdate();

	
	
	void doStepSafe(long steps);
	void doStep(long steps);
	void doStepToMax();
	void doStepToMin();
	void calibrateMax();
	void calibrateMin();
	void setUpdateInterval(unsigned long interval);
	void stop();
	void release();

};

