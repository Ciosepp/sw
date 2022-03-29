//MonoV2.ino
//main file
#include <monophonoBoardPinHeader.h>


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



