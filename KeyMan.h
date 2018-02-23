#pragma once
#include "TimeTo.h"



enum keycode_t
{
	keyNull, keySelect, keyLeft, keyDown, keyUp, keyRight, keyEscape
};



class KeyMan
{
	static int prevkeyana;
	static unsigned int currkey;
	static unsigned int prevkey;
	static TimeTo timeCheckkey;
	static TimeTo timeRepeat;
	static int state;
	static void repeatCheck();
	static void analogcheck();
	static unsigned int keyout;

public:
	KeyMan();
	~KeyMan();
	static void updatekey();
	static int getkey();
	static bool haskey();
};

