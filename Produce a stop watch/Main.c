
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "lcd_drv.h"
#include "mydefs.h"
#include "lcd_drv.c"

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// timer1 overflw
char msg[8]={'0', '0', ':', '0', '0', ':', '0', '0',0};
int j=0;
ISR(TIMER1_OVF_vect)
{	
	if(msg[7] == '9'){
		if(msg[6] == '5'){
			if(msg[4] == '9'){
				if(msg[3] == '5'){
					if(msg[1] == '9'){
						if(msg[0] =='5'){
							msg[0] = '0';
							msg[1] = '0';
							msg[3] = '0';
							msg[4] = '0';					
							msg[6] = '0';
							msg[7] = '0';
						}else{
							msg[0]++;
							msg[1] = '0';
							msg[3] = '0';
							msg[4] = '0';					
							msg[6] = '0';
							msg[7] = '0';
						}
					}else{
						msg[1]++;
						msg[3] = '0';
						msg[4] = '0';					
						msg[6] = '0';
						msg[7] = '0';
					}
				}else{
					msg[3]++;
					msg[4] = '0';					
					msg[6] = '0';
					msg[7] = '0';
				}
			}else{
				msg[4]++;
				msg[6] = '0';
				msg[7] = '0';
			}
		}else{
			msg[6]++;
			msg[7] = '0';
		}
	}else{
		msg[7]++;
	}

	TCNT1=0xC2F6;

}

// timer0 overflow
ISR(TIMER0_OVF_vect)
{	
	int i= button_pressed();	
	if(i==1){
		if(j==1){
			j=0;
		}else if(j==0){
			j=1;
		}
	}

	TCNT0=0x00;
}

void init_buttons(void){

	ADCSRA = 0x87;

	ADMUX = 0x40;
}

int button_pressed(void){
	
	unsigned int count = 0;

	ADCSRA |= 0x40;

	while (ADCSRA & 0x40)
		;
	unsigned int val = ADCL;
	unsigned int val2 = ADCH;

	val += (val2 << 8);

	count = count + 1;

	if (val > 1000 )
	{
		return 0;
	}
	else
		return 1;
		 
/*    if (val < 50) 
	  return 1;
    else if (val < 195)
	  return 1;
    else if (val < 380)  
	  return 1;
    else if (val < 555)  
	  return 1;
    else 
	  return 1;	
*/
}

int main( void )
{
	lcd_init();	
	init_buttons();

	// enable timer overflow interrupt for both Timer0 and Timer1
	TIMSK0 |=(1<<TOIE0);
  	TIMSK1 |=(1<<TOIE1);
	
	// set timer0 counter initial value to 1
	TCNT0=0x00;
  
 	// set timer1 counter initial value to 0
	TCNT1=0xC2F6;

	// start timer0 with /1024 prescaler. Timer clock = system clock/1024
	TCCR0B = (1<<CS01)|(1<<CS01); 

	// lets turn on 16 bit timer1 also with /1024
	TCCR1B = (1 << CS10)|(1 << CS12);
	
	// enable interrupts
	sei(); 

	while(true) {

		lcd_xy( 0, 0 );
		lcd_puts(msg);

		if(j==0){
			lcd_xy( 0, 1 );
			lcd_puts(msg);		
		}

	}

}




