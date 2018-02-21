#include "StateMsk.h"
#include "limits.h"
#include "Arduino.h"

StateMsk::StateMsk()
{
}


StateMsk::~StateMsk()
{
}

// Adds the wait time to nextrun (relative to millis).
void StateMsk::wait(milli_t time)
{
	nextrun = millis() + time;
	waiting = true;
}

// Checks if it's time to run next step by comparing millies and nextrun.
bool StateMsk::timetorun()
{
	
	if (waiting)
	{
		// Checks if millis has passed nextrun, in that case it's time to run.
		if (millis() - nextrun < ULONG_MAX / 2u) 
		{
			waiting = false;
			return true;
		}
		return false;
	}
	else 
	{
		// If it's not waiting it's time to run.
		return true;
	}
	
}
