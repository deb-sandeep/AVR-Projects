#define F_CPU 1000000UL

#include "lcd.h"

int count = 0 ;
int countDir = 1 ;
int progressCount = 0 ;

void incrementCountOnScreen(void) {
	clearLCDRegion( 0, 8, 0, 10 ) ;
	writeIntToLCDatRC( 0, 8, count ) ;
	
	if( count >= 100 ) {
		countDir = -1 ;
	}
	else if( count <= 0 ) {
		countDir = 1 ;
	}
	count += countDir ;
}

void turnProgressWheel(void) {
	
	int oldRow=0, oldCol=0, newRow=0, newCol=0 ;
	
	if( progressCount == 0 ) {
		oldRow = 1; oldCol=14;
		newRow = 0; newCol=14;
	}
	else if( progressCount == 1 ) {
		oldRow = 0; oldCol=14;
		newRow = 0; newCol=15;
	}
	else if( progressCount == 2 ) {
		oldRow = 0; oldCol=15;
		newRow = 1; newCol=15;
	}
	else if( progressCount == 3 ) {
		oldRow = 1; oldCol=15;
		newRow = 1; newCol=14;
	}
	
	progressCount = (progressCount+1)%4 ;

	writeCharToLCDatRC( oldRow, oldCol, 0x20 ) ;
	writeCharToLCDatRC( newRow, newCol, 0xFF ) ;
}

void initScreen(void) {
	writeStringToLCDatRC( 0, 0, "Count = " ) ;
}

int main(void) {
	
	initLCD() ;
	initScreen() ;
	
	while(1){
		incrementCountOnScreen() ;
		turnProgressWheel() ;
		_delay_ms( 200 ) ;
	}
}
