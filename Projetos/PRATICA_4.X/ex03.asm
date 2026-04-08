    #include <p18f4550.inc>
    
    #define VN          0x32    ; Número binário avaliado
    #define FN          0x00    ; Endereço do avaliado
    #define FMASK       0x01    ; Endereço da máscara de bits
    #define FCONTROL    0x02    ; Endereço da variável de controle
    #define FCOUNT      0x03    ; Endereço do contador

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
    MOVLW VN            ; Carregar avaliado
    MOVWF FN, 0
    CLRF FCONTROL       ; Limpar variavel de controle
    MOVLW 0x01
    MOVWF FMASK, 0      ; Inicializar Máscara como 00000001
    MOVLW 0x08          ; Contador inicializado em 8 (varredura de 8 bits)
    MOVWF FCOUNT, 0
repeat:
    MOVF FN, W          ; Filtra Bit do avaliado
    ANDWF FMASK, W
    BZ bit_pair
bit_odd:
    BTG FCONTROL, 0     ; Se bit é ímpar inverte o valor da variável de controle
bit_pair:
    RLNCF FMASK, F, 0   ; Para a próxima iteração a mascará roda
    DECF FCOUNT         ; Contador decrementa
    BNZ repeat          ; Continua até contador zerar
    MOVF FCONTROL, W
    BNZ set_bit         ; Se a variável de controle é 1 (Nº de bits ÍMPAR) Seta LSB
    BCF, 0              ; Se a variável de controle é 0 (Nº de bits PAR)   Reseta LSB
    bra out
set_bit:
    BSF, 0
out:
    bra out
    end