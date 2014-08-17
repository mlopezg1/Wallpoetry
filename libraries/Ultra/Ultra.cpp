#include "Ultra.h"

Ultra::Ultra(uint8_t trig, uint8_t ech,int dist){
	trigger = trig;
	echo = ech;
	max_dist = dist;
	pinMode(trigger,OUTPUT);
	pinMode(echo,INPUT);
	digitalWrite(trigger,0);
}

bool Ultra::check(){
	long duration, distance;
	digitalWrite(trigger,1);
	delayMicroseconds(10);
	digitalWrite(trigger,0);
  	duration = pulseIn(echo, HIGH);
  	distance = (duration/2) / 29.1;
  	if(distance < 350){
  		return true;
  	}
  	return false;
}