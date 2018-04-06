#include <avr/io.h>
#include <stdint.h> // needed for uint8_t
#include <avr/interrupt.h>
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD -1

volatile uint8_t ADCvalue; // Global variable, set to volatile if used with ISR

void initUart();

int main( void )
{
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 16Mhz
	ADCSRA |= (1 << ADATE); // Set ADC Auto Trigger Enable
	ADCSRB = 0; // 0 for free running mode
	ADCSRA |= (1 << ADEN); // Enable the ADC
	ADCSRA |= (1 << ADIE); // Enable Interrupts
	ADCSRA |= (1 << ADSC); // Start the ADC conversion
	
	initUart();
	
	//timer interrupt initialization	
	TCNT1 = 49911; //timer1 number to make 1 sec delay per reading
	TCCR1A = 0; //normal mode
	TCCR1B = (1 << CS12) | (1 << CS10); //prescaler
	TIMSK1 = (1 << TOIE1); //enble interrupt
	
	sei();
	while(1)
	{
	}
}


ISR(TIMER1_OVF_vect) {
	TCNT1 = 49911;	//restart timer
	TIFR1 = (1 << TOV1);	// clear flag
	while(!(UCSR0A & (1 << UDRE0)));	//wait for uart to finish
	UDR0 = ADCvalue;	//transmit to computer
}

ISR(ADC_vect)
{
	ADCvalue = ADCH * 2; // classify ADCvalue has high 8 bits
}

void initUart() {
	/*Set baud rate */
	UBRR0H = ((MYUBRR) >> 8);
	UBRR0L = MYUBRR;

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // Enable receiver and transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Set frame: 8data, 1 stp
}