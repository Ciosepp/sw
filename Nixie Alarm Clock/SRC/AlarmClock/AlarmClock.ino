//Arduino Uno board
/*
pin config
number0
number1
number2
number3
number4
number5
number6
number7
number8
number9

digit1
digit2
digit3
digit4

allarm out

I2C SDA
I2C SCL

Analog Multi1
Analog Multi2
*/

#include "Timer.h"
#include "pinConfig.h"
#include "dataConfig.h"


Timer refeshTimer;

int digits[4];
bool busy = false;
int c = 0;
void diaplay(int AB, int CD) {
	//input domain: AB,CD:[ 0,99]
	digits[0] = AB / 10;
	digits[1] = AB % 10;
	digits[2] = CD / 10;
	digits[3] = CD % 10;

	if (refeshTimer.getClock() && !busy) {
		
		digitalWrite(digits[c],digitOn);
		digitalWrite(anodePin[c],digitOn);
		c = (c++)%4;
		busy = true;
	}

	if(!refeshTimer.getClock() && busy){
    	
	    for(int i=0; i<10; i++){			//reset all the kathode ports
    		digitalWrite(decDigitPin[i],0);
	    }
        busy = false;
    }
    

    

}

void setup() {
	refeshTimer.setClock(10,80);
}

void loop() {

}