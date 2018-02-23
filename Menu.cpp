#include "Menu.h"
#include "KeyMan.h"
#include <LiquidCrystal.h>

int Menu::prevMenu = 0;
int Menu::currMenu = 0;

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
		nollställ updatetimern
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
}

MenuText::MenuText(const char * r0, const char * r1)
{
	row0 = r0;
	row1 = r1;

}
