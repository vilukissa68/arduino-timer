#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lcd.h>

long int runtime = 3*3600;


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



int main(void) {
    LCD_Init();
    LCD_Print("Hello!");
    while(1) {
        LCD_Action(0xC0);		//Go to Line 2, Position 1
        LCD_Print(seconds_to_time(runtime));
        _delay_ms(1000);
        runtime--;
    }
}
