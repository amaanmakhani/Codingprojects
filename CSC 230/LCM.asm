;LCM (Linear Congruential Method) is an algorithm for producing a sequence of random numbers in a recursive way. 
;Xi+1 = (AXi + C) mod M
;where A is the multiplier, C is the increment, and M is the modulus.
;Generate a 6-bit unsigned random integer (range from 0 to 63). 
;Suppose the parameters are A = 13, C = 17, M = 64 and the seed is X0 = 19

.cseg

.org 0
start:
	ldi r16, 19 ;load x0
	ldi r17, 13 ;load A
	ldi r18, 17 ;load C
	
Generate:
	mul r17, r16 ;multiply A and x0
	add r0, r18 ;add constant to multiplication
	andi r0, 0b00111111 ;modulus 64 which is 2^6 so only first 6 digits matter get rid of the rest of the numbers
	mov r16, r0 ;move generated number to register 16 
	rjump Generate

done:
	rjump done