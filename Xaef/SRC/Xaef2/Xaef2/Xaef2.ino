#include <SoftwareSerial.h>
#define rxP 7
#define txP 8
SoftwareSerial BT(rxP, txP);
int baudrate=9600;
long delayTime=1000;
bool ON;
#define maxEx 180
#define minEx 80
#include <Servo.h>
Servo servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int maxTimeMin=5;
int MinTimeMin=1;
void setup() {
   Serial.begin(baudrate);
   BT.begin(baudrate);
  servo.attach(5); 
  pinMode(3,INPUT_PULLUP);
  pinMode(2,OUTPUT),digitalWrite(2,0);//BT power
  pinMode(13,OUTPUT);
  
  if(digitalRead(3)==0){
    maxTimeMin=20;
    MinTimeMin=5;
    digitalWrite(2,1);
    //digitalWrite(13,1);
    delay(2000);
    //digitalWrite(13,0);
  }/*
  else{
    for(int i=0; i<3; i++){
        digitalWrite(13,1);
        delay(100);
        digitalWrite(13,0);
        delay(100);
    }*/
  
}

bool latch=false;
bool timerLatch=false;
long timer=0;
char READ= '0';
void loop(){
    if(BT.available()){
      READ = char(BT.read());
      Serial.println("ping");
      latch=false;
    }
    if(READ=='0'&& !latch){
      servo.write(minEx);
      latch=true;
    }
     if(READ=='1'&& !latch){
      poke(maxEx,25);
      latch=true;
    }
    if(!digitalRead(3)){
       burst(4);
    }  
    if(millis()>=delayTime && latch){
        poke(maxEx,50);
        delay(10000);
        servo.write(minEx);
        delayTime=millis()+(random(MinTimeMin,maxTimeMin)*60000);
    }
}
void burst(int n){
     for(int i=0;i<n;i++){
          servo.write(maxEx);
          delay(1000);
          servo.write(minEx);
          delay(1000);
       }
}
void poke(int n,int t){
  for(pos = minEx;pos<n; pos++) {
            servo.write(pos);              
            delay(t);    
        }
}
