
/*
	AD9850 variable frequency synewave generator up to 40 MHz
	Written by gAz on 2023 gen 4

	The rotary encoder and the LCD function as HMI to control the oscillator.
	The current set frequency is always displayied on screen and a cursor is 
	located underneath the selected digit; the rotation changes the selected digit.
	Pressing the knob allows the setting of the selected digit: rotating the knob will
	increase/decrease the value. Note that the digits' overflow changes the all whole
	frequency value so effectively the selected digit determines the amount of variation 
	per rotation step.
*/
#include<rEncoder.h>
#include <AD9850.h>

const int maxSelection = 7;// 0->7
const long maxFrq = 40000000; \\[Hz]
double trimFreq = 125000000;

#define pinA 12
#define pinB 13
#define pinP 14
rEncoder ENC(pinA, pinB, pinP, 0); //0 -> INPUT_PULLUP
bool flag = true;

const int W_CLK_PIN = 9;
const int FQ_UD_PIN = 10;
const int DATA_PIN = 11;
const int RESET_PIN = 12;

#include <LiquidCrystal.h>
#define rs 2
#define en 3
#define d4 4
#define d5 5
#define d6 6 
#define d7 7
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
void setup(){
	pinInit();
	lcd.begin(16, 2);

  	DDS.begin(W_CLK_PIN, FQ_UD_PIN, DATA_PIN, RESET_PIN);
 	DDS.calibrate(trimFreq);
}

void loop(){
	menu();
}

void pinInit(){
	pinMode(AD9850_W_CLK, OUTPUT);
	pinMode(AD9850_FQ_UD, OUTPUT);
	pinMode(AD9850_SD, OUTPUT);
	pinMode(AD9850_RST, OUTPUT);
}

int layer = 0; //horizontal pointer
int selection =3, oldSelection =0 ; //vertical pointer
long frequency =100, oldFrequency =frequency;
long oldFrequencyGraphic= 0;
long multipliers[8]={   1,
						10,
						100,
						1000,
						10000,
						100000,
						1000000,
						10000000};

int lcdPos[] ={3,4,6,7,8,10,11,12};

void menu(){
	if(layer == 0){	// digit selection
		selection = trimm(Enc.updateEncoder(selection),0,maxSelection);
		if(Enc.getPBState() && flag){// if encoder button is pressed
			layer = 1;
			flag = false;
		}
	}

	if(layer == 1){	//digit value setting 
		frequency = trimm(Enc.updateEncoderMult(frequency,multipliers[selection]),1,maxFrq);
		
		updateDDS();
		if(Enc.getPBState() && flag){// if encoder button is pressed
			layer = 0;
			flag = false;
		}
	}
	if(!Enc.getPBState() && !flag) flag =true;
	updateGraphics();

}
int trimm(int in, int max, int min){
	if(in < min){
		in = min;
	}
	if(in > max){
		in =max;
	}
	return in;
}

void updateDDS(){
	if(frequency != oldFrequency){
		DDS.setfreq(freq, 0);
		oldFrequency = frequency;
	}
}


void updateGraphics(){
	if(oldFrequencyGraphic != frequency){
		for(int i=0; i<8;i++){
     		if(i == 2 || i== 5)lcd.print(".");
        	lcd.print(int(f/multipliers[7-i]));
        	f -= int(f/multipliers[7-i])*multipliers[7-i];
  		}
  		lcd.print(" Hz");
  		oldFrequencyGraphic = frequency;
	}
	if(selection != oldSelection){
		lcd.setCursor(lcdPos[selection],0);
		lcd.cursor();
		oldSelection = selection;
	}
}