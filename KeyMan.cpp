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

enum keystate_t
{
	noPress = 0, noRep, rep, specSelect
};

void KeyMan::repeatCheck()
{
	switch (state)
	{
	case noPress:
		if (currkey != keyNull)
		{
			switch (currkey)
			{
			case keyLeft:
			case keyRight:
				keyout = currkey;
				state = noRep;
				break;
			case keyUp:
			case keyDown:
				timeRepeat.runOnce(500u);
				keyout = currkey;
				state = rep;
				break;
			case keySelect:
				timeRepeat.runOnce(3000u);
				state = specSelect;
				break;
			}
		}
		break;
	case noRep:
		if (currkey == keyNull)
		{
			state = noPress;
		}
		break;
	case rep:
		if (currkey == keyNull)
		{
			state = noPress;
		}
		else
		{
			if (timeRepeat.timetorun())
			{
				timeRepeat.runOnce(200);
				keyout = currkey;
			}
		}
		break;
	case specSelect:
		if (currkey == keyNull)
		{
			state = noPress;
			keyout = keySelect;
		}
		else
		{
			if (timeRepeat.timetorun())
			{
				keyout = keyEscape;
				state = noRep;
			}
		}
		break;
	}
}

void KeyMan::analogcheck()
{
	if (timeCheckkey.timetorun())
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
}

KeyMan::KeyMan()
{
	timeCheckkey.runRepeat(20u);
}


KeyMan::~KeyMan()
{
}


void KeyMan::updatekey()
{
	analogcheck();
	repeatCheck();
}
