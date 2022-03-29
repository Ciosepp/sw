//-hwFunct.cpp
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
void SerialDebug(){
    for (int i = 0; i < 49; i++){
   		 Serial.print(transient[i]);
 		}
 	Serial.println("");
}
