#include <Wire.h>					// Import TWI lib
#include "Ultra.h"

#define w_distance 400				// Max distance to the wall in cm

#define lsi_trig 2					// Left side internal Trigger pin
#define lsi_echo 3					// Left side internal Echo pin

#define lse_trig 4 					// Left side external Trigger Pin
#define lse_echo 5 					// Left side external Echo Pin

#define rsi_trig 6					// Right side internal Trigger pin
#define rsi_echo 7 					// Right side internal Echo Pin

#define rse_trig 8					// Right side external Trigger pin
#define rse_echo 9 					// Right side external Echo pin

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
			Serial.println("left Half 0");
			counter--;
			if(counter < 0){
				counter = 0;
			}
			return 0;
		}
		else{
			l_half = true;
			Serial.println("left Half 1");
			return 0;
		}
	}

	if(ls_internal.check()){
		if(l_half){
			l_half = false;
			Serial.println("left Half 0");
			counter++;
			return 0;
		}
		else{
			l_half = true;
			Serial.println("left Half 1");
			return 0;
		}
	}

	if(rs_external.check()){
		if(r_half){
			r_half = false;
			Serial.println("right Half 0");
			counter--;
			if(counter < 0){
				counter = 0;
			}
			return 0;
		}
		else{
			r_half = true;
			Serial.println("right Half 1");
			return 0;
		}
	}

	if(rs_internal.check()){
		if(r_half){
			r_half = false;
			Serial.println("right Half 0");
			counter++;
			return 0;
		}
		else{
			r_half = true;
			Serial.println("right Half 1");
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