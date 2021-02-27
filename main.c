#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


#define RS PD2
#define EN PD3
#define LCD_DPin DDRD
#define LCD_Port PORTD

int runtime;

void LCD_Action( unsigned char cmnd )
{
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
    _delay_ms(15);           // Wait Activation
    LCD_Action(0x02);	      //4-Bit Control
    LCD_Action(0x28);       //Control Matrix @ 4-Bit
    LCD_Action(0x0c);       //Disable Cursor
    LCD_Action(0x06);       //Move Cursor
    LCD_Action(0x01);       //Clean LCD
    _delay_ms(2);
}

void LCD_Clear()
{
    LCD_Action (0x01);		//Clear LCD
    _delay_ms(2);			//Wait to clean LCD
    LCD_Action (0x80);		//Move to Position Line 1, Position 1
}



void LCD_Print (char *str)
{
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
void LCD_Printpos (char row, char pos, char *str)
{
    if (row == 0 && pos<16)
    LCD_Action((pos & 0x0F)|0x80);
    else if (row == 1 && pos<16)
    LCD_Action((pos & 0x0F)|0xC0);
    LCD_Print(str);
}

int main(void) {

  LCD_Init();
  LCD_Print("Hello world");
  while(1) {
      char showruntime [16];
      itoa (runtime,showruntime,10);
      LCD_Action(0xC0);		//Go to Line 2, Position 1
      LCD_Print("RUNTIME (s): ");
      LCD_Print(showruntime);
      _delay_ms(1000);
      runtime++;
	}
}
