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

// A statemachine for translating the button input to useful commands. 
void KeyMan::repeatCheck()
{
	switch (state)
	{
	case noPress:
		if (currkey != keyNull)
		{
			switch (currkey)
			{
			// Output the key input with no repetition.
			case keyLeft:
			case keyRight:
				keyout = currkey;
				state = noRep;
				break;
			// Output the key input with repetition enabled.
			case keyUp:
			case keyDown:
				timeRepeat.runOnce(500u);
				keyout = currkey;
				state = rep;
				break;
			// Waits for select to release to later determine if it was an Escape or a Select.
			case keySelect:
				timeRepeat.runOnce(3000u);
				state = specSelect;
				break;
			}
		}
		break;
	// Returns to the default state if the key has been released.
	case noRep:
		if (currkey == keyNull)
		{
			state = noPress;
		}
		break;
	// Starts to repeat the pressed key if its held and returns to default state when its released.
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
	// Does an escape if Select is held longer than 3 seconds, else outputs a normal Select when released. 
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

// Reads the analog signal and translates it into a stable variable. 
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

// Sets the refresh rate for the class.
KeyMan::KeyMan()
{
	timeCheckkey.runRepeat(20u);
}


KeyMan::~KeyMan()
{
}

// Checks the analog signal translates it into usable keypresses.
void KeyMan::updatekey()
{
	analogcheck();
	repeatCheck();
}

// Outputs the current keypress and resets keyout. 
int KeyMan::getkey()
{
	int holder = keyout;
	keyout = keyNull;

	return holder;
}

// Checks if there is any keypress at all.
bool KeyMan::haskey()
{
	return keyout != keyNull;
}
