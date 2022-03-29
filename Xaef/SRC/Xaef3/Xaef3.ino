#include <SoftSerial.h>     
#define rxP 2
#define txP 3
#define maxEx 120
#define minEx 80
#include <SoftRcPulseOut.h> 

SoftSerial BT(rxP, txP);
int baudrate=9600;
long delayTime=1000;

SoftRcPulseOut servo;

int pos = 0;    // variable to store the servo position
int maxTime=35;
int minTime=5;

void setup() {

	BT.begin(baudrate);
	servo.attach(0); 
    pinMode(1,OUTPUT);//led
    digitalWrite(1,1);
    delay(1000);
    digitalWrite(1,0);

}


char READ= '2';
bool isConnected = false;
bool isBusy = false;
void loop(){

	if(BT.available() && !isConnected){

		READ = char(BT.read());
		for(int i=0; i<3; i++){

			delay(200);
			digitalWrite(1,1);
			delay(200);
			digitalWrite(1,0);
		}
		isConnected= true;
	}

	if(isConnected && BT.available() && !isBusy){
		
		READ = char(BT.read());
		digitalWrite(1,1);
		isBusy = true;
	}

	if(READ=='1'|| millis()>=delayTime){

		poke(maxEx,25);
		digitalWrite(1,0);
		delayTime = (random(minTime,maxTime)*60000) + millis();
		READ ='0';
	}  
}

void poke(int n,int t){
	isBusy = true;
	for(pos = minEx;pos<n; pos++) {
		servo.write(pos);              
		delay(t); 
		SoftRcPulseOut::refresh(1);   
	}
	delay(500);
	for(pos = n ;pos>minEx ; pos--) {
		servo.write(pos);              
		delay(t);    
		SoftRcPulseOut::refresh(1);
	}
	isBusy = false;
}
