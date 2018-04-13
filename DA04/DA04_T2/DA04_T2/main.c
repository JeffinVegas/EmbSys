#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h> //interrypts
#include <util/delay.h> // delays

#define BUTTONPORT PORTD
#define BUTTONDDR DDRD
#define BUTTON_PIN PD2
#define BUTTON_PIN_B PD3

void Clockwise(); //clockwise function
void CClockwise(); //counter-clockwise function
void delaytime(); // ctc delay function

volatile unsigned int n; // true/false value to determine if motor is on/off
volatile unsigned int m; // true/false value for motor direction
volatile uint8_t ADCvalue; // Global variable, set to volatile if used with ISR

// this interrupt turns the motor on/off
ISR(INT0_vect)
{
	EIFR |= (1 <<INTF0); // clear int flag
	if(n == 0)
		n = 1; // turn on motor
	else
		n = 0; // turn off motor
}

// this interrupt changes motor's direction
ISR(INT1_vect)
{
	EIFR |= (1 <<INTF1); // clear int flag
	if(m == 0)
		m = 1; // change to counter-clockwise
	else
		m = 0; // change to clockwise
}

int main(void)
{
	// set ports
	DDRB = 0x0F;
	BUTTONPORT = (1 << BUTTON_PIN) | (1 << BUTTON_PIN_B);	// pull-up
	// set PD2 and PD3 as input (INT0 and INT1 interrupt)
	BUTTONDDR = (1 << BUTTON_PIN) | (1 << BUTTON_PIN_B);
	
	// ADMUX and ADC config
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA = 0x87; // enables ADC, sets prescaler for ADC conversion
	ADCSRB = 0x00; // free running mode

	// set timer
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); // CTC mode, prescaler 64

	// INT0 and INT1 interrupt settings
	EIMSK = (1 << INT0) | (1 << INT1);		// enables INT0 and INT1 interrupts
	EICRA = (1 << ISC01) | (1 << ISC00);	// triggers INT0 on rising edge
	EICRA = (1 << ISC11) | (1 << ISC10);	// triggers INT1 on rising edge
	n = 0; //initialize motor as turned off
	
	sei(); // enable interrupts

	while (1)
	{
		ADCSRA |= (1 << ADSC); // start ADC conversion
		while ( (ADCSRA&(1<<ADIF)) == 0 ); // wait for conversion to finish
		ADCvalue = ADCH; // classify ADCvalue has high 8 bits
		if(n != 0 && m == 0) // if turned on and m = 0,
		{
			Clockwise(); // call to move motor clockwise.
			_delay_ms(1);
		}
		if(n != 0 && m != 0) // Otherwise,
		{
			CClockwise();// call to move motor counter-clockwise
			_delay_ms(1);
		}
	}
}

void delaytime()
{
	TCNT1 = 0; // initialize counter	
	OCR1A = ADCvalue * 100; // initialize top
	while(!(TIFR1 & (1 << OCF1A))); // waits for overflow
	TIFR1 |= (1 << OCF1A); // clears overflow flag
}

void Clockwise()
{
	PORTB = 0x06;
	delaytime();
	PORTB = 0x0C;
	delaytime();
	PORTB = 0x09;
	delaytime();
	PORTB = 0x03;
	delaytime();
}

void CClockwise()
{
	PORTB = 0x06;
	delaytime();
	PORTB = 0x03;
	delaytime();
	PORTB = 0x09;
	delaytime();
	PORTB = 0x0C;
	delaytime();
}

