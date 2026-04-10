;ex01.asm
   #include <p18f4550.inc>

   org 0x0000
   bra    start
   org 0x0020

start:
    MOVLW 0x5A          ; O literal é movido para acumulador
    MOVWF 0x00, 0       ; O conteúdo do acumulador é copiado para posição 0x000
    MOVFF 0x000, 0x3AA  ; O conteúdo do endereço 000 é copiado para 3AA
    MOVLB 5             ; O BSR é carregado em 5, pois utilizar esse banco na próxima instrução
    MOVWF 0xAA, 1       ; O conteúdo do acumulador é movido para o endereço 5AA
    
    bra start
    end
