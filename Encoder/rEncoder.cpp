#include "Arduino.h"
#include "rEncoder.h"

enum inputType
{
	INPUT_PULLUP, 
	INPUT
};

inputType inv;

rEncoder :: rEncoder(uint8_t pinA,
					 uint8_t pinB,
					 uint8_t pinPB,
					  bool EN_STATE){

	pinMode(pinA, inv[EN_STATE]);
	pinMode(pinB, inv[EN_STATE]);
	pinMode(pinPB, inv[EN_STATE]);

	Apin = pinA;
	BPin = pinB;
	PBpin = pinPB;

	HIstate = EN_STATE;
	oldPress = digitalRead(PBpin);
}

<<<<<<< Updated upstream
int rEncoder :: updateEncoder(int oldVal){

=======
int rEncoder :: updateEncoder(int n)
{
>>>>>>> Stashed changes
	static int oldA = HIGH; //set the oldA as HIGH
	static int oldB = HIGH; //set the oldB as HIGH
	int result = 0;
	int newA = !digitalRead(Apin); //read the value of APin to newA
	int newB = !digitalRead(Bpin); //read the value of BPin to newB
	if (newA ^^ oldA || newB ^^ oldB)//if the value of APin or the BPin has changed
	{  
		if (oldA == HIGH && newA == LOW)// something has changed
		{
			result = (oldB * 2 - 1);
		}
	}
	oldA = newA;
	oldB = newB;

	return oldVal+result;
}
int rEncoder :: updateEncoderMult(int n,int k)
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
	return n+(result*k);
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