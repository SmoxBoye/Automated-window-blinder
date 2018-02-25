#pragma once
/*
Baseclass for a timer function. 



*/


typedef unsigned long milli_t;
typedef unsigned long micro_t;

class TimeTo
{

	micro_t nextrun = 0u;
	bool waiting = false;
	bool repeat = false;
	int timeSave = 0;


public:
	TimeTo();
	~TimeTo();
	void runOnce(milli_t time);
	void runRepeat(milli_t time);
	void runOnceMicros(micro_t time);
	void runRepeatMicros(micro_t time);
	bool timetorun();
	void runStop();
};

