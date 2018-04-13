#define F_CPU 8000000L

#include <avr/io.h>
#include <avr/interrupt.h> // interrupt
#include <util/delay.h> // delays

#define BUTTONPORT PORTD
#define BUTTONDDR DDRD
#define BUTTON_PIN PD2

volatile unsigned int n; // true/false value to determine if motor is on/off

// this interrupt turns the motor on/off
ISR(INT0_vect)
{
	EIFR |= (1 <<INTF0); // clear int flag
	PORTB ^= (1 << PORTB0);
}

int main(void)
{	 
	// set ports
	DDRB = 0xFF;
	BUTTONPORT = (1 << BUTTON_PIN);	// pull-up
	BUTTONDDR = (1 << BUTTON_PIN); // set PD2 as input (INT0 interrupt)
	DDRD |= (1 << PORTD6);	// PD.6 (OC0A) is an output

	// ADMUX and ADC config
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA = 0x87; // enables ADC, sets prescaler for ADC conversion
	ADCSRB = 0x00; // free running mode

	OCR0A = 0;
	TCCR0A |= (1 << COM0A1); // non-inverting mode
	TCCR0A |= (1 << WGM01) | (1 <<WGM00);	// fast PWM mode
	TCCR0B |= (1 << CS01); // prescaler 8
	
	// INT0 interrupt settings
	EIMSK = (1 << INT0);					// enables INT0 interrupt
	EICRA = (1 << ISC01) | (1 << ISC00);	// triggers INT0 on rising edge	
	
	n = 0; //initialize motor as turned off
	PORTB = 0x01;	//hbridge.forward
	
	sei(); // enable interrupts
		
	while (1)
	{
		ADCSRA |= (1 << ADSC); // start conversion
		while ( (ADCSRA&(1<<ADIF)) == 0 ) ; // wait for conversion to finish
		OCR0A = ADCH;
	}
}