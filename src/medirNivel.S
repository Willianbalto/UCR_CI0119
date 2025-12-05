.global medirNivelASM
.type medirNivelASM, @function

medirNivelASM:
    ; Parámetro canal en r24
    push r16
    push r17
    push r18
    push r19
    push r20
    push r21
    push r22          ; Agregar r22 para el contador
    
    ; vMax = 0 
    clr r18
    clr r19
    
    ; vMin = 1023 
    ldi r20, 0xFF
    ldi r21, 0x03
    
    ; contador = 150 (r22 en lugar de r17)
    ldi r22, 150

loop_adc:
    ; ADMUX = (ADMUX & 0xF0) | canal
    lds r16, 0x7C
    andi r16, 0xF0
    or r16, r24
    sts 0x7C, r16
    
    ; Iniciar conversión (ADSC=1, bit 6)
    lds r16, 0x7A
    ori r16, 0x40
    sts 0x7A, r16

wait_adc:
    lds r16, 0x7A
    sbrc r16, 6        ; Salta si ADSC=0 
    rjmp wait_adc
    
    ; Leer ADC  
    lds r16, 0x78      ; ADCL
    lds r17, 0x79      ; ADCH
    
    ; Comparar con vMax (si v > vMax, actualizar)
    cp r18, r16
    cpc r19, r17
    brsh no_update_max  ; Si vMax >= v, no actualizar
    mov r18, r16
    mov r19, r17
no_update_max:
    
    ; Comparar con vMin (si v < vMin)
    cp r16, r20
    cpc r17, r21
    brsh no_update_min  ; Si v >= vMin
    mov r20, r16
    mov r21, r17
no_update_min:
    
    dec r22             ; Decrementar contador 
    brne loop_adc
    
    ; Return = vMax - vMin
    mov r24, r18
    mov r25, r19
    sub r24, r20
    sbc r25, r21
    
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18
    pop r17
    pop r16
    ret