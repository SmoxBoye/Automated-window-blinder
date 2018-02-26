#include "Menu.h"
#include "KeyMan.h"
#include "StepContr.h"

#include <LiquidCrystal.h>

const int singleStep = 10;

int Menu::prevMenu = 0;
int Menu::currMenu = 0;
int Menu::menuState = 0;

extern LiquidCrystal lcd;

class MenuDateTime : public Menu
{
	virtual void init();
	virtual void update();
	virtual void doKey(int key);
public:
	MenuDateTime(const char * r0);
};

class MenuDTEditT : public Menu
{
	virtual void init();
	virtual void update();
	virtual void doKey(int key);
public:
	MenuDTEditT(const char * r0);
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
	int* pTime;
	virtual void init();
	virtual void update();
	virtual void doKey(int key);
public:
	MenuTime(const char* r0, int* pt);
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

MenuText calibMenu("Calibrate", "curtain");
MenuCalibrateMin calibMinMenu("Move curtain", "all way up");
MenuCalibrateMin calibMinMenu("Move curtain", "all way down");

const menuList_t menuList[] = 
{
/* 0 */	{ &dateTimeMenu, -1, -1, -1, 1, -1},
/* 1 */	{ &upDownMenu, -1, 2, -1, -1, -1},
/* 2 */	{ &timerMenu, 1, -1, -1, -1, -1},
/* 3 */	

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

extern int currYear;
extern int currMonth;
extern int currDay;
extern int currHour;
extern int currMin;

void MenuDateTime::update()
{
	lcd.setCursor(0,0);
	lcd.print("TimeDate");
	lcd.setCursor(0, 1);
	char row1[17];
	sprintf(row1, "%04i-%02i-%02i %02i:%02i", currYear, currMonth, currDay, currHour, currMin);
	lcd.print(row1);
}

extern StepContr stepContr;

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
	editVal = *pTime;
}

void MenuTime::update()
{
	int min = (editVal / 60) % 60;
	int hour = editVal / 3600;
	lcd.setCursor(0, 0);
	lcd.print(row0);
	lcd.setCursor(0, 1);
	char row1[17];
	sprintf(row1, "%02i:%02i", hour, min);
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
			*pTime = editVal;
		}
	}
}

MenuTime::MenuTime(const char * r0, int * pt)
{
	row0 = r0;
	pTime = pt;
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

void MenuDTEditT::init()
{
	editVal = 0;
}

void MenuDTEditT::update()
{
	int sec = editVal % 60;
	int min = (editVal / 60) % 60;
	int hour = editVal / 3600;
	lcd.setCursor(0, 0);
	lcd.print(row0);
	lcd.setCursor(0, 1);
	char row1[17];
	sprintf(row1, "%02i:%02i", hour, min);
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

void MenuDTEditT::doKey(int key)
{
}

MenuDTEditT::MenuDTEditT(const char * r0) :
	Menu(r0)
{
}
