    #include <p18f4550.inc>

    CONFIG CPUDIV = OSC1_PLL2
    CONFIG FOSC = HS
    CONFIG WDT = OFF
    CONFIG PBADEN = OFF
    CONFIG LVP = OFF
    CONFIG MCLRE = ON
    CONFIG PWRT = ON

    #define FA       0x00   ; Endereço de A
    #define FB       0x01   ; Endereço de B
    #define FQ       0x02   ; Endereço de Q(uociente)
    #define FR       0x03   ; Endereço de R(esto)
    #define COUNT    0x04   ; Endereço do contador
    #define SIGNAL_Q 0x05   ; Endereço do sinal final de Q
    #define SIGNAL_R 0x06   ; Endereço do sinal final de R

    #define VA     0x06     ; Valor de A (-20)
    #define VB     0x00     ; Valor de B (6)

    org 0x0000
    bra start

    org 0x0020
start:
    MOVLW   VA        ; Inicializando variáveis
    MOVWF   FA, 0
    MOVLW   VB
    MOVWF   FB, 0
    CLRF    FQ
    CLRF    FR

    MOVF  FA, W         ; Verifica se é divisão do tipo: 0/X = 0
    BZ out
    MOVF  FB, W         ; Verifica se é divisão do tipo: X/0 = indf
    BZ  out

    MOVLW   0x08        ; Inicializando variáveis auxiliares
    MOVWF   COUNT
    CLRF    SIGNAL_Q
    CLRF    SIGNAL_R

            ; Verificar se FA < 0, se sim, FA <- |VA|
    MOVF    FA, W
    BNN     next_step
    COMF    FA, F
    INCF    FA, F
    BSF     SIGNAL_R, 7     ; Sinal de R será FA(bit7)
    BSF     SIGNAL_Q, 7
next_step:  ; Verificar se FB < 0, se sim, FB <- |VB|
    MOVF    FB, W
    BNN     end_step
    COMF    FB, F
    INCF    FB, F
end_step:
    ANDLW   0x80
    XORWF   SIGNAL_Q, F    ; Sinal de Q será FA[bit7] XOR FB[bit7]
    MOVFF   FA, FQ
div_loop:
    RLCF    FQ, F          ; Q << 1, carry <- MSB(Q)
    RLCF    FR, F          ; R << 1, LSB(R) <- carry

    BTFSS   FR, 7          ; R < 0 -> R = R + VB
    bra     r_positive
   
    MOVF    FB, W
    ADDWF   FR, F
    bra     set_qbit
r_positive:                 ; R >= 0 -> R = R - VB
    MOVF    FB, W
    SUBWF   FR, F
set_qbit:                   ; Se R >= 0 -> Q[bit0] = 1, else Q[bit0] = 0
    BTFSS FR, 7
    BSF FQ, 0
    BTFSC FR, 7
    BCF FQ, 0
    DECFSZ COUNT, F         ; Condição de parada (COUNT == 0)
    bra div_loop

    BTFSS FR, 7             ; correção final: se R < 0, R = R + VB
    bra  insert_signal_fQ
    MOVF FB, W
    ADDWF FR, F
insert_signal_fQ:           ; aplicar sinal do quociente
    BTFSS SIGNAL_Q, 7
    bra insert_signal_fR
    COMF FQ, F
    INCF FQ, F
insert_signal_fR:           ; aplicar sinal do resto
    BTFSS SIGNAL_R, 7
    bra out
    COMF FR, F
    INCF FR, F
out:
    bra out
    end