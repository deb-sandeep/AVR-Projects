#define F_CPU 1000000L
#define NUM_BUTTONS 2

#include <avr/io.h>
#include "common_defs.h"
#include "button.h"
#include "lcd.h"

void initProgram(void) {
	
	// Button definitions
	uint8_t btnData[][2] = {
		{PORT_D, PIN2},
		{PORT_C, PIN0}
	};
	initButtons( btnData ) ;
	
	// Output port - Pin 0,1 are output pins
	DDRD  = 0b00000011 ;

	// Init screen
	initLCD() ;
	writeStringToLCDatRC( 0, 0, "Btn 1 = " ) ;
	writeStringToLCDatRC( 1, 0, "Btn 2 = " ) ;
}

int main(void) {

	initProgram() ;
	
	int numBtn1Press = 0 ;
	int numBtn2Press = 0 ;
	
	while(1) {
		if( isButtonPressed( 0 ) ) {
			PORTD ^= ( 1 << PIND0 ) ;
			numBtn1Press++ ;
			writeIntToLCDatRC( 0, 8, numBtn1Press ) ;
		}
		
		if( isButtonPressed( 1 ) ) {
			PORTD ^= ( 1 << PIND1 ) ;
			numBtn2Press++ ;
			writeIntToLCDatRC( 1, 8, numBtn2Press ) ;
		}
	}
}