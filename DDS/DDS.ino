#include<rEncoder.h>
#include <DDS.h>
#include <SPI.h>
const int maxSelection = 7;// 0->7
int maxFrq = 60000000; \\[Hz]

void setup(){
	pinInit();
}

void loop(){
	rEncoder Enc(A,B, PB, 0);
	menu();
}
#define AD9850_W_CLK 8
#define AD9850_FQ_UD 9
#define AD9850_SD	11
#define AD9850_RST	10

void pinInit(){
	pinMode(AD9850_W_CLK, OUTPUT);
	pinMode(AD9850_FQ_UD, OUTPUT);
	pinMode(AD9850_SD, OUTPUT);
	pinMode(AD9850_RST, OUTPUT);
}

int layer = 0; //horizontal pointer
int selection =0; //vertical pointer
int value=0;

void menu(){
	if(layer == 0){	// digit selection
		selection = trimm(Enc.updateEncoder(selection),0,maxSelection);
		updateGraphics();
		if(Enc.getPBState()){// if encoder button is pressed
			layer = 1;
		}
	}
	if(layer == 1){	//digit value setting 
		selection = trimm(Enc.updateEncoder(selection),0,maxSelection);
		updateGraphics();
		if(Enc.getPBState()){// if encoder button is pressed
			layer = 0;
		}
	}

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