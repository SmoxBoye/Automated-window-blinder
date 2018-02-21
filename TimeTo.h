#pragma once
/*
Baseclass for a timer function. 



*/


typedef unsigned long milli_t;

class TimeTo
{

	milli_t nextrun = 0u;
	bool waiting = false;
	bool repeat = false;
	int timeSave = 0;


public:
	TimeTo();
	~TimeTo();
	void runOnce(milli_t time);
	void runRepeat(milli_t time);
	bool timetorun();
};

