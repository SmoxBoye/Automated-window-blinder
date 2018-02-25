#include "TimeTo.h"
#include "limits.h"
#include "Arduino.h"

TimeTo::TimeTo()
{
}


TimeTo::~TimeTo()
{
}

// Adds the wait time to nextrun (relative to millis).
void TimeTo::runOnce(milli_t time)
{
	nextrun = micros() + (time * 1000u);
	waiting = true;
	repeat = false;
}

void TimeTo::runRepeat(milli_t time)
{
	runOnce(time);
	repeat = true;
	timeSave = time;
}

void TimeTo::runOnceMicros(micro_t time)
{
	nextrun = micros() + time;
	waiting = true;
	repeat = false;
}

void TimeTo::runRepeatMicros(micro_t time)
{
	runOnceMicros(time);
	repeat = true;
	timeSave = time;
}

// Checks if it's time to run next step by comparing millies and nextrun.
bool TimeTo::timetorun()
{
	
	if (waiting)
	{
		// Checks if millis has passed nextrun, in that case it's time to run.
		if (micros() - nextrun < ULONG_MAX / 2u) 
		{
			// If it's supposed to be repeated it redoes nextrun.
			if (repeat) 
			{
				nextrun = micros() + timeSave;
			}
			else
			{
				waiting = false;
			}
			return true;
		}
	}
	
	return false;
	
	
}

void TimeTo::runStop()
{
	waiting = false;
}
