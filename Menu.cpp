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
};

class MenuText : public Menu
{
	const char* row0;
	const char* row1;
	virtual void init();
	virtual void update();
public:
	MenuText(const char* r0, const char* r1);
};

class MenuAllUpDown : public MenuText
{
	virtual void doKey(int key);
};

class MenuCalibrateMin : public MenuText
{
	virtual void doKey(int key);
};

class MenuCalibrateMax : public MenuText
{
	virtual void doKey(int key);
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

MenuDateTime dateTimeMenu;

MenuText upDownMenu("Up / Down", "Full Raise/Lower");
MenuText timerMenu("Timer", "Set the timer");

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

Menu::Menu()
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

MenuText::MenuText(const char * r0, const char * r1)
{
	row0 = r0;
	row1 = r1;
}

enum menutimestate_t
{
	msview = 0, msedithour, mseditmin
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

void keyUpdateVal(int key, int & value, int minValue, int maxValue, int & state, int nextState)
{
	switch (key)
	{
	case keyUp:
		value++;
		if (value > maxValue) value = minValue;
		break;
	case keyDown:
		value--;
		if (value < 0) value = maxValue;
		break;
	case keySelect:
		state = nextState;
		break;
	}
}

void MenuTime::doKey(int key)
{
	int min = (editVal / 60) % 60;
	int hour = editVal / 3600;
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
		keyUpdateVal(key, hour, 0, 23, menuState, mseditmin);
		break;
	case mseditmin:
		keyUpdateVal(key, min, 0, 59, menuState, msview);
		if (key == keySelect)
		{
			*pTime = (hour * 60 + min) * 60;
		}
	}
	editVal = (hour * 60 + min) * 60;
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
		stepContr.doStepAbs(-10);
		break;
	case keyDown:
		stepContr.doStepAbs(10);
		break;
	default:
		Menu::doKey(key);
		break;
	}
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
		stepContr.calibranteMin();
		Menu::doKey(key);
		break;
	default:
		Menu::doKey(key);
		break;
	}
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
		stepContr.calibranteMax();
		Menu::doKey(key);
		break;
	default:
		Menu::doKey(key);
		break;
	}
}
