#pragma once
#include "TimeTo.h"

class Menu
{
	virtual void init();
	virtual void update();
	static int prevMenu;
	static int currMenu;
	
protected: 
	const char * row0;
	static TimeTo timeUpdate;
	static int menuState;
	static long editVal;
	static long editVal2;
	static long editVal3;
	virtual void doKey(int key);

public:
	Menu(const char * r0);
	~Menu();
	static void doMenu();
};

