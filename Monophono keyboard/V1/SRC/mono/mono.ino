/*
 Questo firmware è il primo mai scritto per Monophono ed in teoria funziona.
 E' scritto per una millefori avente solo l'Arduino Pro Micro, il DAC MCP4725
 ed il connettore 8x2-2.54
*/
//gestore polifonia /monofonia
const int N =49; //n. tasti
//int n= 2; //ordine di polifonia
int keyboard[N]; //array contenente gli stati di tutti i pulsanti
int ReadPin[6]={A7,9,A6,8,6,7};
int WritePin[9]={10,11,4,12,5,13,A1,A0,A2};
int gatePin=2;
bool isEmpty=true;
uint32_t voltages[N];
int R=6,r=0;
int W=9,w=0;
int n=0;
#define pressed 1
#define unpressed 0
#define baudrate 115200
#define debugEnable true
//if(debugEnable)
#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

void dacInit(){    //funzione che genera le tensioni specifiche per ogni tasto
	dac.begin(0x61);
	dac.setVoltage(0, false);
	
  	for(uint32_t i=0; i<N; i++){
    	voltages[i]= i*4095/60;
        if(debugEnable)Serial.println(voltages[i]);
  	}  	
}

bool oldkeyboard[N];
bool transient[N];
bool gate=false,noteDown=false;
bool noteUp=false;

void keyboardScan(){

	for(int w=0; w<W; w++){//0->8 columns

  		digitalWrite(WritePin[w],1); //activate Read column

  		for(int r=0; r<R; r++){//0->5 rows

  	  		if(w==8&&r==2) break;//stop at key 49(48)

			if((r==0||r==2)){ //pin 0 and 2 are only analog
       	  		if(analogRead(ReadPin[r])>400){
       	  			keyboard[w*R+r]=1;//threshold
       	  			nPressedKeys++;
       	  		} 
      	  		else 
        			keyboard[w*R+r]=0;    
        	}
        	else {
        		if(digitalRead(ReadPin[r]) ){
       				keyboard[w*R+r]=1;//other pins can be readed digitally
       				nPressedKeys++;	
        		}
        		else
        			keyboard[w*R+r]=0;
        	}

       		transient[w*R+r]= oldkeyboard[w*R+r] ^ keyboard[w*R+r];//XOR
       		oldkeyboard[w*R+r]=keyboard[w*R+r];//updating memory vector
       		
		}
		digitalWrite(WritePin[w],0); //activate Read column
	}
}

/////////////////////////////////////////////////////ARPEGGIATOR/////////////////////
/*
modes:  0-hold - incremental 
		1-hold - decremental
		2-hold - U/D
		3-latch - incremental  
		4-latch - decremental
		5-latch - U/D
*/

const int maxArpVoices= 5;
int arpeggioNotes[maxArpVoices]={-1,-1,-1,-1,-1};
bool filled=false;
int nVoices=0;
bool gateOn=false; //to adress as output at int/ext ck function
bool aLatch=true;
int c=0;
void arpeggiator(int mode){
	keyboardScan();
	if(gateOn && aLatch){
		dac.setVoltage(voltages[ arpeggioNotes[c]], false);
       	digitalWrite(gatePin,0);
       	digitalWrite(gatePin,1);
       	aLatch=false;
	}
	if(!gateOn && !aLatch){
		switch(mode){
			case 0:
			
		}
	}


}

int mono(int oldNote){
	bool MonoLatch=true;
	int y=oldNote;
	int nPressedKeys=0;
  	for(int w=0; w<W; w++){//0->8 columns

  		digitalWrite(WritePin[w],1); //activate Read column

  		for(int r=0; r<R; r++){//0->5 rows

  	  		if(w==8&&r==2) break;//stop at key 49(48)

			if((r==0||r==2)){ //pin 0 and 2 are only analog
       	  		if(analogRead(ReadPin[r])>400){
       	  			keyboard[w*R+r]=1;//threshold
       	  			nPressedKeys++;
       	  		} 
      	  		else 
        			keyboard[w*R+r]=0;    
        	}
        	else {
        		if(digitalRead(ReadPin[r]) ){
       				keyboard[w*R+r]=1;//other pins can be readed digitally
       				nPressedKeys++;	
        		}
        		else
        			keyboard[w*R+r]=0;
        	}

       		transient[w*R+r]= oldkeyboard[w*R+r] ^ keyboard[w*R+r];//XOR
       		oldkeyboard[w*R+r]=keyboard[w*R+r];//updating memory vector
       		
       		if(MonoLatch && transient[w*R+r] && keyboard[w*R+r]){
       			y= w*R+r;
       			dac.setVoltage(voltages[y], false);
       			digitalWrite(gatePin,0);
       			digitalWrite(gatePin,1);
       			Serial.println(y);
       			noteDown=true;
       			MonoLatch = false;
       		}
       		if(MonoLatch && noteUp && keyboard[w*R+r]){
       			y= w*R+r;
       			dac.setVoltage(voltages[y], false);
       			digitalWrite(gatePin,0);
       			digitalWrite(gatePin,1);
       			Serial.println(y);
       			noteDown=true;
       			noteUp=false;
       			MonoLatch = false;
       		}
		}
		digitalWrite(WritePin[w],0); //activate Read column
	}
	if(noteUp){
		digitalWrite(gatePin,0);
		noteUp=false;
		if(debugEnable)Serial.println("gate OFF");
	}
	if(MonoLatch&& transient[y] && !keyboard[y] && noteDown){
		noteUp=true;
       	MonoLatch = false;
	}
  	return y;//pressed key out
}


void pinInit(){
  for(int i=0; i<R; i++){
    pinMode(ReadPin[i],INPUT);
  }
  for(int i=0; i<W; i++){
    pinMode(WritePin[i],OUTPUT);
  }
  pinMode(gatePin,OUTPUT);
}

void SerialDebug(){
    for (int i = 0; i < 49; i++){
   		 Serial.print(transient[i]);
 		}
 	Serial.println("");
	
}

void setup(){
  pinInit();
  Serial.begin(baudrate);
  Serial.print("Mono keyboard firmware 1.0 rev.C");
  for(int i=0; i>N; i++)oldkeyboard[i]=0;
  dacInit();
}

int oldnote=0;
void loop(){
    n=mono(oldnote);
    oldnote=n;
    if(debugEnable)SerialDebug();
}
