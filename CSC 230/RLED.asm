;Repeat illuminating LEDs randomly (according to the random number generated) with an interval as approximately 1 second. 
;For example, if the random number generated is 55, or 0b00110111 in binary, 
;then the LEDs to be illuminated are indexed as 1, 2, 3, 5 and 6. 

.def ZERO = r20
.def ONE = r21
.cseg
.org 0
	;Necessary initialization
	ldi ZERO, 0
	ldi ONE, 1
	ldi r22,0XFF
	ldi r23,0XFF
	out DDRB, r22
	sts DDRL, r23
	ldi r25, 19 ;load x0
	ldi r26, 13 ;load A
	ldi r28, 17 ;load C

repeat:
	;Generate a random number in range 0~63 and store it in R16
	clr r0
	clr r27
	clr r16
	mul r26, r25 
	mov r27, r0
	add r27, r28
	andi r27, 0b00111111
	mov r25, r27
	mov r16, r27
	
	;Illuminate LEDs according to value in R16
	clr r22
	clr r23
	;Get values for light in port B
	sbrc r16, 0 
	ori r22, 0b00000010
	sbrc r16, 1
	ori r22, 0b00001000
	;Get values for light in port L
	sbrc r16, 2
	ori r23, 0b00000010
	sbrc r16, 3
	ori r23, 0b00001000
	sbrc r16, 4
	ori r23, 0b00100000
	sbrc r16, 5
	ori r23, 0b10000000

	;Output values to lights
	out PORTB, r22
	sts PORTL, r23

	;Delay 1s
		clr r19
		clr r18
	loop1:
		clr r29
	loop2:
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		inc r29
		cpi r29, 20
		brlo loop2
		add r18, ONE
		adc r19, ZERO
		cpi r19, 250
		brne loop1
		;another round
		rjmp repeat