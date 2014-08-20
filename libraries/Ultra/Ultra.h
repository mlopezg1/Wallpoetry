#ifndef Ultra_h
#define Ultra_h

#include <Arduino.h>

class Ultra{
	public:
		Ultra(uint8_t,uint8_t,int);
		bool check();
		long distance();
	private:
		int max_dist;
		uint8_t trigger;
		uint8_t echo;
};
#endif