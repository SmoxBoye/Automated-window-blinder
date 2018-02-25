#include "Menu.h"
#include "KeyMan.h"
#include <LiquidCrystal.h>

int Menu::prevMenu = 0;
int Menu::currMenu = 0;
int Menu::menuState = 0;

extern LiquidCrystal lcd;

class MenuDateTime : public Menu
{
	virtual void init();
	virtual void update();
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

void MenuDateTime::update()
{
	lcd.setCursor(0,0);
	lcd.print("TimeDate");
	lcd.setCursor(0, 1);
	lcd.print("2018-02-24 00:12");

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

enum menustate_t
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
	sprintf(row1, "%n:%n", hour, min);
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

void MenuTime::doKey(int key)
{
	int min = (editVal / 60) % 60;
	int hour = editVal / 3600;
	switch (menuState)
	{
	case msview:
		switch (key)
		{
		case keySelect:
			menuState = msedithour;
			break;
		default:
			Menu::doKey(key);
			break;
		}
		break;
	case msedithour:
		switch (key)
		{
		case keyUp:
			hour++;
			if (hour > 23) hour = 0;
			break;
		case keyDown:
			hour--;
			if (hour < 0) hour = 23;
			break;
		case keySelect:
			menuState = mseditmin;
			break;
		}
		break;
	case mseditmin:
		switch (key)
		{
		case keyUp:
			min++;
			if (min > 59) min = 0;
			break;
		case keyDown:
			min--;
			if (min < 0) min = 59;
			break;
		case keySelect:
			*pTime = (hour * 60 + min) * 60;
			menuState = msview;
			break;
		}
	}
	editVal = (hour * 60 + min) * 60;
}

MenuTime::MenuTime(const char * r0, int * pt)
{
	row0 = r0;
	pTime = pt;
}

