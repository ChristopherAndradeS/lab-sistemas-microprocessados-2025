#include <xc.h>

#pragma config CPUDIV       = OSC1_PLL2
#pragma config FOSC         = HS
#pragma config WDT          = OFF
#pragma config PBADEN       = OFF
#pragma config LVP          = OFF
#pragma config MCLRE        = ON
#pragma config PWRT         = ON
#pragma config XINST        = OFF

#define _XTAL_FREQ 20000000UL

#define T2_PRESCALE_1   1
#define T2_PRESCALE_4   4
#define T2_PRESCALE_16  16

#define ON              (1)
#define OFF             (0)

#define GATE_CLOSED    (1)
#define GATE_IN_MIDDLE (0)
#define GATE_OPENED    (-1)

#define GATE_STOPED    (0)
#define GATE_CLOSING   (1)
#define GATE_OPENING   (2)

#define REASON_LIMIT_SWITCH (0)
#define REASON_PRESENCE     (1)
#define REASON_EMERGENCY    (2)
#define REASON_BY_USER      (3)

static unsigned char calc_PR2(float freq, unsigned char *ps_val, unsigned char *t2con_bits);
void tune(unsigned char channel, float freq);
void noTone(void);
void delay_s(unsigned int seconds);
void delay_ms(unsigned int milliseconds);
void Play_Alarm_Gate();
void Play_Alarm_Emergency();
void handle_INT0(void);
void handle_INT1(void);
void handle_INT2(void);

int     rise_fall_time;
int     steps;
int     f_max;
int     f_min;
float   delay_time;
float   step_size;

unsigned char  gate_pattern        = 0xFF;
signed char    gate_static_state   = GATE_CLOSED;
signed char    gate_dinamic_state  = GATE_STOPED;
signed char    last_dinamic_state  = GATE_CLOSING;
unsigned char  stop_reason         = REASON_LIMIT_SWITCH;

unsigned char flag_INT0 = 0;
unsigned char flag_INT1 = 0;
unsigned char flag_INT2 = 0;

unsigned char alarm_gate_enable = 0;
unsigned char is_presence       = 0;
unsigned char is_emergency      = 0;


void interrupt isr()
{
    if(INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = OFF;
        TMR0  = 65536 - (_XTAL_FREQ / 512);

        LATEbits.LATE0 = !LATEbits.LATE0;
    }

    if(INTCONbits.INT0IF)
    {
        INTCONbits.INT0IF = OFF;
        flag_INT0 = 1;
    }

    if(INTCON3bits.INT1IF)
    {
        INTCON3bits.INT1IF = OFF;
        flag_INT1 = 1;
    }

    if(INTCON3bits.INT2IF)
    {
        INTCON3bits.INT2IF = OFF;
        flag_INT2 = 1;
    }
}

void main(void)
{
    INTCON  = 0xF0;
    INTCON2 = 0xF0;
    INTCON3 = 0x18;

    T0CON  = 0x06;
    TMR0  = 65536 - (_XTAL_FREQ / 512) ;

    TRISD = 0x00;
    TRISB = 0xFF;
    TRISC = 0x00;

    TRISEbits.RE0 = 0;
    TRISAbits.RA5 = 0;
    LATAbits.LATA5 = 1;

    LATD  = ~gate_pattern;

    while(1)
    {
        if(flag_INT2)
        {
            __delay_ms(5);
            handle_INT2();
            flag_INT2 = 0;
        }

        Play_Alarm_Emergency();
        Play_Alarm_Gate();

        if(flag_INT0)
        {
            __delay_ms(5);
            handle_INT0();
            flag_INT0 = 0;
        }

        if(flag_INT1)
        {
            __delay_ms(5);
            handle_INT1();
            flag_INT1 = 0;
        }

        if(gate_dinamic_state == GATE_CLOSING)
        {
            if(gate_pattern != 0xFF)
            {
                gate_pattern = (gate_pattern >> 1) | 0x80;
                alarm_gate_enable = ~alarm_gate_enable;
                LATD = ~gate_pattern;
                for (unsigned int i = 0; i < 750; i++)
                {
                    if (gate_dinamic_state == GATE_STOPED) break;
                    __delay_ms(1);
                }
            }
           else
            {
                gate_static_state  = GATE_CLOSED;
                last_dinamic_state = GATE_CLOSING;
                gate_dinamic_state = GATE_STOPED;
                alarm_gate_enable  = 0;
                stop_reason        = REASON_LIMIT_SWITCH;
            }
        }

        else if(gate_dinamic_state == GATE_OPENING)
        {
            if(gate_pattern != 0x80)
            {
                gate_pattern <<= 1;
                LATD = ~gate_pattern;
                alarm_gate_enable = ~alarm_gate_enable;
                for (unsigned int i = 0; i < 750; i++)
                {
                    if (gate_dinamic_state == GATE_STOPED) break;
                    __delay_ms(1);
                }
            }
            else
            {
                gate_static_state  = GATE_OPENED;
                last_dinamic_state = GATE_OPENING;
                gate_dinamic_state = GATE_STOPED;
                alarm_gate_enable  = 0;
                stop_reason        = REASON_LIMIT_SWITCH;
            }
        }

        else
        {
            gate_static_state = GATE_IN_MIDDLE;
        }
    }

    return;
}

/* Se o sensor de presença ativar */
void handle_INT0(void)
{
    if(is_emergency)
        return;

    if(is_presence)
    {
       is_presence = 0;
       LATCbits.LATC1 = 0;
    }

    else
    {
        is_presence = 1;
        LATCbits.LATC1 = 1;

        if(gate_dinamic_state != 0)
        {
           last_dinamic_state = (gate_dinamic_state == GATE_OPENING) ? GATE_CLOSING : GATE_OPENING;
        }

        stop_reason        = REASON_PRESENCE;
        gate_dinamic_state = GATE_STOPED;
        alarm_gate_enable = 0;
    }
}

/* Botão apertado pelo usuário */
void handle_INT1(void)
{
    if(is_emergency || is_presence)
        return;

    LATCbits.LATC6 = !LATCbits.LATC6;

    if(gate_dinamic_state == GATE_CLOSING || gate_dinamic_state == GATE_OPENING)
    {
        alarm_gate_enable  = 0;
        last_dinamic_state = gate_dinamic_state;
        gate_dinamic_state = GATE_STOPED;
        stop_reason = REASON_BY_USER;
    }

    else if(gate_dinamic_state == GATE_STOPED)
    {
        if(last_dinamic_state == GATE_CLOSING)
        {
            gate_dinamic_state = GATE_OPENING;
        }

        else if(last_dinamic_state == GATE_OPENING)
        {
            gate_dinamic_state = GATE_CLOSING;
        }
        last_dinamic_state = GATE_STOPED;
    }
}

/* Se o botão de emergência for acionado */
void handle_INT2(void)
{
    if(is_emergency)
    {
       is_emergency = 0;
       LATEbits.LATE0 = 0;
       LATAbits.LATA5 = 1;
       T0CONbits.TMR0ON = 0;
    }

    else
    {
        is_emergency = 1;
        LATEbits.LATE0 = 1;
        LATAbits.LATA5 = 0;
        T0CONbits.TMR0ON = 1;

        if(gate_dinamic_state != 0)
        {
           last_dinamic_state = (gate_dinamic_state == GATE_OPENING) ? GATE_CLOSING : GATE_OPENING;
        }

        stop_reason        = REASON_EMERGENCY;
        gate_dinamic_state = GATE_STOPED;
        alarm_gate_enable = 0;
    }
}

static unsigned char calc_PR2(float freq, unsigned char *ps_val, unsigned char *t2con_bits)
{
    unsigned long pr2;

    *ps_val = T2_PRESCALE_1;
    *t2con_bits = 0b00000000;
    pr2 = (_XTAL_FREQ / (4UL * (unsigned int) freq * (*ps_val))) - 1UL;
    if (pr2 <= 255UL) return (unsigned char)pr2;

    *ps_val = T2_PRESCALE_4;
    *t2con_bits = 0b00000001;
    pr2 = (_XTAL_FREQ / (4UL * (unsigned int) freq * (*ps_val))) - 1UL;
    if (pr2 <= 255UL) return (unsigned char)pr2;

    *ps_val = T2_PRESCALE_16;
    *t2con_bits = 0b00000010;
    pr2 = (_XTAL_FREQ / (4UL * (unsigned int) freq * (*ps_val))) - 1UL;
    if (pr2 <= 255UL) return (unsigned char)pr2;

    return 255;
}

void tune(unsigned char channel, float freq)
{
    unsigned char ps_val, t2bits;
    unsigned char pr2;
    unsigned int duty_10bit;

    pr2 = calc_PR2(freq, &ps_val, &t2bits);

    PR2 = pr2;

    duty_10bit = (unsigned int)(pr2 + 1) * 2;

    if (channel == 1)
    {
        TRISCbits.TRISC2 = 0;
        CCPR1L = (unsigned char)(duty_10bit >> 2);
        CCP1CONbits.DC1B = (unsigned char)(duty_10bit & 0x03);
        CCP1CONbits.CCP1M = 0b1100;
    }

    else if (channel == 2)
    {
        TRISCbits.TRISC1 = 0;
        CCPR2L = (unsigned char)(duty_10bit >> 2);
        CCP2CONbits.DC2B = (unsigned char)(duty_10bit & 0x03);
        CCP2CONbits.CCP2M = 0b1100;
    }
    else
        return;

    TMR2 = 0;
    T2CON = (T2CON & 0b11111000) | (t2bits & 0x03);
    T2CONbits.TMR2ON = 1;
}

void noTone(void)
{
    CCP2CON = 0x00;             // Desabilitar CCP2
    T2CONbits.TMR2ON = 0;       // Parar Timer2
    CCPR2L = 0;                 // Duty cycle 0
}

void delay_ms(unsigned int milliseconds)
{
    while(milliseconds > 0)
    {
        __delay_ms(1);
        milliseconds--;
    }
}

void Play_Alarm_Gate()
{
    if(alarm_gate_enable)
        tune(1, 1200);
    else
        noTone();
}

void Play_Alarm_Emergency()
{
    if(!is_emergency)
        return;

    for(int i = 0; i < 10; i++)
    {
        if(!is_emergency)
            break;

        for(long float f = 3000; f > 300; f = f * 0.85)
        {
            if(!is_emergency)
                break;
            tune(1, f);
            __delay_ms(15);
        }
    }

    if(!is_emergency)
        return;

    noTone();
    __delay_ms(10);

    for(int i = 0; i < 10; i++)
    {
        if(!is_emergency)
           break;
        for (float f = 3000; f > 100; f = f * 0.85)
        {
            if(!is_emergency)
                break;

            tune(1, 2 * f);
            __delay_ms(5);
            tune(1, f);
            __delay_ms(5);
        }
    }

    if(!is_emergency)
        return;

    noTone();
    __delay_ms(10);

    rise_fall_time = 90;
    steps = 10;
    f_max = 1000;
    f_min = 500;
    delay_time = rise_fall_time/steps;
    step_size = 0.93;

    for(int i = 0; i < 20; i++)
    {
        if(!is_emergency)
           break;
        for (float f = f_max;f > f_min; f *= step_size)
        {
            if(!is_emergency)
                break;

            tune(1, f);
            delay_ms((unsigned int) delay_time);
        }
    }

    if(!is_emergency)
        return;

    noTone();
    __delay_ms(10);

    rise_fall_time = 1000;
    steps = 100;
    f_max = 1500;
    f_min = 500;
    delay_time = rise_fall_time/steps;
    step_size = 1.05;

    for(int i = 0; i < 10; i++)
    {
        if(!is_emergency)
            return;
        for (float f =f_min;f<f_max;f*=step_size)
        {
            if(!is_emergency)
                return;
            tune(1, f);
            delay_ms((unsigned int) delay_time);
        }
    }

    if(!is_emergency)
        return;

    noTone();
    __delay_ms(10);
}