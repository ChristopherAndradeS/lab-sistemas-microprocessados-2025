    #include <p18f4550.inc>

    #define V0 0            ; Fibonnaci(0)
    #define V1 1            ; Fibonnaci(2)
    ; Endereço 0x00 -> Fibonnaci(N - 2)
    ; Endereço 0x01 -> Fibonnaci(N - 1)
    #define COUNTER 0x02    ; Endereço do contador (0 -> 13)

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    org 0x0000
    bra    start
    org 0x020

start:
    MOVLB 0x03          ; Seleção do Banco 3
    MOVLW (0x0D - 2)    ; Inicializando COUNTER COM 13
    MOVWF COUNTER, 0
    LFSR FSR0, 0x300    ; Inicializando FSR0
    MOVLW V0            ; Carregando V0
    MOVWF POSTINC0
    MOVWF 0x00, 0       ; Inicializando F(n - 2)
    MOVLW V1            ; Carregando V1
    MOVWF POSTINC0
    MOVWF 0x01, 0       ; Inicializando F(n - 1)
repeat:
    MOVF 0x00, W        ; W <- F(n - 2)
    ADDWF 0x01, W, 0    ; W <- F(n - 2) + F(n - 1)

    MOVFF 0x001, 0x000  ; F(n - 2) <- F(n - 1)
    MOVWF 0x01, 0       ; F(n - 1) <- W
    MOVWF POSTINC0      ; Atualizando Sequência no banco

    DECF COUNTER        ; Decrementa COUNTER até zerar
    BNZ repeat
out:
    bra out
    end