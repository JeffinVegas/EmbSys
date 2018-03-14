.INCLUDE "M328pDEF.INC"

.org 0
	jmp main
.org 0x02
	jmp INT0_v ;external interrupt vector

main:
	LDI R16, 4 ;needed to toggle LED
	SBI DDRB, 2 ;PB2 as output
	CBI DDRD, 2 ;PD2 as input
	LDI R17, 0  ;needed to toggle led

	LDI R20,5 ;to set prescaler
	OUT TCCR0B,R20 ;Prescaler: 1024

	LDI R20, (1 << INT0)
	OUT EIMSK, R20 ;set bits for EIMSK
	LDI R20, (1 << ISC11) | (1 << ISC01) 
	STS EICRA, R20 ;set bits for EICRA
	SEI

button:
	SBI PIND, 2 ;prompts for button
	RJMP begin ;toggle LED if != button

begin:
	RCALL delay ;delay function
	EOR R17,R16 ;XOR to toogle led
	OUT PORTB,R17 ;display LED
	RJMP button ;jump to prompt for button again
delay:
	IN R28, TCNT0 ;loading lower bit of counter to R28
	CPI R28, 0xFF ;comparing if lower is 0x41
	BREQ HIGHINC

cont:
	CPI R28, 0xA0 ;comparing if timer is 0xA0
	BREQ body ;if equal, check for OVF_count
	RJMP delay ;if not, loop back

HIGHINC:
	INC R21 ;increment overflow count
	LDI R20, 0 
	OUT TCNT0, R20 ;reset TCNT
	RJMP cont ;loop back to check timer bits

body:
	CPI R21,0x07 ;comparing if higher is 0x07
	BRSH done ;if equal, branch to "done"
	RJMP delay ;if not, loop back
done:
	LDI R21, 0 ;reset overflow count
	LDI R20, 0
	OUT TCNT0,R20 ;resetting the counter to 0 for next round
	RET

INT0_v:
begin2:
	LDI R20, (1 << INTF0)
	OUT EIFR, R20 ;clear flag
	SBI PORTB, 2 ;turn LED light on
	RCALL delay2 ;delay function
	RETI ;return from interrupt, interrupts enabled	
delay2:
	IN R28, TCNT0 ;loading timer0 to R28
	CPI R28, 0xFF ;check for overflow
	BREQ HIGHINC2 ;jump to increment overflow count

cont2:
	CPI R28, 0x84 ;comparing if timer is 0xB6
	BREQ body2 ;if equal, check for OVF_count
	RJMP delay2 ;if not, loop back

HIGHINC2:
	INC R21 ;increment overflow count
	LDI R20, 0 
	OUT TCNT0, R20 ;reset TCNT
	RJMP cont2 ;loop back to check timer bits

body2:
	CPI R21,0x1E ;comparing if OVF_count is 0x1E
	BRSH done2 ;if equal, branch to "done2"
	RJMP delay2 ;if not, loop back
done2:
	LDI R21, 0 ;reset overflow count
	LDI R20, 0
	OUT TCNT0,R20 ;resetting the counter to 0 for next round
	RET