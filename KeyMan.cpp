#include "KeyMan.h"
#include "Arduino.h"
#include "math.h"

const int keyNullAna = 1023;
const int keySelectAna = 639;
const int keyLeftAna = 407;
const int keyDownAna = 255;
const int keyUpAna = 98;
const int keyRightAna = 0;

const int keySelectLim = (keyNullAna + keySelectAna) / 2;
const int keyLeftLim = (keySelectAna + keyLeftAna) / 2;
const int keyDownLim = (keyLeftAna + keyDownAna) / 2;
const int keyUpLim = (keyDownAna + keyUpAna) / 2;
const int keyRightLim = (keyUpAna + keyRightAna) / 2;


KeyMan::KeyMan()
{
}


KeyMan::~KeyMan()
{
}


void KeyMan::updatekey()
{
	int currkeyana = analogRead(0);
	int diffkeyana = abs(currkeyana - prevkeyana);
	prevkeyana = currkeyana;
	// Check if the analog signal is stable.
	if (diffkeyana < 5)
	{
		// Calculate which key is pressed
		if (currkeyana < keyRightLim) currkey = keyRight;
		else if (currkeyana < keyUpLim) currkey = keyUp;
		else if (currkeyana < keyDownLim) currkey = keyDown;
		else if (currkeyana < keyLeftLim) currkey = keyLeft;
		else if (currkeyana < keySelectLim) currkey = keySelect;
		else currkey = keyNull;

	}
}
