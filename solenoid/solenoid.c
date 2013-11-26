/* We need this defined before we include util.delay.h
 * so _delay_ms() knows how many clock cycles 1 ms is */
#define F_CPU 1000000U	// 1Mhz clock speed

/* Includes */
#include <avr/io.h>		// for port names
#include <util/delay.h>	// for _delay_ms
#include <stdint.h>		// for uint8_t
#include <avr/interrupt.h>	// for ISR()

/* Global variables.
 * We only have these so that the interrupts can
 * pass data to the rest of the program.
 * So, they must be volatile so the compiler doesn't
 * assume the value has remained the same.
 */
volatile uint8_t data;	// only the interrupt writes to this


/* Function prototypes */


/* This is where our initialization happens.
 * This is a pretty simple program, so there
 * probably won't be any function calls in here.
 */
void init( void );


/* This is our program that verifies our data
 * is correct.  It prevents us from doing unintentional
 * things like trying to extend and retract a piston
 * at the same time.  (This particular situation would
 * actually retract the piston, but at a slower rate
 * than normal) */
uint8_t data_checker( uint8_t to_be_checked );


int main( void ) {
	/* this is where we put data we've verified */
	uint8_t verified_data;


	init();	// set up everything


	while( 1 ) {
		verified_data = data_checker( data );
		PORTA = verified_data;
		_delay_ms( 1 ); // do nothing
	}
	return 0;
}


/****************************************
 ****    Interrupts     *****************
 ***************************************/

/* When external interrupt is pulled low:
 * Start listening to USI.
 */
ISR( INT0_vect ) {
	USISR = 0;	// clear USI counter

	/* Enable the USI counter overflow interrupt.
	 * Put in SPI mode, and clock in rising edge
	 */
	USICR = 0x58;
}

/* When byte is received:
 * Copy data over so it can be handled in main.
 * 
 * If slave select pin is released: 
 * Disable USI.
 * Don't drive input or output.
 *
 * If we were also sending data back, this might be a good time
 * to put that data in the register.
 */
ISR( USI_OVF_vect ) {
	data = USIDR;	// copy data over

	if( !(PINB & 0x40) ){ // if message is complete
		USICR = 0;	// disable USI
	}

}




/* Catch any unintended interrupts */
ISR( BADISR_vect ) {
	// do nothing
}


/*************************************
 *********    Functions     **********
 ************************************/

void init( void ) {
	DDRA = 0xFF;	// PORTA is all outputs
	DDRB = 0;		// PORTB is all inputs
	
	/* TODO:  Set up watchdog to disable the outputs
	 * if info isn't received soon enough */


	/* Set up SPI */
	// Most of this is actually done in the INT0 interrupt.
	USIPP = 0;	// use default pin position

	/* Set up external interrupt */
	MCUCR = (MCUCR & 0xFC) | 0b10;	// INT0 should be rising edge
	GIMSK = 0x40;	// enable INT0

	sei();	// enable interrupts
}



uint8_t data_checker( uint8_t checkme ) {
	/* If we're trying to extend AND retract the same piston,
	 * Don't do either.
	 */
	if( (checkme & 0x80) && (checkme & 0x40) ){
		checkme &= 0x3F;
	}
	if( (checkme & 0x20) && (checkme & 0x10) ){
		checkme &= 0xCF;
	}
	if( (checkme & 0x08) && (checkme & 0x04) ){
		checkme &= 0xF3;
	}
	if( (checkme & 0x02) && (checkme & 0x01) ){
		checkme &= 0xFC;
	}

	//checkme has now been checked.
	return checkme;
}
