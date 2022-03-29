#include "Arduino.h"
#include "rEncoder.h"

rEncoder :: rEncoder(uint8_t pinA,
					 uint8_t pinB,
					 uint8_t pinPB,
					  bool EN_STATE){
	Apin = pinA;
	Bpin = pinB;
	PBpin = pinPB;

	pinMode(pinA, INPUT_PULLUP);
	pinMode(pinB, INPUT_PULLUP);
	pinMode(pinPB, INPUT_PULLUP);

	if(EN_STATE){
		pinMode(pinA, INPUT);
		pinMode(pinB, INPUT);
		pinMode(pinPB, INPUT);
	}

	HIstate = EN_STATE;
	oldPress = digitalRead(PBpin);
}

int rEncoder :: updateEncoder(int oldVal)
{
	static int oldA = HIstate; //set the oldA as HIGH
	static int oldB = HIstate; //set the oldB as HIGH
	int newA, newB;
	int result = 0;
	
	if(HIstate){
		newA = digitalRead(Apin); //read the value of APin to newA
		newB = digitalRead(Bpin); //read the value of BPin to newB
	}
	else{
		newA = !digitalRead(Apin); //read the value of APin to newA
		newB = !digitalRead(Bpin); //read the value of BPin to newB

	}
	if (newA != oldA || newB != oldB)//if the value of APin or the BPin has changed
	{  
		if (oldA == HIstate && newA == !HIstate)// something has changed
		{
			result = (oldB * 2 - 1);
		}
	}
	oldA = newA;
	oldB = newB;
	return oldVal+result;
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