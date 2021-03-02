#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lcd.h>

long int timer1 = 3*3600;
long int timer2 = 60*60;

static int timer1_running = 0; // Global bool to check if timer 1 is on.
static int timer2_running = 0; // Global bool to check if timer 2 is on.

void buttons_init(void) {
    /*Set pins to input*/
    DDRB = ~_BV(DDB5);

    PORTB |= _BV(PORTB5);

    PCICR |= _BV(PCIE0);
    PCMSK0 |= _BV(PCINT4);

    sei(); // Interrupts on
}


char * seconds_to_time(int seconds){
    static char buf[16];

    int h = seconds / 3600; // hours
    int m = (seconds - (3600*h))/60; // minutes
    int s = (seconds - (3600*h)) - (m*60); // seconds
    if (m < 10) {
        if(s < 10) {
        sprintf(buf, "%d:0%d:0%d", h,m, s);
        }
        else{
        sprintf(buf, "%d:0%d:%d", h,m, s);
        }
    }
    else if( s < 10 ) {
        sprintf(buf, "%d:%d:0%d", h,m, s);
    }
    else {
        sprintf(buf, "%d:%d:%d", h,m, s);
    }

    return buf;
}

ISR (PCINT0_vect) {
    timer1_running = 1;
    timer2_running = 1;
    timer1 = 3 * 3600 + 1;
    timer2 = 60 * 60 + 1;
}

int main(void) {
    buttons_init(); // Initialize button interrupts
    LCD_Init();
    while(1) {
        LCD_Action(0x80);
        LCD_Print("Timer1: ");
        LCD_Print(seconds_to_time(timer1));
        LCD_Action(0xC0); // Go to Line 2, Position 1
        LCD_Print("Timer2: ");
        LCD_Print(seconds_to_time(timer2));
        _delay_ms(1000);
        if (timer1_running) {
          timer1--;
        }
        if (timer2_running) {
          timer2--;
        }
    } 
}
