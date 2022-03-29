//hwFunct.cpp
void pinInit(){
  for(int i=0; i<3; i++){
    pinMode(ReadMuxAddressPin[i],OUTPUT);
    pinMode(WriteMuxAddressPin[i],OUTPUT);
  }
  pinMode(WriteMuxOutPin,OUTPUT);
  pinMode(ReadMuxInPin,INPUT);
  pinMode(gatePin,OUTPUT);
}

void dacInit(){    //funzione che genera le tensioni specifiche per ogni tasto
	dac.begin(0x61);
	dac.setVoltage(0, false);
	
  	for(uint32_t i=0; i<N; i++){
    	voltages[i]= i*4095/60;
        if(debugEnable)Serial.println(voltages[i]);
  	}  	
}
void  muxWrite(int val,int* Pins){
    uint8_t out =val;
    digitalWrite(Pins[0], out&1);//lsb 0000 0011 & 0000 0001 = 0000 0001 >0 ==> true
    digitalWrite(Pins[1], out&2);//    0000 0011 & 0000 0010 = 0000 0010 >0 ==> true
    digitalWrite(Pins[2], out&4);//msb 0000 0011 & 0000 0100 = 0000 0000 =0 ==> false
}