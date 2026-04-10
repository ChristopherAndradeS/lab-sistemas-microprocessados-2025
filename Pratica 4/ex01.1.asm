    #include <p18f4550.inc>

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    #define FA      0x00
    #define FB      0x01
    #define FC      0x02

    #define COUNT   0x03
    #define SIGN    0x04
    
    #define VA      0x0A
    #define VB      0x03

    org 0x0000
    bra start

    org 0x0020
start:
    MOVLW   VA        ; Carrega variáveis
    MOVWF   FA, 0
    MOVLW   VB
    MOVWF   FB, 0
    CLRF    FC
    ; Verifica se é multiplicação por 0
    MOVF    FA, W
    BZ      out
    MOVF    FB, W
    BZ      out
    ; Inicializa variáveis auxiliares
    CLRF    SIGN
    MOVLW   0x08
    MOVWF   COUNT

    MOVF    FA, W
    BNN     next_step   ; Verificar se FA < 0, se sim, FA <- |VA|
    COMF    FA, F
    INCF    FA, F
    BSF     SIGN, 7
next_step:
    MOVF    FB, W
    BNN     end_step    ; Verificar se FB < 0, se sim, FB <- |VB|
    COMF    FB, F
    INCF    FB, F
end_step:
    ANDLW 0x80
    XORWF SIGN, F       ; Sinal de FC será FA[bit7] XOR FB[bit7]
mul_loop:  
    BTFSS FB, 0         ; Se B[bit0] = 1, FC += FA
    bra shift
    MOVF FA, W
    ADDWF FC, F
shift:
    BCF STATUS, C
    RLCF FA, F          ; FA * 2
    RRCF FB, F          ; FB / 2
    DECFSZ COUNT, F     ; Condição de parada (COUNT == 0)
    bra mul_loop
    ; Ajuste de sinal do resultado
    BTFSS SIGN, 7
    bra out
    COMF FC, F
    INCF FC, F
out:
    bra out
    end