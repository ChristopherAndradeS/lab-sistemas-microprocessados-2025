    #include <p18f4550.inc>

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    org 0x0000
    bra    start
    org 0x0020

start:
    LFSR FSR0, 0x300        ; Carrega FSR0 para banco 3
    LFSR FSR1, 0x400        ; Carrega FSR1 para banco 4
    ; Essa parte define os valores de inicio do vetor
    CLRF INDF0              ; Limpa 0x300
    MOVFF INDF0, INDF1      ; Copia 0x300 para 0x400
; loop da primeira parte (preencher banco 3)
loop_1:
    MOVFF FSR0L, PREINC0    ; i + 1 recebe seu próprio indíce
    MOVF FSR0L, W, 0        ; WREG recebe o índice atual (Atualizar Flag)
    BNZ  loop_1             ; Repita até Z = 0
    LFSR FSR0, 0x300        ; Limpa FSR0
; loop da segunda parte (preencher banco 4 com banco 3)
loop_2:
    MOVFF PREINC0, PREINC1  ; Move (i + 1) Banco 3 para (i + 1) Banco 4
    MOVF FSR1L, W, 0        ; WREG recebe o índice atual (Atualizar Flag)
    BNZ  loop_2
; parada
out:
    bra out
    end