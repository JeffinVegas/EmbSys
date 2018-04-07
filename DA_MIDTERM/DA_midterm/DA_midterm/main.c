#define F_CPU 16000000UL

#include <stdlib.h>
#include <avr/io.h>
#include <stdint.h> // needed for uint8_t
#include <util/delay.h> // delays
#include <avr/interrupt.h> // interrupts

//calc baud rate
#define FOSC 16000000				// Clock speed
#define BAUD 115200					
#define MYUBRR FOSC/8/BAUD-1					

//AT commands for ESP
volatile unsigned char AT[] = "AT\r\n";
volatile unsigned char CIPMUX[] = "AT+CIPMUX=0\r\n";
volatile unsigned char CIPSTART[] = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n"; //ip of thingspeak
volatile unsigned char SEND_DATA[] = "GET /update?key=4YEZ22JG6Q4EMQEC&field1="; //update thingspeak

volatile unsigned char CIPSIZE[] = "AT+CIPSEND=45\r\n"; //send data
volatile unsigned char CWMODE[] = "AT+CWMODE=3\r\n"; //wifi mode
volatile unsigned char CONNECTWIFI[] = "AT+CWJAP=\"WIFI NAME\",\"WIFI PASSWORD\"\r\n"; //connect to ap
volatile unsigned char FIRMWARE[] = "AT+GMR\r\n"; //view version info
volatile unsigned char BREAK[] = "\r\n\r\n"; //assures end of reading temp

//global variables
volatile uint8_t ADCvalue;
volatile unsigned char temp[5];	

//prototypes
void initUart();
void send_AT(volatile unsigned char AT[]);

int main(void)
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
	
	_delay_ms(1500);
	send_AT(AT); //at
	_delay_ms(1500);
	send_AT(FIRMWARE); //firmware
	_delay_ms(1500);
	send_AT(CWMODE); //wifi mode
	_delay_ms(1500);
	send_AT(CONNECTWIFI); //connect with WiFi
	_delay_ms(5000);
	send_AT(CIPMUX); //enable
	
	sei();
	while (1)
	{
		_delay_ms(500);					
		send_AT(CIPSTART); // start connection
		_delay_ms(500);	
		send_AT(CIPSIZE); // size
		_delay_ms(500);	
		send_AT(SEND_DATA);
		send_AT(temp); //temperature
		send_AT(BREAK);
	}
	return 0;
}

void initUart() {
	/*Set baud rate */
	UBRR0H = ((MYUBRR) >> 8);
	UBRR0L = MYUBRR;

	UCSR0A |= (1<< U2X0);
	UCSR0B |= (1 << TXEN0); // Enable transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Set frame: 8data, 1 stp
}

// Interrupt subroutine for ADC value
ISR(ADC_vect) {
	unsigned char i;
	char tmptemp[5];
	
	ADCvalue = (ADCH << 1);	
	itoa(ADCvalue, tmptemp, 10); //convert char to ascii
	for(i = 0 ; i < 5 ; i++)
		temp[i] = tmptemp[i]; //move converted ascii
}

void send_AT(volatile unsigned char AT[]) {
	volatile unsigned char a;
	volatile unsigned char ATlength = 0;
	
	while(AT[ATlength] != 0)
		ATlength++; // find length
		
	for(a = 0 ; a < ATlength ; a++)
	{
		while(!(UCSR0A & (1<<UDRE0)));	// wait for UART
		UDR0 = AT[a];					// transmit char array
	}
}