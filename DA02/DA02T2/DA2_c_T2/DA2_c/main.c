/*
	1) push button is initially off
	2) push button
	3) toggle LED light
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LEDPORT PORTB
#define LED_PIN PORTB2
#define LED_DDR DDRB

#define BUTTONPORT PORTD
#define BUTTONDDR DDRD
#define BUTTON_PIN PORTD2
#define BUTTONPIN PIND

short checkButton(); // prototype

int main(void)
{
	LED_DDR |= (1 << LED_PIN);	// Set LED to output
	LEDPORT &= ~(1 << LED_PIN);	// Initial state of OFF
	
	BUTTONPORT |= (1 << BUTTON_PIN);	// pull-up
	
	while (1)	// Loops forever
	{
		short pressed = checkButton(); // branch to
		if(pressed) // if button is pressed,
		{
			LEDPORT |= (1 << LED_PIN);	// turn LED on
			_delay_ms(1000); // 1 sec delay	
			LEDPORT &= ~(1 << LED_PIN); // turn LED back off	
		}
	}
}

short checkButton()
{
	short count = 0; // counter
	
	while(!(BUTTONPIN & (1 << BUTTON_PIN)) && // check if button is stayed press
			count < 10) //for 0.1 sec
	{
		// counter to make sure button is being pressed for 0.1 sec
		count++;
		_delay_ms(5);
	}
	
	return (count == 10);
	// return true if button has been held for 0.1 sec
}