;Amaan Makhani
;V00883520

;parity 

.cseg 
.org 0

;number should be 257
ldi r16, 0X56      ;load number
ldi r19, 0XAB
add r16, r19

ldi r17, 0        ;hold a counter
ldi r18, 0        ;hold loop counter

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
