#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h> // delays

int main(void) {

	// set ports
	DDRB = 0xFF;

	// ADMUX and ADC config
	ADMUX = 0; // use ADC0
	ADCSRA = 0x87; // enable ADC, system clock used for A/D conversion
	ADCSRB = 0x00; // free running mode

	// timer config, ICR1 = TOP
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) |  (1 << WGM11); // Non-inverted PWM
	TCCR1B |= (1 << WGM13) | (1 << WGM12);  // Fast PWM mode
	TCCR1B |= (1 << CS11) | (1 << CS10); // Prescaler 64
	ICR1 = 2500; // top
	
	while (1)
	{
			ADCSRA |= (1 << ADSC); // start ADC conversion
			while( (ADCSRA&(1<<ADIF)) == 0 ); // wait for conversion to finish
			OCR1B = ADC * 0.3; // approximate value for pot. to rotate servo 180 degrees
	}
}