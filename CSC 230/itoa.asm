.equ COUNT = 4

.cseg
.org 0

	rjmp start

	nums: .db 0x12, 0x45, 0x89, 0xCD

start:
	;create alias for registers
	.def dividend=r0
	.def divisor=r1
	.def quotient=r2
	.def tempt=r21
	.def char0=r3
	;set up program memory
 	ldi ZL, low(nums<<1)
	ldi ZH, high(nums<<1)
	;set char0 and storing for string 
	ldi tempt, '0'
	mov char0, tempt
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
	;if negative load neg and flip msb
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
	clr tempt
	
	;load dividend and divisor
	mov dividend, r16
	ldi tempt, 10
	mov divisor, tempt
	
	mod_10:
		;divide by ten until less than 10
		clr quotient
		;see if less than 10 if so store it
		cp dividend, divisor
		brlo store
		division:
			inc quotient
			sub dividend, divisor
			cp dividend, divisor
			brsh division
		;change unsigned integer to character integer
		add quotient, char0
		;store leading digit
		st X+, quotient
		clr quotient
		jmp mod_10
		
		store:	
			;store last digit
			add dividend, char0
			st x+, dividend
			ldi r25, '\0'
			st X+, r25
	
		ret

.dseg

.org 0x200

	str: .byte 100
