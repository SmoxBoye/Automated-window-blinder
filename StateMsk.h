#pragma once
/*
Baseclass for a statemachine. 



*/


typedef unsigned long milli_t;

class StateMsk
{

	unsigned int state = 0u;
	milli_t nextrun = 0u;
	bool waiting = false;


public:
	StateMsk();
	~StateMsk();
	void wait(milli_t time);
	bool timetorun();
};

