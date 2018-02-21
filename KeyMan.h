#pragma once

const int keyNull = 0;
const int keySelect = 1;
const int keyLeft = 2;
const int keyDown = 3;
const int keyUp = 4;
const int keyRight = 5;

class KeyMan
{
	int prevkeyana = 1023;
	unsigned int currkey = 0;
	

public:
	KeyMan();
	~KeyMan();
	void updatekey();
};

