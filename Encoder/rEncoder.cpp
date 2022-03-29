#include "Arduino.h"
#include "rEncoder.h"

enum inputType
{
	INPUT_PULLUP, 
	INPUT
};

rEncoder :: rEncoder(uint8_t pinA,
					 uint8_t pinB,
					 uint8_t pinPB,
					  bool EN_STATE){

	pinMode(pinA, inputType[EN_STATE]);
	pinMode(pinB, inputType[EN_STATE]);
	pinMode(pinPB, inputType[EN_STATE]);
	Apin = pinA;
	BPin = pinB;
	PBpin = pinPB;

	HIstate = EN_STATE;
	oldPress = digitalRead(PBpin);
}

int rEncoder :: updateEncoder(int oldVal)
{
	static int oldA = HIGH; //set the oldA as HIGH
	static int oldB = HIGH; //set the oldB as HIGH
	int result = 0;
	int newA = !digitalRead(Apin); //read the value of APin to newA
	int newB = !digitalRead(Bpin); //read the value of BPin to newB
	if (newA != oldA || newB != oldB)//if the value of APin or the BPin has changed
	{  
		if (oldA == HIGH && newA == LOW)// something has changed
		{
			result = (oldB * 2 - 1);
		}
	}
	oldA = newA;
	oldB = newB;
	return n+result;
}
bool rEncoder :: getPBState()
{
	return digitalRead(PBpin);
}

bool rEncoder :: getLongPress(uint8_t lpTime)
{
	if(digitalRead(PBpin) != oldPress && digitalRead(PBpin) ==HIstate){//rising edge detected
		tBuffer = millis();
		oldPress = digitalRead(PBpin) ;
	}

	if(digitalRead(PBpin) ==HIstate && (millis() - tBuffer) > lpTime ) return true;
	return false;
}