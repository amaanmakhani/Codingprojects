;Parity Flag is set to 1 when the number of bits in the calculation result in binary form is even. 
;Otherwise it is set to 0. Write a program to simulate such a ﬂag. 
.cseg 
.org 0

ldi r16, 0X56
ldi r19, 0XAB
add r16, r19

ldi r17, 0
ldi r18, 0

loop:
    inc r18
    sbrc r16, 0
    inc r17
    lsr r16
    cpi r18, 8
    brne loop

even:
    andi r17, 0b00000001
    cpi r17, 0
    brne false
    rjmp true

true:
    ldi r20, 1 ;set flag to 1
    mul r20, r20
    rjmp done

false:
    ldi r20, 0 ;set flag to 0
    mul r20, r20
    rjmp done

done:
    rjmp done
