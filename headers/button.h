//                        BUTTON_H
// A bunch of utility functions to ease handling of button presses along with
// debouncing logic.
//
// NOTE: A pulled-up button is considered pressed on release (rising edge)
//
// How to use this header?
// -----------------------
// 1. The program using this header needs to define NUM_BUTTONS, the value of 
//    which should be equal to the number of buttons being used by the program
//
// 2. Initializing the buttons - the 'initButtons' functions should be called 
//    with a two column, two dimensional array. Each row represents the meta
//    data for a single button. The first element of a row is the PORT (a,b,c,d)
//    to which the button is connected, while the second element is the pin
//    number in the port to which the button is connected. For example
//
//		uint8_t btnData[][2] = {
//			{PORT_D, PIN2}, // First button connected to pin2 port D
//			{PORT_C, PIN0}  // Second button connected to pin 0 of port C
//		} ;
//		initButtons( btnData ) ; // Calling the initiation funciton
//
//  3. Checking if a button is pressed by calling the isButtonPressed method.
//     This method takes an integer parameter representing the index of the 
//     button as defined during initialization. This function returns a non
//     zero integer if the button is pressed, 0 otherwise.
//
//     It is important to call this function within the main loop.
//
//		if( isButtonPressed( 0 ) ) {
//			...
//		}

#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifndef NUM_BUTTONS
	#error "NUM_BUTTONS is not defined"
#endif

#define CONFIDENCE_THRESHOLD 200

#include <avr/io.h>

struct Button {
	uint8_t* pinAddress ;
	uint8_t  pinNumber ;
	uint8_t  isArmed ;
	uint8_t  releasedConfidenceLevel ;
} ;

struct Button buttons[NUM_BUTTONS] ;

void initButtons( uint8_t data[][2] ) {
	for( int i=0; i<NUM_BUTTONS; i++ ) {
		uint8_t port   = data[i][0] ;
		uint8_t pinNum = data[i][1] ;

		if( port == PORT_A ) {
			buttons[i].pinAddress = &PINA ;
			DDRA &= ~( 1 << pinNum ) ;
			PORTA |= ( 1 << pinNum ) ;
		}
		else if( port == PORT_B ) {
			buttons[i].pinAddress = &PINB ;
			DDRB &= ~( 1 << pinNum ) ;
			PORTB |= ( 1 << pinNum ) ;
		}
		else if( port == PORT_C ) {
			buttons[i].pinAddress = &PINC ;
			DDRC &= ~( 1 << pinNum ) ;
			PORTC |= ( 1 << pinNum ) ;
		}
		else if( port == PORT_D ) {
			buttons[i].pinAddress = &PIND ;
			DDRD &= ~( 1 << pinNum ) ;
			PORTD |= ( 1 << pinNum ) ;
		}
		
		buttons[i].pinNumber = pinNum ;
		buttons[i].isArmed = 0 ;
		buttons[i].releasedConfidenceLevel = 0 ;
	}
}

// PIN     1111111111110101010110000000000000000000001010101001111111111111111
// Armed   0000000000001111111111111111111111111111111111111111111111111000000
// Pressed 0000000000000000000000000000000000000000000000000000000000001000000
int isButtonPressed( int btnNumber ) {
	
	struct Button* btn = &buttons[btnNumber] ;
	uint8_t btnDown = bit_is_clear( *(btn->pinAddress), btn->pinNumber ) ;

	if( !btn->isArmed && btnDown ) {
		btn->isArmed = 1 ;
	}
	
	if( btn->isArmed ) {
		if( btnDown ) {
			btn->releasedConfidenceLevel = 0 ;
		}
		else {
			btn->releasedConfidenceLevel++ ;
			if( btn->releasedConfidenceLevel >= CONFIDENCE_THRESHOLD ) {
				btn->isArmed = 0 ;
				btn->releasedConfidenceLevel = 0 ;
				return 1 ;
			}
		}
	}
	return 0 ;
}

#endif /* __BUTTON_H__ */