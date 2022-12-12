#include <SoftSerial.h>     
#include <Timer.h>
#define rxP 2
#define txP 3
#define led 1
#define maxEx 120
#define minEx 80
#include <SoftRcPulseOut.h> 

Timer Tconv;
Timer Tpulse;

SoftSerial BT(rxP, txP);
int baudrate=9600;
#define pulseLenght 200 //ms

int maxTime=35;
int minTime=5;

void setup() {

	BT.begin(baudrate);
	pinMode(4,OUTPUT);//low nmos
	pinMode(5,OUTPUT);//high nmos

    pinMode(led,OUTPUT);//led
    digitalWrite(led,1);
    delay(1000);
    digitalWrite(led,0);

    Tconv.ckInit(100,50); //starting backgriund 100Hz clock

}


char READ= '2';
bool isConnected = false;
bool isBusy = false;
void loop(){

	if(BT.available() && !isConnected){

		READ = char(BT.read());
		for(int i=0; i<3; i++){

			delay(200);
			digitalWrite(led,1);
			delay(200);
			digitalWrite(led,0);
		}
		isConnected= true;
	}

	if(isConnected && BT.available() && !isBusy){
		
		READ = char(BT.read());
		digitalWrite(led,1);
		isBusy = true;
	}

	if(READ=='1' &&isBusy){

		poke();
		digitalWrite(led,0);
		READ ='0';
	}  
}
bool trigged =false;
void poke(){
	
	if (trigged == false ){
		Tpulse.Trg()
	}
	
	isBusy = false;
}
