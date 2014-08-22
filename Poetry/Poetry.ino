#include <SPI.h>					// Import SPI Lib
#include <SdFat.h>					// Import SD card Lib
#include <SdFatUtil.h>				
#include <SFEMP3Shield.h>			// Import MP3 Shield Lib

#include <Wire.h>					// Import TWI lib

SdFat sd;							// SD Card Object		
SFEMP3Shield MP3player;				// MP3 Shield Object	

const int chipSelect = 9;

#define g_min 0 					// Min number for Greating message
#define g_max 3						// Max number for Greating Message 

#define a_min 4						// Min number for ask to stay message
#define a_max 6 					// Max number for ask to stay message

#define c_min 7  					// Min number for complaint message
#define c_max 10 					// Max number for complaint message

#define t_min 11 					// Min number for poetry
#define t_max 211 					// Max number for poetry

#define time_to_retunr 5000			// Time to retunr after the ask to stay message

bool change = false;				// Check if is any Track is running
bool timeout = false;				// Timeout before the complaints
bool timer_activated = false;		

int t_number = 0;					// Track number
int t_last = 0;						// Last Track Number
int32_t t_pos;						// Track Position

int a_number;						// Ask to stay track number
int a_last;							// Last Ask to stay track number

int c_number = 0;					// Complaints track number
int c_last = 0;						// last complaints track number

int g_number = 0;					// Gretings track number
int g_last = 0;						// Last gretings track number

uint8_t state = 0;					// State 

double retunr_timer = 0;			// Retunr timer variable

int counter = 0;					// People Counter

void setup()
{
	Wire.begin(0x0A);							// Start of the I2C port
	Wire.onReceive(receiveEvent);				// On even received over I2C
	Serial.begin(115200);						// Serial port Using to debug
	sd.begin(chipSelect,SPI_HALF_SPEED);		// Begin the SD card
	MP3player.begin();							// Begin the MP3 circuit
	// Serial.println("--- Serial Debug ---");
}

void loop()							// Main Loop	
{
	poetry();						// Main Funtion
	delay(100);
	SerialDebug();
}

/*Main funtion*/

int poetry(){
	/*Gretings!*/

	if((state == 0) && (counter >= 1)){							// If system is not running and at least one person is inside

		g_number = int(random(g_min,g_max+1));					// Find a random gretings number	

		while(g_number == g_last){								// Take a gretings diferent to the last one
			g_number = int(random(g_min,g_max+0.99));
		}

		g_last = g_number;										// Store the gretings number into the last gretings number
		MP3player.playTrack(g_number);							// Play the select gretings message
		state = 1;												// Change the state to 1
		
		// Serial Debug Lines
		DebugOutput(state,counter,g_number);


		return 0;	
	}

	// Play poetry

	if((state == 1) && (counter >= 1) && (change)){
		
		change = false;											// Change the system status to playing

		t_number = int(random(t_min,t_max+0.99));				// Find a random track number
		while(t_number == t_last){
		    t_number = int(random(t_min,t_max+0.99));			// Take a track diferent from the last one
		}

		t_last = t_number;										// Store the track numbe to last track number
		MP3player.playTrack(t_number);							// Play the select track
		state = 2;												// Change state to 2

		// Serial Debug Lines
		DebugOutput(state,counter,t_number);

		return 0;
	}

	// Ask to stay!

	if((state == 2 ) && (counter == 0)){

		t_pos = MP3player.currentPosition();					// Store the position of the playing track
		MP3player.stopTrack();

		a_number = int(random(a_min,a_max+0.99));				// Pick a random ask to stay message

		while(a_number == a_last){								
			a_number = int(random(a_min,a_max+0.99));			// Pick a ask to stay message diferent from the last one
		}

		a_last = a_number;										// Store the last ask to stay message
		MP3player.playTrack(a_number);							// Play the message
		state = 3;												// Change to state 3
		
		// Serial Debug Lines
		DebugOutput(state,counter,a_number);
		Serial.println(t_pos);

		return 0; 
	}

	// Timer Counter

	if(state == 3){
		if(change && !timer_activated){	
			// Serial.println("State = 3 - Complaints Timer Start");
			change = false;
			timer_activated = true;
			retunr_timer = millis();			
		}
		if(timer_activated){
			if(millis() - retunr_timer > time_to_retunr){
				timer_activated = false;
				timeout = true;
				// Serial.println("State = 3 - Complaints Timer Done!");	
			}
		}
		if(counter >= 1){
			// Serial.println("State = 3 - The peole is back!");
			change = true;
			timer_activated = false;
			timeout = false;
			
		}
	}

	// Complaints 

	if((state == 3) && (counter == 0) && (timeout)){
		
		timeout = false;

		c_number = int(random(c_min,c_max+0.99));

		while(c_number == c_last){
		    c_number = int(random(c_min,c_max+0.99));
		}

		c_last = c_number;
		MP3player.playTrack(c_number);
		state = 4;

		// Serial Debug Lines
		DebugOutput(state,counter,c_number);

		return 0;
	}

	// Return Poetry

	if((state == 3) && (counter >= 1) && (change)){

		change = false;
		MP3player.playTrack(t_number);
		MP3player.skipTo(t_pos);
		state = 2;

		// Serial Debug Lines
		DebugOutput(state,counter,t_number);
		Serial.println(t_pos);

		return 0;
	}

	if(!MP3player.isPlaying())
	{
		switch (state) {
		    case 1:										// If the Gretings message is finished
		    	change = true;							// If the Gretings message is finished and still people in the zone 
		    	if(counter == 0){						// If no one still in the zone
		    		change = false;
		    		state = 0;							// Returns to the base state
		    	}
		      	break;
		    
		    case 2:										
		    	change = false;							// If the poetry finish
		    	state = 0;								
		    	if(counter >= 1){						// If someone still in the zone the next poetry will be played
		    		state = 1;							
		    		change = true;
		    	}
		    	break;

		    case 3:										// If the ask to stay message finish
		    	change = true;				
		    	break;

		    case 4:
		    	change = false;							// If the complain message finish
		    	state = 0;
		    	break;
		}
	}
}

void receiveEvent(int rec){
	counter = int(Wire.read());
}

void SerialDebug(){
	if(Serial.available()){
		String output="";
		while(Serial.available()){
	  		output.concat(char(Serial.read()));
	  		delay(10);
		}
		counter = output.toInt();
	}
}

void DebugOutput(int st, int cou, int trk){
	char out[64];
	sprintf(out,"State = %u Counter = %u TrackNumber = %u \n --- --- --- \n",st, cou, trk );
	Serial.println(out);
}