/*
	1) toggle LED light on
	2) delay 0.25s
	3) toggle LED light off
	4) delay 0.25s
	5) repeat
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LEDPORT PORTB
#define LED_PIN PORTB2
#define LED_DDR DDRB

int main(void)
{
	LED_DDR |= (1 << LED_PIN);	// Set LED to output
	LEDPORT &= ~(1 << LED_PIN);	// Initial state of OFF
	
	while (1)	// Loops forever
	{
		LEDPORT |= (1 << LED_PIN);	// turn LED on
		_delay_ms(250);	// 0.25 sec delay
		LEDPORT &= ~(1 << LED_PIN);	// turn LED off
		_delay_ms(250);	// 0.25 sec delay
	}
}