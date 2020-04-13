;The MSB is used to represent the sign (with 0 indicating a non-negative number and 1 indicating a negative number)
;and the remaining bits correspond to an unsigned value.
;Transform a signed magnitude and convert it into zero-ending string.

.equ COUNT = 4
.cseg
.org 0
	rjmp start
	nums: .db 0x12, 0x45, 0x89, 0xCD

start:
	;Create alias for registers
	.def dividend=r0
	.def divisor=r1
	.def quotient=r2
	.def temp=r21
	.def Char0=r3
	;Set up program memory
 	ldi ZL, low(nums<<1)
	ldi ZH, high(nums<<1) 
	ldi temp, '0'
	mov Char0, temp
	ldi r17, 0
	
loop:
	clr r16
	lpm r16, Z+
	rcall itoa
	inc r17
	cpi r17, COUNT
	brlo loop

done:
	rjmp done


;description: convert a signed magnitude value into 0-ending string
;input: R16 - the value
;       X   - starting address of the string
;output:none   
itoa:
	;If negative load neg and flip msb
	ldi XL, low(str)
	ldi XH, high(str)
	ldi r18, '-'
	sbrc r16, 7
	st x+, r18
	ldi r18, '+'
	sbrs r16, 7
	st x+, r18
	ldi r18, 0b01111111
	and r16, r18
	clr temp
	
	;Load dividend and divisor
	mov dividend, r16
	ldi temp, 10
	mov divisor, temp
	
	mod_10:
		;Divide by ten until less than 10
		clr quotient
		cp dividend, divisor
		brlo store
		division:
			inc quotient
			sub dividend, divisor
			cp dividend, divisor
			brsh division
		;Change unsigned integer to character integer and store it
		add quotient, Char0
		st X+, quotient
		clr quotient
		jmp mod_10
		
		store:	
			;Store last digit
			add dividend, Char0
			st x+, dividend
			ldi r25, '\0'
			st X+, r25
	
		ret
.dseg
.org 0x200
	str: .byte 100