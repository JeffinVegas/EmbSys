/*
	1) HAVE A 0.5 PERIOD, TOGGLE LED EVERY 0.25s
	2) ONCE OVF == 1, TURN LED ON FOR 1 SEC
	3) AFTER 1 SEC, CONTINUE 0.5 PERIOD LIKE NORMAL
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#define LEDPORT PORTB
#define LED_PIN PORTB2
#define LED_DDR DDRB

volatile int count;

int main(void)
{
	volatile int count = 0;
	LED_DDR |= (1 << LED_PIN);	// Set LED to output

	TCCR0A = 0;
	TCCR0B |= (1 << CS02) | (1 << CS00);	// clock divided by 1024

	TCNT0 = 0;
	TIMSK0 |= (1 << TOIE0);	// enable interrupt on overflow

	sei(); // turn on interrupts
	
	while (1);
}

// called after overflow of timer.
// toggle LED on/off
ISR(TIMER0_OVF_vect)
{
	if(count == 6){
		for(volatile unsigned int i=0; i<36; i++)
			for(volatile unsigned int j=0; j<255; j++);
				LEDPORT ^= (1 << LED_PIN);	//turn LED on
	count = 0;
	}
	else
		count++;
}
