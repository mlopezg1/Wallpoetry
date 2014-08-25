#include <Wire.h>					// Import TWI lib
#include "Ultra.h"

#define w_distance 400				// Max distance to the wall in cm

#define lsi_trig 6					// Left side internal Trigger pin
#define lsi_echo 7					// Left side internal Echo pin

#define lse_trig 8 					// Left side external Trigger Pin
#define lse_echo 9 					// Left side external Echo Pin

#define rsi_trig 4					// Right side internal Trigger pin
#define rsi_echo 5 					// Right side internal Echo Pin

#define rse_trig 2					// Right side external Trigger pin
#define rse_echo 3 					// Right side external Echo pin

int counter = 0;
int l_counter = 0;
bool l_half = false;
bool r_half = false; 

Ultra ls_external(lse_trig,lse_echo,w_distance);
Ultra ls_internal(lsi_trig,lsi_echo,w_distance);

Ultra rs_external(rse_trig,rse_echo,w_distance);
Ultra rs_internal(rsi_trig,rsi_echo,w_distance);

void setup() {
	Wire.begin();
	Serial.begin(115200);
}

void loop() {
	chkZone();
	delay(500);
	sendCounter();
}

int chkZone(){
	if(ls_external.check()){
		if(l_half){
			l_half = false;
			Serial.println("lse:left Half 0");
			counter--;
			if(counter < 0){
				counter = 0;
			}
			return 0;
		}
		else{
			l_half = true;
			Serial.println("lse:left Half 1");
			return 0;
		}
	}

	if(ls_internal.check()){
		if(l_half){
			l_half = false;
			Serial.println("lsi:left Half 0");
			counter++;
			return 0;
		}
		else{
			l_half = true;
			Serial.println("lsi:left Half 1");
			return 0;
		}
	}

	if(rs_external.check()){
		if(r_half){
			r_half = false;
			Serial.println("rse:right Half 0");
			counter--;
			if(counter < 0){
				counter = 0;
			}
			return 0;
		}
		else{
			r_half = true;
			Serial.println("rse:right Half 1");
			return 0;
		}
	}

	if(rs_internal.check()){
		if(r_half){
			r_half = false;
			Serial.println("rsi:right Half 0");
			counter++;
			return 0;
		}
		else{
			r_half = true;
			Serial.println("rsi:right Half 1");
			return 0;
		}
	}
}

void sendCounter(){
	if(counter != l_counter){
		Wire.beginTransmission(0x0A);
		byte send_val;
		if(counter < 0){
			send_val = 0;
		}
		else{
			send_val = counter;
		}
  		Wire.write(send_val);                
  		Wire.endTransmission();
  		Serial.println("Number of persons = " + char(counter));
  		l_counter = counter;
	}
}