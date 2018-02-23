#pragma once
#include "TimeTo.h"

class Menu
{
	virtual void init();
	virtual void update();
	virtual void doKey(int key);
	static int prevMenu;
	static int currMenu;
	
protected: 
	static TimeTo timeUpdate;

public:
	Menu();
	~Menu();
	static void doMenu();
};

