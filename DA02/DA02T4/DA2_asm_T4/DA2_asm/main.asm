.org 0
	jmp  main
.org 0x20
	jmp  TIM0_OVF ;Timer0 overflow interrupt vector

main:
	LDI R16, 4 ;needed to toggle LED
	LDI R21, 0 ;OVF count
	SBI DDRB, 2 ;PB2 as output
	LDI R17,0  ;needed to toggle led

	;OUT PORTB,R17
	LDI R20,5 ;to set prescaler
	OUT TCCR0B,R20 ;Prescaler: 1024

	LDI R20, 0x01 ;can also use (1<<TOIE0)
	STS TIMSK0,R20 ;enable overflow interrupt
	SEI ;enable global interrupts
Loop: 	
	RJMP Loop ;infinite loop	

TIM0_OVF:
		
delay:
	IN R28, TCNT0 ;loading timer0 to R28
	CPI R28, 0xFF ;check for overflow
	BREQ HIGHINC ;jump to increment overflow count

cont:
	CPI R28, 0xB6 ;comparing if timer is 0xB6
	BREQ body ;if equal, check for OVF_count
	RJMP delay ;if not, loop back

HIGHINC:
	INC R21 ;increment overflow count
	LDI R20, 0 
	OUT TCNT0, R20 ;reset TCNT
	RJMP cont ;loop back to check timer bits

body:
	CPI R21,0x06 ;comparing if OVF_count is 0x06
	BRSH done ;if equal, branch to "done"
	RJMP delay ;if not, loop back
done:
	EOR R17,R16 ;XOR to toogle led
	OUT PORTB,R17 ;display LED
	LDI R21, 0 ;reset overflow count
	LDI R20, 0
	OUT TCNT0,R20 ;resetting the counter to 0 for next round
	LDI	R20, 5 ;Timer0: enabled, prescaler = 1024
	OUT	TCCR0B,	R20 ; prescaler = 1024
	RETI ;return from interrupt, interrupts enabled	