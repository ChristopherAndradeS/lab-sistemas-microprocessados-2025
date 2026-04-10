#include <xc.h>

#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 20000000  

#define LCD_DATA   LATD
#define LCD_TRIS   TRISD
#define RS         LATEbits.LATE0
#define EN         LATEbits.LATE1
#define RS_TRIS    TRISEbits.TRISE0
#define EN_TRIS    TRISEbits.TRISE1

void delay_lcd(void)
{
    int i;
    for(i = 0; i < 1000; i++);
}

void lcd_cmd(unsigned char cmd)
{
    LCD_DATA = cmd;   
    RS = 0;          
    EN = 1;          
    delay_lcd();
    EN = 0;          
}

void lcd_char(unsigned char data)
{
    LCD_DATA = data;  
    RS = 1;           
    EN = 1;
    delay_lcd();
    EN = 0;
}

void lcd_string(const char *str)
{
    while(*str)
    {
        lcd_char(*str++);
    }
}

void lcd_init(void)
{
    LCD_TRIS = 0x00;
    RS_TRIS = 0;
    EN_TRIS = 0;
    TRISEbits.TRISE2 = 0;
    LATEbits.LATE2 = 0;

    delay_lcd();

    lcd_cmd(0x38);  
    lcd_cmd(0x0C);  
    lcd_cmd(0x06);  
    lcd_cmd(0x01);  
}

void main(void)
{
    lcd_init();

    lcd_cmd(0x80);
    lcd_string("    CEFET-MG    ");
    lcd_cmd(0xC0);
    lcd_string("  Lab. de micro ");

    while(1);
}