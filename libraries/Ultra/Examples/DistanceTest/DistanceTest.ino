#include "Ultra.h"

#define trig 2					// Left side internal Trigger pin
#define echo 3					// Left side internal Echo pin

#define w_distance 100

Ultra sensor(trig,echo,w_distance);

void setup(){
	Serial.begin(115200);
}

void loop(){
	Serial.print("Distance = ");
	Serial.println(sensor.distance());
	delay(1000);
}