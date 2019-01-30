;
; LCM_V00883520.asm
;
; Created: 1/23/2019 12:02:51 PM
; Author : amaanmakhani
; A1 code number 5

.cseg

.org 0

	ldi r16, 19 ;load x0
	ldi r17, 13 ;load A
	ldi r18, 17 ;load C
	mul r17, r16 ;multiply A and x0
	add r18, r0 ; add constant to multiplication
	andi r18, 0b00111111 ; modulus 64 which is 2^6 so only first 6 digits matter get rid of the rest of the numbers
	mov r16, r18 ; move generated number to register 16 

done:
	rjump done
