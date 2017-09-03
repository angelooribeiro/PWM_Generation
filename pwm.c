/*
 * pwm.c
 *
 * Created: 03/05/2016 09:36:48
 *  Author: angel
 */ 
#include "pwm.h"

volatile char pwm_value; /*defines the pwm high time*/

extern volatile int pwm_blue;
extern volatile int pwm_red;
extern volatile int pwm_green;

extern volatile int temp_pwm_red;
extern volatile int temp_pwm_green;
extern volatile int temp_pwm_blue;

extern volatile int temp_pwm_value;

extern volatile float step;
extern volatile unsigned char flag_inc;
extern volatile unsigned char color;

void config_pwm(void)
{
	TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00) ;		/*set OC0A on Compare Match*/
	/*Clear OC0A on Compare Match, set OC0A at BOTTOM (non-inverting mode) */
	TCCR0B |= (1<<CS00) | (1<<WGM02);
	/*FOC0A: Force Output Compare A*/
	TIMSK0 |= (1<<FOC0A) | (1<<OCIE0A);
	/*OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable*/
	DDRA = 1; /*ativated as output, used to control the pwm mode (direct or reverse)*/
	DDRB = 1;
	OCR0A = 0x11;
}

void config_pwm_rgb(void)
{
	/*pwm_red PB5 pin 11*/
	TCCR1A |= (1<<WGM10) | (1<<COM1A1) ;	/*fast pwm 8 bists compare output on A*/
	TCCR1B |= (1<<WGM12);					/*FOC0A: Force Output Compare A*/
	TIMSK1 |= (1<<OCIE1A);								/*Output Compare Match A Interrupt Enable*/
	DDRB |= (1<<PB5);
	OCR1A = 0x50; TCCR1B |= (1<<CS10); pwm_red = 0x50;
	
	/*pwm_green PE3 pin5 pwm*/
	TCCR3A |= (1<<WGM30) | (1<<COM3A1) ;	/*fast pwm 8 bists compare output on A*/
	TCCR3B |= (1<<WGM32);					/*FOC0A: Force Output Compare A*/
	TIMSK3 |= (1<<OCIE3A);								/*Output Compare Match A Interrupt Enable*/
	DDRE |= (1<<PE3);
	
	/*pwm_blue PH3 pin6*/
	TCCR4A |= (1<<WGM42) | (1<<WGM40) | (1<<COM4A1) ;	/*fast pwm 8 bists compare output on A*/
	TCCR4B |= (1<<WGM42);					/*FOC0A: Force Output Compare A*/
	TIMSK4 |= (1<<OCIE4A);								/*Output Compare Match A Interrupt Enable*/
	DDRH |= (1<<PH3);
}

/*to make cycles of time*/
void config_timer5(void)
{
	TCCR5A |= (1<<COM5A0);					/*compare output A*/ 
	TCCR5B |= (1<<FOC5A) | (1<<WGM52);		/*CTC mode*/
	TIMSK5 |= (1<<OCIE5A);
}

ISR(TIMER0_COMPA_vect)
{
	OCR0A = 0x11;
}

/*
ISR(TIMER0_OVF_vect)
{
	OCR0A = 0xF;	
}
*/

ISR(TIMER1_COMPA_vect)
{
	OCR1A = pwm_red;
}

ISR(TIMER3_COMPA_vect)
{
	OCR3A = pwm_green;
}

ISR(TIMER4_COMPA_vect)
{
	OCR4A = pwm_blue;
}

ISR(TIMER5_COMPA_vect)
{	
	if(color == 'R')		/*red color*/
	{
		if( (flag_inc==0) && (pwm_red > 0) )		/*stop the pwm*/
		{
			pwm_red = 0;
			step = 0;
			TCCR5B &= ~0x07;
			color = 0;
		}
		
		if(flag_inc == 1)		/*inc the pwm with ramp*/
		{
			if(pwm_red < temp_pwm_value)
			{
				if( (pwm_red + (int)(step * temp_pwm_red)) >= temp_pwm_value) /*prevents the overflow*/
				{
					TCCR5B &= ~0x07;
					pwm_red = temp_pwm_value;
					step = 0;
					color = 0;
				}
				else
					pwm_red = pwm_red + (int)(step * temp_pwm_red);
			}
		}
		
		if(flag_inc == 2)	/*dec the pwm with ramp*/
		{
			if(pwm_red >= temp_pwm_value)
			{
				if( (pwm_red - (int)(step * temp_pwm_red)) <= temp_pwm_value) /*prevents the overflow*/
				{
					TCCR5B &= ~0x07;
					pwm_red = temp_pwm_value;
					step = 0;
					color  = 0;
				}
				else
				pwm_red = pwm_red - (int)(step * temp_pwm_red);
			}
		}
	} /*end red*/
	
	if(color == 'G')		/*green color*/
	{
		if( (flag_inc==0) && (pwm_green > 0) )		/*stop the pwm*/
		{
			pwm_green = 0;
			step = 0;
			TCCR5B &= ~0x07;
			color = 0;
		}
		
		if(flag_inc == 1)		/*inc the pwm with ramp*/
		{
			if(pwm_green < temp_pwm_value)
			{
				if( (pwm_green + (int)(step * temp_pwm_green)) >= temp_pwm_value) /*prevents the overflow*/
				{
					TCCR5B &= ~0x07;
					pwm_green = temp_pwm_value;
					step = 0;
					color = 0;
				}
				else
				pwm_green = pwm_green + (int)(step * temp_pwm_green);
			}
		}
		
		if(flag_inc == 2)	/*dec the pwm with ramp*/
		{
			if(pwm_green >= temp_pwm_value)
			{
				if( (pwm_green - (int)(step * temp_pwm_green)) <= temp_pwm_value)		/*prevents the overflow*/
				{
					TCCR5B &= ~0x07;
					pwm_green = temp_pwm_value;
					step = 0;
					color = 0;
				}
				else
				pwm_green = pwm_green - (int)(step * temp_pwm_green);
			}
		}
	} /*end green*/
		
		
	if(color == 'B')		/*blue color*/
	{
		if( (flag_inc==0) && (pwm_blue > 0) )		/*stop the pwm*/
		{
			pwm_blue = 0;
			step = 0;
			TCCR5B &= ~0x07;
			color = 0;
		}
		
		if(flag_inc == 1)		/*inc the pwm with ramp*/
		{
			if(pwm_blue < temp_pwm_value)
			{
				if( (pwm_blue + (int)(step * temp_pwm_blue)) >= temp_pwm_value) /*prevents the overflow*/
				{
					TCCR5B &= ~0x07;
					pwm_blue = temp_pwm_value;
					step = 0;
					color = 0;
				}
				else
				pwm_blue = pwm_blue + (int)(step * temp_pwm_blue);
			}
		}
		
		if(flag_inc == 2)	/*dec the pwm with ramp*/
		{
			if(pwm_blue >= temp_pwm_value)
			{
				if( (pwm_blue - (int)(step * temp_pwm_blue)) <= temp_pwm_value)		/*prevents the overflow*/
				{
					TCCR5B &= ~0x07;
					pwm_blue = temp_pwm_value;
					step = 0;
					color = 0;
				}
				else
				pwm_blue = pwm_blue - (int)(step * temp_pwm_blue);
			}
		}
	} /*end blue*/
 }