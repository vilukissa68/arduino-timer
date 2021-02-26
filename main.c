#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR     DDRB
#define LED_PIN     PINB
#define LED         PB5

int main(void)
{
  DDRB = 0xFF;

  while (1)
    {
      PORTB ^= 0xFF;
      _delay_ms(500);
    }

  return(0);
}
