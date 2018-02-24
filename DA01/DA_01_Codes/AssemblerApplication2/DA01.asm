;razonj1
;1. Store 300 numbers starting from the STARTADDS=0x0222 location. Populate the value
;of the memory location by adding high(STARTADDS) and low(STARTADDS) . Use the X/
;Y/Z registers as pointers to fill up 300 numbers.

;2. Use X/Y/Z register addressing to parse through the 300 numbers, if the number is
;divisible by 5 store the number starting from memory location 0x0400, else store at
;location starting at 0x0600.

;3. Use X/Y/Z register addressing to simultaneously add numbers from memory location
;0x0400 and 0x0600 and store the sums at R16:R17 and R18:R19 respectively. Do not
;worry about the overflow.

;4. Verify your algorithm and answers using C programming.

;5. Determine the execution time @ 16MHz/#cycles of your algorithm using the simulation.

start:
; Declare POINTERS
	LDI	XL, LOW(0x0222)
	LDI	XH, HIGH(0x0222)
	LDI	YL, LOW(0x0400)
	LDI	YH, HIGH(0x0400)
	LDI	ZL, LOW(0x0600)
	LDI	ZH, HIGH(0x0600)

; Declare variables
	CLR	R16					; Stores total sum of numbers that are divisible by 5, set to 0
	CLR	R17					; R16 overflow
	CLR	R18					; Stores total sum of numbers that are not divisible by 5, set to 0
	CLR	R19					; R18 overflow

	CLR	R20					; Counter register (HIGH), set to 0
	CLR	R21					; Counter register (LOW), set to 0
	LDI	R23, HIGH(300)		; Immediate value 300 H (0x01), count checker
	LDI R24, LOW(300)		; Immediate value 300 L (0x2c), count checker

	CLR R22					; R26 = 0
	LDI R22, LOW(0x0222)	; R26 = 0x22
	ADD R25, R22			; R25 = R25 + R26
	LDI R22, HIGH(0x0222)	; R26 = 0x02

; Populate and store 300 numbers
popLOOP:
	ADD R25, R22			; R25 += R26
	ST X+, R25				; Store number
	INC R21					; increment counter (low)
	JMP check300first

; Increment higher bits	
changeHighfirst:
	INC R20					; increment higher bit
	CLR R21					; restarts lower bits
	JMP popLOOP

; Checks if counter == 300
check300first:
	CPI R21, 0x00			; Check to increment high bit
	BREQ changeHighfirst
	CP R24, R21				; Compare LOW(300) and R21
	BRNE popLOOP
	CP R23, R20				; Compare HIGH(300) and R20
	BRNE popLOOP

	CLR R20					; clear counter(high)
	CLR R21					; clear counter(low)
	CLR R22					; clear R22
	LDI R22, 0x05			; load a 5 to check if divisible

; Loads next number number in stack
loader:
	LD R15,	-X				; Load number to R20 downwards the stack
	MOV	R14, R15			; Moves value in R16 to check if divisible by 5

; checks if %5=0
D5:							; Checks for divisibility by 5
	CP R15, R22				; Compare R15 and R22 (5)
	BRLO ISNOT5				; Go to ISNOT5 back if R15 => R22
	CP	R15, R22			; Compare R15 and R22 (5)
	BREQ IS5				; Go to IS5 if R15 == R22
	SUB	R15, R22			; R15 = R15 - 5
	CP R15, R22				; Compare R15 and R22 again
	BRSH D5					; Loop back if R15 => R22

; when %5 != 0 (not divisible)
ISNOT5:
	ST Z+, R14				; Stores number
	ADD R18, R14			; Simultaneously add number (LOW BITS)
	LDI R19, 0				; We were told not to worry about overflow
	INC R21					; Increment counter
	JMP check300second		

; when %5 == 0 (divisible)
IS5:
	ST Y+, R14				; Stores number
	ADD R16, R14			; Simultaneously add number (LOW BITS)
	LDI R17, 0				; We were told not to worry about overflow
	INC R21					; Increment counter
	JMP check300second

; Increments higher bits
changeHighsecond:
	INC R20					; increment higher bit
	CLR R21					; restarts lower bits
	JMP loader

; Checks if counter == 300
check300second:
	CPI R21, 0x00			; Check to increment high bit
	BREQ changeHighsecond	; counter reaches 256
	CP R24, R21				; Compare LOW(300) and R21
	BRNE loader				; loop back if R24 != R21
	CP R23, R20				; Compare HIGH(300) and R20
	BRNE loader				; loop back if R23 != R20

DONE:
	NOP						; Program end