    #include <p18f4550.inc>

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    #define len     5    ; Tamanho do vetor

    ; Valores de cada componente do vetor
    #define V0      0xCD
    #define V1      0x81
    #define V2      0x5E
    #define V3      0x3F
    #define V4      0x01

    ; Endereço de memória de cada componente
    #define F0      0x200
    #define F1      0x201
    #define F2      0x202
    #define F3      0x203
    #define F4      0x204

    ; Endereços de memória de trabalho
    #define ELEITO  0x00

    org 0x0000
    bra    start
    org 0x0020

start:
    CLRF ELEITO         ; Limpar Eleito
    LFSR FSR0, F0       ; FSR0 definido em P0
    MOVLW V0            ; Início: Carregar componentes do vetor
    MOVWF POSTINC0
    MOVLW V1
    MOVWF POSTINC0
    MOVLW V2
    MOVWF POSTINC0
    MOVLW V3
    MOVWF POSTINC0
    MOVLW V4
    MOVWF POSTINC0      ; Fim: Carregar componentes do vetor
    
    LFSR FSR0, F0       ; FSR0 redefinido em P0
    LFSR FSR1, F0       ; FSR1 definido em P1
init:
    MOVF FSR0L, W       ; W <- FRS0L
    SUBLW (len - 1)
    BZ  out             ; Condição de parada: FRS0L - 0x04 = 0
execution1:
    INCF FSR0L, F       ; P0 = P0 + 1
    MOVFF INDF0, ELEITO ; Eleito = Num[P0]
    MOVFF FSR0L, FSR1L  ; P1 = P0
execution2:
    DECF FSR1L, F       ; P1 = P1 - 1
condition1:
    BN init             ; P1 < 0 ?
    MOVF INDF1, W       ; W <- Num[P1]
    CPFSGT ELEITO       ; Skipa se ELEITO > Num[P1]
    bra execution3
    bra execution2
execution3:
    MOVLW 0x01          ; W = 1
    MOVFF INDF1, PLUSW1 ; Num[P1 + 1] = Num[P1]
    MOVFF ELEITO, INDF1 ; Num[P1] = Eleito
    bra execution2
; parada
out:
    bra out
    end