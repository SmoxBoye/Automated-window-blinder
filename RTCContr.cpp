#include "RTCContr.h"
#include "TimeTo.h"
#include "StepContr.h"

RTC_DS3231 rtc;
DateTime now;

static TimeTo timeUpdate;

extern StepContr stepContr;


RTCContr::RTCContr()
{
	rtc.begin();
	timeUpdate.runRepeat(10000u);
}


RTCContr::~RTCContr()
{
}

void RTCContr::start()
{
	now = rtc.now();
}

void RTCContr::doUpdate()
{
	if (timeUpdate.timetorun())
	{
		now = rtc.now();
		if (alarmHourUp == getHour() && alarmMinuteUp == getMinute())
		{
			stepContr.doStepToMin();
		}
		else if (alarmHourDown == getHour() && alarmMinuteDown == getMinute())
		{
			stepContr.doStepToMax();
		}
	}
}

void RTCContr::setTime(int year, int month, int day, int hour, int minute)
{
	rtc.adjust(DateTime(year, month, day, hour, minute, 0));
}

void RTCContr::setAlarmUp(int hour, int minute)
{
	alarmHourUp = hour;
	alarmMinuteUp = minute;
}

void RTCContr::setAlarmDown(int hour, int minute)
{
	alarmHourDown = hour;
	alarmMinuteDown = minute;
}

void RTCContr::getAlarmUp(int & hour, int & minute)
{
	hour = alarmHourUp;
	minute = alarmMinuteUp;
}

void RTCContr::getAlarmDown(int & hour, int & minute)
{
	hour = alarmHourDown;
	minute = alarmMinuteDown;
}

int RTCContr::getYear()
{
	return (int)now.year();
}

int RTCContr::getMonth()
{
	return (int)now.month();
}

int RTCContr::getDay()
{
	return (int)now.day();
}

int RTCContr::getHour()
{
	return (int)now.hour();
}

int RTCContr::getMinute()
{
	return (int)now.minute();
}
