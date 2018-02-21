#pragma once
#include "TimeTo.h"



enum keycode_t
{
	keyNull, keySelect, keyLeft, keyDown, keyUp, keyRight, keyEscape
};



class KeyMan
{
	int prevkeyana = 1023;
	unsigned int currkey = 0;
	unsigned int prevkey = 0;
	TimeTo timeCheckkey;
	TimeTo timeRepeat;
	int state = 0;
	void repeatCheck();
	void analogcheck();
	unsigned int keyout = 0;

public:
	KeyMan();
	~KeyMan();
	void updatekey();
};

