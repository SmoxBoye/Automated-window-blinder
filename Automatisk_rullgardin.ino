#include <Wire.h>
#include "KeyMan.h"
#include "StepContr.h"
#include <LiquidCrystal.h>

KeyMan keyMan;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
StepContr stepContr;


void setup()
{
	lcd.begin(16, 2);
	lcd.clear();
  /* add setup code here */

}

void loop()
{
	keyMan.updatekey();
  
	
	
	/* add main program code here */
}
