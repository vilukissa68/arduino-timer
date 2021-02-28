#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RS PD2
#define EN PD3
#define LCD_DPin DDRD
#define LCD_Port PORTD

long int runtime = 3*3600;

void LCD_Action( unsigned char cmnd ) {
  LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);
  LCD_Port &= ~ (1<<RS);
  LCD_Port |= (1<<EN);
  _delay_us(1);
  LCD_Port &= ~ (1<<EN);
  _delay_us(200);
  LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);
  LCD_Port |= (1<<EN);
  _delay_us(1);
  LCD_Port &= ~ (1<<EN);
  _delay_ms(2);
}

void LCD_Init(void) {
    LCD_DPin = 0XFF;        // Enable data pins
    _delay_ms(15);          // Wait Activation
    LCD_Action(0x02);	      //4-Bit Control
    LCD_Action(0x28);       //Control Matrix @ 4-Bit
    LCD_Action(0x0c);       //Disable Cursor
    LCD_Action(0x06);       //Move Cursor
    LCD_Action(0x01);       //Clean LCD
    _delay_ms(2);
}

void LCD_Clear() {
    LCD_Action (0x01);		//Clear LCD
    _delay_ms(2);			//Wait to clean LCD
    LCD_Action (0x80);		//Move to Position Line 1, Position 1
}

void LCD_Print (char *str) {
	int i;
	for(i=0; str[i]!=0; i++)
      {
        LCD_Port = (LCD_Port & 0x0F) | (str[i] & 0xF0);
        LCD_Port |= (1<<RS);
        LCD_Port|= (1<<EN);
        _delay_us(1);
        LCD_Port &= ~ (1<<EN);
        _delay_us(200);
        LCD_Port = (LCD_Port & 0x0F) | (str[i] << 4);
        LCD_Port |= (1<<EN);
        _delay_us(1);
        LCD_Port &= ~ (1<<EN);
        _delay_ms(2);
      }
}
//Write on a specific location
void LCD_Printpos (char row, char pos, char *str) {
    if (row == 0 && pos<16)
    LCD_Action((pos & 0x0F)|0x80);
    else if (row == 1 && pos<16)
    LCD_Action((pos & 0x0F)|0xC0);
    LCD_Print(str);
}


char * seconds_to_time(int seconds){

  static char buf[16];

  int h = seconds / 3600; // hours
  //buf[2] = ':';
  int m = (seconds - (3600*h))/60; // minutes
  //buf[5] = ':';
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
    LCD_Print("Hello world");
    while(1) {
        LCD_Action(0xC0);		//Go to Line 2, Position 1
        LCD_Print(seconds_to_time(runtime));
        _delay_ms(1000);
        runtime--;
    }
}
