/*
 * rled.asm
 *
 *  Created: Feb 8, 2019
 *   Author: Amaan Makhani
 *	   vnum: V00883520
 */ 

 .def ZERO = r20
 .def ONE = r21

 .cseg

.org 0

	ldi ZERO, 0
	ldi ONE, 1

	;necessary initialization
	ldi r22,0XFF
	ldi r23,0XFF
	out DDRB, r22
	sts DDRL, r23
	
	ldi r25, 19 ;load x0
	ldi r26, 13 ;load A
	ldi r28, 17 ;load C

repeat:

	; generate a random number in range 0~63 and store it in R16
	
	clr r0
	clr r27
	clr r16
	
	mul r26, r25 ;multiply A and Seed
	mov r27, r0 ;move to reg 27
	add r27, r28 ;add constant
	andi r27, 0b00111111 ;modulus 64 which is 2^6 so only first 6 digits matter get rid of the rest of the numbers
	mov r25, r27 ;move generated number to register 17 to redo seed
	mov r16, r27 ;move generated number to register 16 
	
	; illuminate LEDs according to value in R16
	clr r22
	clr r23

	;get values for light in port B
	sbrc r16, 0 
	ori r22, 0b00000010
	sbrc r16, 1
	ori r22, 0b00001000

	;get values for light in port L
	sbrc r16, 2
	ori r23, 0b00000010
	sbrc r16, 3
	ori r23, 0b00001000
	sbrc r16, 4
	ori r23, 0b00100000
	sbrc r16, 5
	ori r23, 0b10000000

	;output values to lights
	out PORTB, r22
	sts PORTL, r23

	; delay 1s
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
	
	;"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avrdude.exe" -C "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf" -p atmega2560 -c wiring -P COM3 -b 115200 -D -F -U flash:w:AssemblerApplication1.hex
