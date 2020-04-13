;Pick out even numbers in range [20, 40) from a set of given numbers deﬁned in program memory and put them into data memory.

.equ COUNT = 21

.cseg

.org 0

	src: .db 33, 60, 24, 55, 20, 44, 38, 90, 40, 0, 22, 80, 57, 43, 31, 100, 33, 27, 24, 18

ldi zl, low(src<<1)
ldi zh, high(src<<1)
ldi yl, low(dest)
ldi yh, high(dest)
ldi r18, 0

;Steps:
;Check if they are even 
;Check if they are within the range

load:
	lpm r16, z+
	inc r18 
	cpi r18, COUNT
	breq done
	
in_top_range:
	cpi r16, 40
	brlo in_bottom_range
	rjmp load

in_bottom_range:
	cpi r16, 20
	brsh is_even
	rjmp load

is_even:
	mov r17, r16
	andi r17, 0b00000001
	cpi r17, 0
	breq store
	rjmp load

store:
	st y+, r16
	rjmp load

done:
	rjmp done

.dseg

.org 0x200

	dest: .byte 20
