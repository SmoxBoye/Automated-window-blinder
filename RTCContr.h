#pragma once

#include "RTClib.h"

class RTCContr
{
	int alarmHourUp;
	int alarmMinuteUp;
	int alarmHourDown;
	int alarmMinuteDown;

public:
	RTCContr();
	~RTCContr();

	void start();
	void doUpdate();
	void setTime(int year, int month, int day, int hour, int minute);
	void setAlarmUp(int hour, int minute);
	void setAlarmDown(int hour, int minute);
	void getAlarmUp(int & hour, int & minute);
	void getAlarmDown(int & hour, int & minute);
	int getYear();
	int getMonth();
	int getDay();
	int getHour();
	int getMinute();
};
