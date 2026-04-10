    #include <p18f4550.inc>

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    #define VA      0x3F    ; Valor de VA
    #define VB      0x03    ; Valor de VB

    #define FA      0x00    ; Endereço de VA
    #define FB      0x01    ; Endereço de VB
    #define FQ      0x02    ; Endereço do Resultado (Quociente)
    #define FR      0x03    ; Endereço do Resto da divisão

    org 0x0000
    bra    start
    org 0x0020

start:
    MOVLW VA        ; Carregar VA
    MOVWF FA, 0

    MOVWF FR, 0     ; Carregar FR com Valor de VA
    CLRF  FQ        ; Limpa Resultado

    MOVLW VB        ; Carregar VB
    MOVWF FB, 0

    MOVF  FA, W     ; Verifica se é divisão do tipo: 0/X = 0
    BZ out
    MOVF  FB, W     ; Verifica se é divisão do tipo: X/0 = indf
    BZ  out

    CPFSLT  FR      ; Verifica se FR < VB (fração própria)
    bra sub         ; SIM ? Então começa a operar
    bra out         ; NÃO ? Então vai pra parada

; Loop de subtrações
sub:
    INCF FQ, F      ; Incrementa Quociente para cada VB removida de FR
    SUBWF FR, F     ; Subtrai VB de FR e armazena em FR
    CPFSLT  FR      ; Skipa (vai para parada) se FR < VB
    bra sub
out:
    bra out
    end