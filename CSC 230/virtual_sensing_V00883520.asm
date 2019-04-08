#define LCD_LIBONLY
; written by Amaan Makhani
;define variables
.equ ADC_BTN_SELECT = 0x316
.equ TOP = 6249
.equ A = 13
.equ C = 17
.equ SEED = 19
.cseg
rjmp start
;start of interrupt vector table (IVT)
.org 0x0000                           ; interrupt vector 1: reset
jmp reset
.org 0x0024
jmp isr_timer1_cmpb
.org 0x003A                           ; interrupt vector 30: ADC Conversion Complete
jmp isr_adc

.org 0x0074                           ; start of user code

reset:
	;timer setup
    ldi r16, 0b00100000                 ; clear register OC1B on Compare Match
    sts TCCR1A, r16
    ldi r16, 0x05                       ; timer clock = system clock/256
    sts TCCR1B, r16
    ldi r16, 0b00000000
    sts TCCR1C, r16
	ldi r16, 0x04                       ;Output Compare B Interrupt
    sts TIMSK1, r16
    ldi r16, high(TOP)                  ;TOP Value
    sts OCR1BH, r16
    ldi r16, low(TOP)
    sts OCR1BL, r16

    ;init ADC for Buttons
    ldi r16, 1<<REFS0 ; 0x40  REFS=01: using external AVcc, ADLAR=0: right adjustment, MUX=00000: single end input from ADC0
    sts ADMUX, r16
    ldi r16, 0x05        ; use Timer/Counter 1 Compare Match B to trigger ADCconversion=
    sts ADCSRB, r16
    ldi r16, 0xAF ; ADEN=1: enable ADC; ADATE=1:auto-trigger, ADIE=1:enable interrupt; ADPS=111: prescalar as 128
    sts ADCSRA, r16
    ldi r16, 0xFE
    sts DIDR0, r16 ; disable pins ADC1:7, to reduce power consumption
    ser r16
    sts DIDR2, r16 ; disable pins ADC8:15, to reduce power consumption
	
	sei
	jmp start

isr_timer1_cmpb:
	reti

isr_adc:
    push XL
    push XH
    push r16
    ldi r24, low(ADC_BTN_SELECT)
    ldi r25, high(ADC_BTN_SELECT)
    lds XL, ADCL
    lds XH, ADCH
    cp XL, r24
    cpc XH, r25

    brlo btn_pressed
    rjmp isr_adc_c_ret
 
btn_pressed:
	inc r23
	andi r23, 0b00000001 	;modulus 2

isr_adc_c_ret:
	pop r16
	rcall rand				;generate random value
	rcall led				;illuminate LEDs

	;determine what case to choose
	sbrs r23, 0
	rcall convert_signed
	sbrc r23, 0
	rcall convert_unsigned	;convert both to signed and unsigned

	rcall display_h_or_t	;display temp or humidity string

	pop XH
	pop XL
	reti

start:
	.include "lcd.asm"
	.cseg
	;initialize seed
	ldi r16, SEED

	;display vnum
	call lcd_init
	rcall display_vnum

	;intialize things needed
	ldi r24, low(ADC_BTN_SELECT)
	ldi r25, high(ADC_BTN_SELECT)
	ldi r23, 0

done:
   rjmp done

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
	
;description: ; illuminate Luk  EDs
;input: R16 - controlling value
;output:none
led:
	;illuminate LEDs according to value in R16
	push r23
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
	pop r23
	ret
	
;if 0
zero:
	ldi r18, ' '
	st z+, r18
	st z+, r18
	ldi r18, '0'
	st z+, r18
	ldi r18, 'C'
	st z+, r18
	ldi r18, ' '
	st z+, r18
	st z+, r18
	ldi r18, '\0'
	pop	r18
	pop r17
	pop r19
	ret
	
;description: ; convert to temp
;input:	r16 rand number
;output:string in data mem
convert_signed:
	ldi ZL, low(data_storage)
	ldi ZH, high(data_storage)
	push r19
	push r17
	push r18
	ldi r18, 'T'
	st z+, r18
	ldi r18, ':'
	st z+, r18

	cpi r16, 0b00100000
	breq zero
	
	ldi r18, '-'
	sbrc r16, 5
	st z+, r18
	ldi r18, ' '
	sbrs r16, 5
	st z+, r18
	
	clr r18
	ldi r17, '0'
	mov r19, r16
	mod_10_2:
		;subtract 10 till lower than 10
		cpi r19, 10
		brlo store_2
		division_2:
			inc r18
			subi r19, 10
			cpi r19, 10
			brsh division_2
		;change unsigned integer to character integer
		add r18, r17
		st z+, r18
		clr r18
		jmp mod_10_2
		
		store_2:		
			;get digit of last one
			add r19, r17
			st z+, r19

	ldi r18, 'C'
	st z+, r18
	ldi r18, ' '
	st z+, r18
	st z+, r18
	ldi r18, '\0'
	st z+, r18
	pop	r18
	pop r17
	pop r19
	ret
	
;description: ; convert to humidity
;input:	r16 rand number
;output:string in data mem
convert_unsigned:
	ldi ZL, low(data_storage)
	ldi ZH, high(data_storage)
	push r19
	push r17
	push r18
	ldi r18, 'H'
	st z+, r18
	ldi r18, ':'
	st z+, r18
	ldi r18, ' '
	st z+, r18
	
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

		ldi r18, '%'
	st z+, r18
	ldi r18, ' '
	st z+, r18
	st z+, r18
	ldi r18, '\0'
	st z+, r18
	pop	r18
	pop r17
	pop r19
	ret

;description: ; display humidity or temp
;input: data mem string
;output:temp of vnum to lcd screen 
display_h_or_t:
	push r16
	;Move pointer to top line and 0 in 
	ldi r16, 0x00
	push r16
	ldi r16, 0x00
	push r16
	call lcd_gotoxy
	pop r16
	pop r16
	
	;Now display
	ldi r16, high(data_storage)
	push r16
	ldi r16, low(data_storage)
	push r16
	call lcd_puts
	pop r16
	pop r16
	
	;done
	pop r16
	ret

;description: ; display vnum
;input: data mem string
;output:vnum to lcd screen 
display_vnum:
	push r16
	; copy strings from program memory to data memory
	ldi r16, high(vnum_storage)	; address of the destination string in data memory
	push r16
	ldi r16, low(vnum_storage)
	push r16
	ldi r16, high(v_num << 1) ; address the source string in program memory
	push r16
	ldi r16, low(v_num << 1)
	push r16
	call str_init			; copy from program to data
	pop r16					; remove the parameters from the stack
	pop r16
	pop r16
	pop r16
	call lcd_clr
	
	;Move pointer to bottom line and 10 in 
	ldi r16, 0x01
	push r16
	ldi r16, 0x0A
	push r16
	call lcd_gotoxy
	pop r16
	pop r16
	
	; Now display vnum on bottom corner
	ldi r16, high(vnum_storage)
	push r16
	ldi r16, low(vnum_storage)
	push r16
	call lcd_puts
	pop r16
	pop r16
	
	;done
	pop r16
	ret
	
;Display v number	
v_num:	.db "V-3520", 0	

.dseg
; The program copies the strings from program memory
; into data memory.  

vnum_storage:	.byte 200
data_storage:	.byte 200
