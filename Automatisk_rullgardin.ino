#include <Stepper.h> // I have literally no idea why this has to be here but if it isn't StepContr wont load it in either
#include <Wire.h> // Same here but with the RTC class 
#include "KeyMan.h"
#include "StepContr.h"
#include "RTCContr.h"
#include <LiquidCrystal.h>

KeyMan keyMan;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
StepContr stepContr;
RTCContr rtcContr;

void setup()
{
	lcd.begin(16, 2);
	lcd.clear();
  /* add setup code here */

}

void loop()
{
	keyMan.updatekey();
	stepContr.stepperUpdate();
  
	
	
	/* add main program code here */
}
