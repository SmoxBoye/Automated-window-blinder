#pragma once
#include "TimeTo.h"

class Menu
{
	virtual void init();
	virtual void update();
	static int prevMenu;
	static int currMenu;
	
	
protected: 
	static TimeTo timeUpdate;
	static int menuState;
	static int editVal;
	virtual void doKey(int key);

public:
	Menu();
	~Menu();
	static void doMenu();
};

