#define LCD_LIBONLY
.include "lcd.asm"

.equ A = 13
.equ C = 17
.equ SEED = 19

.cseg
	;initialize lights
	ldi r22,0XFF
	ldi r23,0XFF
	out DDRB, r22
	sts DDRL, r23
    ;initialization (comment this code because it conflicts with the driver of LCD,
	;turning off the background light of LCD)
    ;ldi r16, 0b00001010
    ;out DDRB, r16
	;ldi r16, 0b10101010
    ;sts DDRL, r16h

	call lcd_init

    ldi r16, SEED

repeat:

    rcall rand				;generate random value
	rcall led				;illuminate LEDs
	rcall itoa_binary		;show binary string
	rcall itoa_decimal		;show decimal string
	rcall show_binary_str	;show binary string
	rcall show_decimal_str	;show decimal string
    rcall delay				;delay 1 second
	
    rjmp repeat

;description: ; generate a random number in range 0~63
;input: R16 - seed or previous random number
;output:R16 - new random number 
rand:
	;generate random num
	clr r0
	clr r18
	clr r17
	ldi r17, A ;load A
	ldi r18, C ;load C	
	mul r16, r17 ;multiply A and Seed
	mov r16, r0 ;move to reg 27
	add r16, r18 ;add constant
	andi r16, 0b00111111 ;modulus 64 which is 2^6 so only first 6 digits matter get rid of the rest of the numbers

	ret

;description: ; illuminate LEDs
;input: R16 - controlling value
;output:none
led:
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

	ret


;description: convert a value to binary string
;input: R16 - the value
;output: "bstr" in data memory
itoa_binary:
	;get binary value
	ldi XL, low(bstr)
	ldi XH, high(bstr)
	ldi r17, '0'
	ldi r18, '1'
	sbrc r16,5
	st x+, r18
	sbrs r16,5
	st x+, r17
	sbrc r16,4
	st x+, r18
	sbrs r16,4
	st x+, r17
	sbrc r16,3
	st x+, r18
	sbrs r16,3
	st x+, r17
	sbrc r16,2
	st x+, r18
	sbrs r16,2
	st x+, r17
	sbrc r16,1
	st x+, r18
	sbrs r16,1
	st x+, r17
	sbrc r16,0
	st x+, r18
	sbrs r16,0
	st x+, r17
	ldi r17, '\0'
	st x+, r17

	ret

;description: convert a value to decimal string
;input: R16 - the value
;output: "dstr" in data memory
itoa_decimal:
	ldi ZL, low(dsstr)
	ldi ZH, high(dsstr)
	push r19
	push r17
	push r18
	clr r18
	ldi r17, '0'
	mov r19, r16
	mod_10:
		;subtract 10 till lower than 10
		cpi r19, 10
		brlo store
		division:
			inc r18
			subi r19, 10
			cpi r19, 10
			brsh division
		;change unsigned integer to character integer
		add r18, r17
		st z+, r18
		clr r18
		jmp mod_10
		
		store:		
			;get digit of last one
			add r19, r17
			st z+, r19

	ldi r18, '\0'
	st z+, r18
	pop	r18
	pop r17
	pop r19
	ret

;description: show binary on LCD at the 2st row (right aligned; 6 bits)
;input: string in data memory with label "bstr"
;output: none
show_binary_str:
	
	push r16
	call lcd_clr
	ldi r16, 0x01
	push r16
	ldi r16, 0x0A
	push r16
	call lcd_gotoxy
	pop r16
	pop r16

	ldi r16, high(bstr)
	push r16
	ldi r16, low(bstr)
	push r16
	call lcd_puts
	pop r16
	pop r16
	
	pop r16

	ret

;description: show decimal on LCD at the 1st row (left aligned; two digits)
;input: string in data memory with label "dstr"
;output: none
show_decimal_str:
	push r16
	ldi r16, 0x00
	push r16
	ldi r16, 0x00
	push r16
	call lcd_gotoxy
	pop r16
	pop r16

	ldi r16, high(dsstr)
	push r16
	ldi r16, low(dsstr)
	push r16
	call lcd_puts
	pop r16
	pop r16
	
	pop r16
	ret


;description: delay for some time
;input: none
;output: none
delay:
	push r16
	ldi r16, 0
loop_delay:
	call dly_ms
	inc r16
	cpi r16,250
	brlo loop_delay
	pop r16
	ret

.dseg

	bstr: .byte 100	;for temporarily storing string (for binary display)
	dsstr: .byte 100	;for temporarily storing string (for decimal display)
