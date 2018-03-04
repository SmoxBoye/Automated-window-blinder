#include "Menu.h"
#include "KeyMan.h"
#include "StepContr.h"
#include "RTCContr.h"

#include <LiquidCrystal.h>

extern LiquidCrystal lcd;
extern RTCContr rtcContr;
extern StepContr stepContr;

const int singleStep = 10;

int Menu::prevMenu = 0;
int Menu::currMenu = 0;
int Menu::menuState = 0;
long Menu::editVal = 0;
long Menu::editVal2 = 0;
long Menu::editVal3 = 0;



class MenuDateTime : public Menu
{
	virtual void init();
	virtual void update();
	virtual void doKey(int key);
public:
	MenuDateTime(const char * r0);
};

class MenuRTCEditTime : public Menu
{
	virtual void init();
	virtual void update();
	virtual void doKey(int key);
public:
	MenuRTCEditTime(const char * r0);
};

class MenuRTCEditDate : public Menu
{
	virtual void init();
	virtual void update();
	virtual void doKey(int key);
public:
	MenuRTCEditDate(const char * r0);
};

class MenuText : public Menu
{
	const char * row1;
	virtual void init();
	virtual void update();
public:
	MenuText(const char * r0, const char * r1);
};

class MenuAllUpDown : public MenuText
{
	virtual void doKey(int key);
public:
	MenuAllUpDown(const char * r0, const char * r1);
};

class MenuCalibrateMin : public MenuText
{
	virtual void doKey(int key);
public:
	MenuCalibrateMin(const char * r0, const char * r1);
};

class MenuCalibrateMax : public MenuText
{
	virtual void doKey(int key);
public:
	MenuCalibrateMax(const char * r0, const char * r1);
};

class MenuTime : public Menu
{
	const char* row0;
	virtual void init();
	virtual void update();
	virtual void doKey(int key);
	virtual void store();
public:
	MenuTime(const char* r0);
};

class MenuTimeUp : public MenuTime
{
	virtual void init();
	virtual void store();
public:
	MenuTimeUp(const char* r0);
};

class MenuTimeDown : public MenuTime
{
	virtual void init();
	virtual void store();
public:
	MenuTimeDown(const char* r0);
};

TimeTo Menu::timeUpdate;

struct menuList_t
{
	Menu * pMenu;
	int up;
	int down;
	int left;
	int right;
	int select;
};

MenuDateTime dateTimeMenu("Simon curt. ctrl");

MenuText upDownMenu("Up / Down", "Full Raise/Lower");
MenuText timerMenu("Timer", "Set up/down tim.");
MenuText clockMenu("Clock", "Set the timer");

MenuAllUpDown allUppDownMenu("Up / Down", "Select dir");

MenuTimeUp timeUpMenu("Set up time");
MenuTimeUp timeDownMenu("Set up time");

MenuText calibMenu("Calibrate", "curtain");
MenuCalibrateMin calibMinMenu("Move curtain", "all way up");
MenuCalibrateMax calibMaxMenu("Move curtain", "all way down");

MenuRTCEditTime rtcTimeEdit("Set time");
MenuRTCEditDate rtcDateEdit("Set date");


const menuList_t menuList[] = 
{
/*                           U   D   L   R   S */
/*  0 */ { &dateTimeMenu,   -1, -1, -1,  1, -1},
/*  1 */ { &upDownMenu,      3,  2,  0, -1,  4},
/*  2 */ { &timerMenu,       1,  3,  0,  5, -1},
/*  3 */ { &clockMenu,       2,  1,  0, 10, -1},
/*  4 */ { &allUppDownMenu, -1, -1,  1, -1, -1},
/*  5 */ { &timeUpMenu,      7,  6,  2, -1, -1},
/*  6 */ { &timeDownMenu,    5,  7,  2, -1, -1},
/*  7 */ { &calibMenu,       6,  5,  2, -1,  8},
/*  8 */ { &calibMinMenu,   -1, -1,  7, -1,  9},
/*  9 */ { &calibMaxMenu,   -1, -1,  7, -1,  7},
/* 10 */ { &rtcTimeEdit,    11, 11,  3, -1, -1},
/* 11 */ { &rtcDateEdit,    10, 10,  3, -1, -1}
};

void Menu::init()
{

}

void Menu::update()
{

}

void Menu::doKey(int key)
{
	int newMenu;
	switch (key)
	{
	case keyLeft:
		newMenu = menuList[currMenu].left;
		break;
	case keyRight:
		newMenu = menuList[currMenu].right;
		break;
	case keyUp:
		newMenu = menuList[currMenu].up;
		break;
	case keyDown:
		newMenu = menuList[currMenu].down;
		break;
	case keySelect:
		newMenu = menuList[currMenu].select;
		break;
	}
	if (newMenu >= 0)
	{
		currMenu = newMenu;
	}
}

Menu::Menu(const char * r0) :
	row0(r0)
{
}


Menu::~Menu()
{
}

void Menu::doMenu()
{
	if (currMenu != prevMenu)
	{
		timeUpdate.runStop();
		lcd.noCursor();
		lcd.clear();
		menuState = 0;
		menuList[currMenu].pMenu->init();
		menuList[currMenu].pMenu->update();
		prevMenu = currMenu;
	}
	if (timeUpdate.timetorun()) 
	{
		menuList[currMenu].pMenu->update();
	}
	if (KeyMan::haskey())
	{
		int key = KeyMan::getkey();
		if (key == keyEscape)
		{
			currMenu = 0;
		}
		else
		{
			menuList[currMenu].pMenu->doKey(key);
		}
	}
}

void MenuDateTime::init()
{
	timeUpdate.runRepeat(1000u);
}

void MenuDateTime::update()
{
	lcd.setCursor(0,0);
	lcd.print("TimeDate");
	lcd.setCursor(0, 1);
	char row1[17];
	sprintf(row1, "%04i-%02i-%02i %02i:%02i", rtcContr.getYear(), rtcContr.getMonth(), rtcContr.getDay(), rtcContr.getHour(), rtcContr.getMinute());
	lcd.print(row1);
}

void MenuDateTime::doKey(int key)
{
	switch (key)
	{
	case keyUp:
		stepContr.doStepSafe(-singleStep);
		break;
	case keyDown:
		stepContr.doStepSafe(singleStep);
		break;
	default:
		Menu::doKey(key);
		break;
	}
}

MenuDateTime::MenuDateTime(const char * r0) :
	Menu(r0)
{
}

void MenuText::init()
{
}

void MenuText::update()
{
	lcd.setCursor(0, 0);
	lcd.print(row0);
	lcd.setCursor(0, 1);
	lcd.print(row1);
}

MenuText::MenuText(const char * r0, const char * r1) :
	Menu(r0)
{
	row1 = r1;
}

enum menustate_t
{
	msview = 0, msedityear, mseditmonth, mseditday, msedithour, mseditmin, mseditsec
};

void MenuTime::init()
{
}

void MenuTime::update()
{
	lcd.setCursor(0, 0);
	lcd.print(row0);
	lcd.setCursor(0, 1);
	char row1[17];
	sprintf(row1, "%02i:%02i", (int)editVal, (int)editVal2);
	lcd.print(row1);
	// Show cursor
	switch (menuState)
	{
	case msview:
		lcd.noCursor();
		break;
	case msedithour:
		lcd.setCursor(1, 1);
		lcd.cursor();
		break;
	case mseditmin:
		lcd.setCursor(4, 1);
		lcd.cursor();
		break;
	}
}

void keyUpdateVal(int key, long & value, long step, long minValue, long maxValue, int & state, int nextState)
{
	switch (key)
	{
	case keyUp:
		value += step;
		if (value >= maxValue) value -= maxValue - minValue;
		break;
	case keyDown:
		value--;
		if (value < minValue) value += maxValue - minValue;
		break;
	case keySelect:
		state = nextState;
		break;
	}
}

void MenuTime::doKey(int key)
{
	switch (menuState)
	{
	case msview:
		if (key == keySelect)
		{
			menuState = msedithour;
		}
		else
		{
			Menu::doKey(key);
		}
		break;
	case msedithour:
		keyUpdateVal(key, editVal, 3600, 0, 86400, menuState, mseditmin);
		break;
	case mseditmin:
		keyUpdateVal(key, editVal, 60, 0, 86400, menuState, msview);
		if (key == keySelect)
		{
			store();
		}
	}
}

void MenuTime::store()
{
}

MenuTime::MenuTime(const char * r0) :
	Menu(r0)
{
}

void MenuAllUpDown::doKey(int key)
{
	switch (key)
	{
	case keyUp:
		stepContr.doStepToMin();
		break;
	case keyDown:
		stepContr.doStepToMax();
		break;
	default:
		Menu::doKey(key);
		break;
	}
}

MenuAllUpDown::MenuAllUpDown(const char * r0, const char * r1) :
	MenuText(r0, r1)
{
}

void MenuCalibrateMin::doKey(int key)
{
	switch (key)
	{
	case keyUp:
		stepContr.doStep(-singleStep);
		break;
	case keyDown:
		stepContr.doStep(singleStep);
		break;
	case keySelect:
		stepContr.calibrateMin();
		Menu::doKey(key);
		break;
	default:
		Menu::doKey(key);
		break;
	}
}

MenuCalibrateMin::MenuCalibrateMin(const char * r0, const char * r1) :
	MenuText(r0, r1)
{
}

void MenuCalibrateMax::doKey(int key)
{
	switch (key)
	{
	case keyUp:
		stepContr.doStep(-singleStep);
		break;
	case keyDown:
		stepContr.doStep(singleStep);
		break;
	case keySelect:
		stepContr.calibrateMax();
		Menu::doKey(key);
		break;
	default:
		Menu::doKey(key);
		break;
	}
}

MenuCalibrateMax::MenuCalibrateMax(const char * r0, const char * r1) :
	MenuText(r0, r1)
{
}


void MenuRTCEditTime::init()
{
	editVal = rtcContr.getHour();
	editVal2 = rtcContr.getMinute();
}

void MenuRTCEditTime::update()
{
	if (menuState == msview)
	{
		init();
	}
	lcd.print(row0);
	lcd.setCursor(0, 1);
	char row1[17];
	sprintf(row1, "%02i:%02i", (int)editVal, (int)editVal2);
	lcd.print(row1);
	// Show cursor
	switch (menuState)
	{
	case msview:
		lcd.noCursor();
		break;
	case msedithour:
		lcd.setCursor(1, 1);
		lcd.cursor();
		break;
	case mseditmin:
		lcd.setCursor(4, 1);
		lcd.cursor();
		break;
	}
}

void MenuRTCEditTime::doKey(int key)
{
	switch (menuState)
	{
	case msview:
		if (key == keySelect)
		{
			menuState = msedithour;
		}
		else
		{
			Menu::doKey(key);
		}
		break;
	case msedithour:
		keyUpdateVal(key, editVal, 1, 0, 24, menuState, mseditmin);
		break;
	case mseditmin:
		keyUpdateVal(key, editVal2, 1, 0, 60, menuState, msview);
		if (key == keySelect)
		{
			rtcContr.setTime(rtcContr.getYear(), rtcContr.getMonth(), rtcContr.getDay(), (int)editVal, (int)editVal2);
		}
	}
}

MenuRTCEditTime::MenuRTCEditTime(const char * r0) :
	Menu(r0)
{
}

void MenuRTCEditDate::init()
{
	editVal = rtcContr.getYear();
	editVal2 = rtcContr.getMonth();
	editVal3 = rtcContr.getDay();
}

void MenuRTCEditDate::update()
{
	if (menuState == msview)
	{
		init();
	}
	lcd.print(row0);
	lcd.setCursor(0, 1);
	char row1[17];
	sprintf(row1, "%04i-%02i-%02i", (int)editVal, (int)editVal2, (int)editVal3);
	lcd.print(row1);
	// Show cursor
	switch (menuState)
	{
	case msview:
		lcd.noCursor();
		break;
	case msedityear:
		lcd.setCursor(3, 1);
		lcd.cursor();
		break;
	case mseditmonth:
		lcd.setCursor(6, 1);
		lcd.cursor();
		break;
	case mseditday:
		lcd.setCursor(9, 1);
		lcd.cursor();
		break;
	}
}

void MenuRTCEditDate::doKey(int key)
{
	switch (menuState)
	{
	case msview:
		if (key == keySelect)
		{
			menuState = msedityear;
		}
		else
		{
			Menu::doKey(key);
		}
		break;
	case msedityear:
		keyUpdateVal(key, editVal, 1, 2010, 2030, menuState, mseditmonth);
		break;
	case mseditmonth:
		keyUpdateVal(key, editVal2, 1, 1, 13, menuState, mseditday);
		break;
	case mseditday:
		keyUpdateVal(key, editVal3, 1, 1, 32, menuState, mseditday);
		if (key == keySelect)
		{
			rtcContr.setTime((int)editVal, (int)editVal2, (int)editVal3, rtcContr.getHour(), rtcContr.getMinute());
		}
	}
}

MenuRTCEditDate::MenuRTCEditDate(const char * r0) :
	Menu(r0)
{
}

void MenuTimeUp::init()
{
	int h, m;
	rtcContr.getAlarmUp(h, m);
	editVal = h;
	editVal2 = m;
}

void MenuTimeUp::store()
{
	rtcContr.setAlarmUp(editVal, editVal2);
}

MenuTimeUp::MenuTimeUp(const char * r0) :
	MenuTime(r0)
{
}

void MenuTimeDown::init()
{
	int h, m;
	rtcContr.getAlarmDown(h, m);
	editVal = h;
	editVal2 = m;
}

void MenuTimeDown::store()
{
	rtcContr.setAlarmDown(editVal, editVal2);
}

MenuTimeDown::MenuTimeDown(const char * r0) :
	MenuTime(r0)
{
}
