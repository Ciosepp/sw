//gestore polifonia /monofonia
const int N =49; //n. tasti
int keyboard[N]; //array contenente gli stati di tutti i pulsanti
#define midiOutPin 1
#define ckIOPin 2
#define gatePin 3
#define ckBlinkPin 4
#define WriteMuxOutPin 5
int ReadMuxAddressPin[]={6,7,8};
#define ReadMuxOutPin 9
int WriteMuxAddressPin[]={10,11,12};
#define WritePinExt 13
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
bool gate=false;

int keyboardScan(bool hold){
    int arpCount=0;
    int pressedKeys=0;
    if(!hold){
      for(int w=0; w<W; w++){//0->8 columns
            if(w==0){
                digitalWrite(WritePinExt,0);
                digitalWrite(WriteMuxOutPin,1);
            }
            if(w==8){
                digitalWrite(WritePinExt,1);
                digitalWrite(WriteMuxOutPin,0);
            }
            muxWrite(w,WriteMuxAddressPin);
            /*digitalWrite(WritePinExt,if(w==8));
            muxWrite(w,WriteMuxPin);
            digitalWrite(WritePin,if(w!=8));*/
          for(int r=0; r<R; r++){//0->5 rows

                if(w==8&&r==1) break;//stop at key 49(48)
                muxWrite(r, ReadMuxAddressPin);
                keyboard[w*R+r]=digitalRead(ReadMuxOutPin);

                if(oldkeyboard[w*R+r]){
                    pressedKeys++;
                    if(arpCount<maxArpVoices){//fill arpegg. array with note to play
                        arpCount++;
                        //arpeggioNotes[arpCount]=w*R+r;
                    }
                }
              transient[w*R+r]= oldkeyboard[w*R+r] ^ keyboard[w*R+r];//XOR
              oldkeyboard[w*R+r]=keyboard[w*R+r];//updating memory vector
        }
      }
    }
    return pressedKeys;
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
  
  pinMode(ReadMuxOutPin,OUTPUT);
  pinMode(WritePinExt,OUTPUT);
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
  Serial.print("Mono keyboard testing firmware 2.0 rev.C");
  for(int i=0; i>N; i++)oldkeyboard[i]=0;
  dacInit();
}

int oldnote=0;
void loop(){
    n=keyboardScan(true);
    if(debugEnable){
        SerialDebug();
    }
}

void  muxWrite(int val,int* Pins){
    uint8_t out =val;
    digitalWrite(Pins[0], out&1);//lsb 0000 0011 & 0000 0001 = 0000 0001 >0 ==> true
    digitalWrite(Pins[1], out&2);//    0000 0011 & 0000 0010 = 0000 0010 >0 ==> true
    digitalWrite(Pins[2], out&4);//msb 0000 0011 & 0000 0100 = 0000 0000 =0 ==> false
}
