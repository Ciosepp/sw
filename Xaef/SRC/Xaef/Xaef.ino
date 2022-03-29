long delayTime=1000;
bool ON;
#define maxEx 180
#define minEx 90
#include <Servo.h>

Servo servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
}


int latch=false;
void loop(){
   if(analogRead(14)&&!latch){
      for(int i=0;i<12;i++){
          servo.write(maxEx);
          delay(1000);
          servo.write(minEx);
          delay(100);
      }
      
      
   }
    if(millis()>=delayTime){
        for(pos = 0; pos< maxEx; pos++) {
            servo.write(pos);              
            delay(25);                      
    }
    delay(10000);
    servo.write(minEx);
    delayTime=millis()+(random(5,60)*60000);
  
}
