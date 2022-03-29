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

//#include <Timer.h>
#define dw digitalWrite
#define digitOn false
short decDigitPin[10]=;
short dpPin=;
short anodePin[4];
short pb1=;
short pb2=;
short outPin= ;

#define refreshTime 1
#define gapTime 1

void diaplay(int AB,int CD){
	//input domain: AB,CD:[ 0,99]
	int A= AB /10;
	int B= AB %10;
	int C= CD /10;
    int D= CD %10;
    for(int i=0; i<4; i++){
    	dw(anodePin[(i+3)%4], aOff);
    	delay(gapTime);
    	dw(anodePin[i], aOn);
    	delay(refreshTime);
    }
   
 }