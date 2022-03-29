
#include "U8glib.h"
#include <Wire.h>
#include <BMP180.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

int x=40; //Posizione x testo schermo LCD
float R=287.05; //Costante specifica dei Gas Perfetti 
float p0;
float a;

BMP180 barometer;

float height;
float pressure;
float temperature;
float density;
#define n_samples_first_layer 10
#define n_samples_second_layer 20

#define sampling_time 100
#define log_refresh_time 100
bool sampling=true;

void setup() {  
	Serial.begin(9600);

	barometer.init();
	p0=101325;

	if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
}
else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
}
else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
}
else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
	u8g.setHiColorByRGB(255,255,255);
}
Serial.println(u8g.getMode());
}
int count1=0;
int count2=0;
void loop()
{ 
	if(sampling){
		Serial.println("sampling...");
		pressure=0;
		temperature=0;
		height=0;
        for(int i=0; i<n_samples_second_layer; i++){ //average summer

        	pressure += barometer.getPressure();
        	temperature +=barometer.getTemperature();
        	height +=barometer.getAltitude(p0);
        	delay(sampling_time);

        }
        pressure = pressure/n_samples_second_layer;    //now we have average values
        temperature =temperature/n_samples_second_layer;
        height =height /n_samples_second_layer;
    }

    density= pressure*100/(R*(temperature+ 273.15));
    a= sqrt(1.4* 287.53*(temperature+273.15));
    Serial.println(pressure);

  /*for(int i=0;i<n_s;i++){
  h = h + barometer.GetAltitude(p0);
  p = p + barometer.GetPressure();
  T = T + barometer.GetTemperature();
  delay(pause);
  }*/

  /*float h_m=h/n_s;
  long p_m=p/n_s;
  float T_m=T/n_s ;
  float rho_m= p_m/(R*(T_m+273.15));*/



    u8g.firstPage();  

    do {
    	draw();
    } 
    while( u8g.nextPage() );

}

void draw() {

  //u8g.setFont(u8g_font_profont12);
	u8g.setFont(u8g_font_unifont);
	u8g.drawStr( 0, 12, "h: ");
	u8g.drawStr( 100, 12, "m");
  	u8g.setPrintPos(x,12);  //110,22 for eol
	u8g.print(int(height));

	u8g.drawStr( 0, 24, "P: ");
	u8g.drawStr( 100, 24, "hPa");
	u8g.setPrintPos(x,24);  //110,22 for eol
	u8g.print(pressure);

	u8g.drawStr( 0, 36, "T: ");
	u8g.drawStr( 100, 36, "C");
	u8g.setPrintPos(x,36);  //110,22 for eol
	u8g.print(temperature);

	u8g.drawStr( 0, 48, "Rho: ");
	u8g.drawStr( 90, 48, "Kg/m^2");
	u8g.setPrintPos(x,48);  //110,22 for eol
	u8g.print(density);

	u8g.drawStr( 0, 60, "a: ");
	u8g.drawStr( 90, 60, "m/s");
	u8g.setPrintPos(x,60);  //110,22 for eol
	u8g.print(a);
}
