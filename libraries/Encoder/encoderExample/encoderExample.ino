#include <rEncoder.h>

rEncoder encoder(3, 4, 5, false);

void setup() {
  
    Serial.begin(9600);
    //while (!Serial){}
}
int y=0;
void loop() {

    y = encoder.updateEncoder(y);
    Serial.println(y);
    if(encoder.getPBState())Serial.println("plup");
    
}
