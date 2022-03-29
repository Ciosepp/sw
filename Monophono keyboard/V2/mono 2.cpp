//gestore polifonia /monofonia
const int N =49; //n. tasti
int keyboard[N]; //array contenente gli stati di tutti i pulsanti
#define midiOutPin 1
#define ckIOPin 2
#define gatePin 3
#define ckBlinkPin 4
#define WriteMuxOutPin 5
int ReadMuxAddressPin[]={6,7,8};
#define ReadMuxInPin 9
int WriteMuxAddressPin[]={10,11,12};
#define rowNinePin 13
#define arpeggiatorModePin 14
#define arpeggiatorCkDivisionPin 15
#define arpeggiatorCkRatePin 16
#define arpeggiatorOctPin 17
#define SequencerPin 20

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
const int maxArpVoices= 7;

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
int keyboardScan(bool hold){
    int arpCount=0;
    int pressedKeys=0;
    if(!hold){
    	for(int w=0; w<W; w++){//0->8 columns
            if(w<8){
                digitalWrite(WritePinExt,0);
                muxWrite(w,WriteMuxPin);
      		    digitalWrite(WritePin,1);
            }
            else{
                digitalWrite(WritePinExt,1);
                digitalWrite(WritePin,0);
            }
            /*digitalWrite(WritePinExt,if(w==8));
            muxWrite(w,WriteMuxPin);
            digitalWrite(WritePin,if(w!=8));*/
      		for(int r=0; r<R; r++){//0->5 rows

                if(w==8&&r==1) break;//stop at key 49(48)
                muxWrite(r, WriteMuxPin);
                keyboard[w*R+r]=digitalRead(ReadPin);

                if(oldkeyboard[w*R+r]){
                    pressedKeys++;
                    if(arpCount<maxArpVoices){//fill arpegg. array with note to play
                        arpCount++;
                        arpeggioNotes[arpCount]=w*R+r;
                    }
                }
           		transient[w*R+r]= oldkeyboard[w*R+r] ^ keyboard[w*R+r];//XOR
           		oldkeyboard[w*R+r]=keyboard[w*R+r];//updating memory vector
    		}
    	}
    }
    return pressedKeys;
}


/////////////////////////////////////////////////////ARPEGGIATOR/////////////////////
/*
modes:  0- incremental 
		1- U/D
		2- decremental
		
*/

int arpeggioNotes[maxArpVoices];
bool filled=false;
int nVoices=0;
bool gateOn=false; //to adress as output at int/ext ck function
bool aLatch=true;
int c=0;
bool up=true;
void arpeggiator(int mode,bool enable){
	keyboardScan();
	if(gateOn && aLatch && nPressedKeys>0){
		dac.setVoltage(voltages[ arpeggioNotes[c]], false);
       	gateRefresh();
       	aLatch=false;
	}
	if(!gateOn && !aLatch){
        
		switch(mode){
			case 0:	
                c=(c+1)%maxArpVoices;
                break;
            case 1: 
                if(c>=maxArpVoices-1 && up) up=false;//up
                if(c<=0 && !up) up=true;
                if(up) c++;
                else    
                    c--;
                break;
            case 2: 
                c=(c-1)%maxArpVoices;
                break;
		}
        aLatch=true;
	}

    for(int i=0; i<maxArpVoices; i++){
        arpeggioNotes[i]=-1;
    }
}

int mono(int oldNote){
	bool Release=true;
	int y=oldNote;
	int nPressedKeys=0;
  	for(int i=0; i<N; i++){
       	
       	if(noteDown(i, keyboard, transient)){//detect note down
       		y= i;
       		dac.setVoltage(voltages[i], false);
       		gateRefresh();
       		Serial.println(i);
            nPressedKeys++;
       		break;
       	}
       	if(noteUp(i, keyboard, transient)){//detect note release
       		Release=true;
            gateOff();
            nPressedKeys--;
            break;
       	}
        if(Release&& nPressedKeys==0){//only pressed key release handler
            gateOff();
            Release=false;
            break;
        }
		if(Release&& keyboard[i]){//release handler selects the first secondly pressed key 
            y= i;
            dac.setVoltage(voltages[i], false);
            gateRefresh();
            Release=false;
            break;
        }
  	return y;//pressed key out
}

bool noteDown(int i, bool* noteScan, bool* noteTrans){
    return noteScan[i]&&noteTrans[i];
}
bool noteUp(int i, bool* noteScan, bool* noteTrans){
    return noteScan[i]&& !noteTrans[i];
}
void gateRefresh(){
    digitalWrite(gatePin,0);
    digitalWrite(gatePin,1);
}
void gateOn(){
    digitalWrite(gatePin,1);
}
void gateOff(){
    digitalWrite(gatePin,0);
}


void pinInit(){
  for(int i=0; i<3; i++){
    pinMode(ReadMuxAddressPin[i],OUTPUT);
    pinMode(WriteMuxAddressPin[i],OUTPUT);
  }
  pinMode(WriteMuxOutPin,OUTPUT);
  pinMode(ReadMuxInPin,INPUT);
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
  Serial.print("Mono keyboard firmware 2.0 rev.C");
  for(int i=0; i>N; i++)oldkeyboard[i]=0;
  dacInit();
}

int oldnote=0;
void loop(){
    n=mono(oldnote);
    oldnote=n;
    if(debugEnable)SerialDebug();
}

void  muxWrite(int val,int* Pins){
    uint8_t out =val;
    digitalWrite(Pins[0], out&1);//lsb 0000 0011 & 0000 0001 = 0000 0001 >0 ==> true
    digitalWrite(Pins[1], out&2);//    0000 0011 & 0000 0010 = 0000 0010 >0 ==> true
    digitalWrite(Pins[2], out&4);//msb 0000 0011 & 0000 0100 = 0000 0000 =0 ==> false
}

