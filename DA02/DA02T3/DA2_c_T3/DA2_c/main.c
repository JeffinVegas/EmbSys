/*
	1) USE TIMER0 (TCNT0) TO CHECK FOR OVF
	2) ONCE OVF == 1, TOGGLE LED LIGHT
	3) RESTART TIMER0
	4) REPEAT
*/
#include <avr/io.h>

#define LEDPORT PORTB
#define LED_PIN PORTB2
#define LED_DDR DDRB

int main(void)
{
	volatile int count = 0; // initialize overflow count
	LED_DDR |= (1 << LED_PIN);	// Set LED to output
	LEDPORT &= ~(1 << LED_PIN);	// Initial state of OFF
	
	TCCR0A = 0; // normal mode
	TCCR0B = (1 << CS02) | (1 << CS00);	// Clock divided by 1024
	
	while (1)	// Loops forever
	{
		if(TCNT0 >= 255)
		{	
			// using this check and the two for loops inside
			//with cause the LED to toggle for 0.25 sec
			if(count == 6) 
			{
				for(volatile unsigned int i=0; i<38; i++)
				for(volatile unsigned int j=0; j<255; j++);
				LEDPORT ^= (1 << LED_PIN); //toggle LED light
				count = 0; //reset count to continuously toggle LED
			}
			else
				count++; // overflow occurred
		TCNT0 = 0; // reset timer
		}
	}
}