.INCLUDE "M328pDEF.INC"

.org 0

	LDI R16, 4 ;needed to toggle LED
	SBI DDRB, 2 ;PB2 as output
	CBI DDRD, 2 ;PD2 as input
	LDI R17, 0  ;needed to toggle led

	;OUT PORTB,R17
	LDI R20,5 ;to set prescaler
	OUT TCCR0B,R20 ;Prescaler: 1024

button:
	SBIS PIND, 2 ;prompts for button
	JMP begin ;jumps to LED toggle function
	RJMP button ;loop back to prompt for button

begin:
	EOR R17,R16 ;XOR to toogle led
	OUT PORTB,R17 ;display LED
	RCALL delay ;calling timer to wait for 1 sec
	EOR R17,R16 ;XOR to toogle led
	OUT PORTB,R17 ;redisplay LED
	RJMP button ;jump to prompt for button again
delay:
	IN R28, TCNT0 ;loading lower bit of counter to R28
	CPI R28, 0xFF ;check for overflow
	BREQ HIGHINC ;jump to increment overflow count

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
	CPI R21,0x1E ;comparing if OVF_count is 0x1E
	BRSH done ;if equal, branch to "done"
	RJMP delay ;if not, loop back
done:
	LDI R21, 0 ;reset overflow count
	LDI R20, 0
	OUT TCNT0,R20 ;resetting the counter to 0 for next round
	RET
