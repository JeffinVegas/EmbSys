#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LEDPORT PORTB
#define LED_PIN PORTB2
#define LED_DDR DDRB

#define BUTTONPORT PORTD
#define BUTTONDDR DDRD
#define BUTTON_PIN PORTD2
#define BUTTONPIN PD2

ISR(INT0_vect)
{
	unsigned char temp; // Saves current state of LED light
	EIFR |= (1 <<INTF0); // clear flag
	temp = LEDPORT;	// Save the LED light state
	LEDPORT |= (1 << LED_PIN);	// Turn LED on
	_delay_ms(1000);	// Keep LED on for 1 sec
	LEDPORT = temp;	// Restore state
}

int main(void)
{
	LED_DDR |= (1 << LED_PIN);	// Set LED to output
	LEDPORT &= ~(1 << LED_PIN);	// Initial state of OFF
	
	BUTTONPORT = (1 << BUTTONPIN);	// pull-up
	BUTTONDDR = (1 << BUTTONPIN);	// set PD2 as input (using for interrupt INT0)
	
	EIMSK = (1 << INT0);					// Enable INT0
	EICRA = (1 << ISC01) | (1 << ISC00);	// Trigger INT0 on rising edge
	
	sei(); // enables interrupt
	
	while (1)	// Loops forever
	{
		LEDPORT |= (1 << LED_PIN);	// turn LED on
		_delay_ms(250);	// 0.25 sec delay
		LEDPORT &= ~(1 << LED_PIN);	// turn LED off
		_delay_ms(250);	// 0.25 sec delay
	}
}