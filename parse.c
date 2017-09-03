/*
 * CFile1.c
 *
 * Created: 22/02/2016 22:13:04
 *  Author: angel
 */ 


#include "header.h"

volatile unsigned char digital_flag;
extern volatile char parse_index;
extern volatile unsigned char pwm_value;
extern volatile char received_comand[];					//command to be executed
extern char str_read(void);
extern void parse_read();
extern volatile int pwm_red;

void parse(void)
{
	digital_flag = 0;
			
	char temp = str_read();
	switch(temp)
	{
		/*defines the duty cycle value*/
		case 'd':
		case 'D':
			temp = str_read();			/*c*/
			unsigned char control_digit1 = str_read();		/*space*/
			unsigned char signal_character = str_read();	/*+, -, R, G or B*/
			serial_write('3');
			/*test of the common conditions*/
			if( ((temp == 'c' ) || (temp == 'C')) && (control_digit1==0x20) )
			{serial_write('2');
				/*direct and reverse pwm for H-Bridge*/
				if( (signal_character == '+') || (signal_character == '-') )
				{			serial_write('1');
					unsigned int temp_pwm_value = parse_number();	/*takes the % of pwm in hex*/
					unsigned char control_digit2 = str_read();		/*end character*/
					
					if( (temp_pwm_value>=0) && (temp_pwm_value<=100) && (control_digit2=='\0') )
					{	unsigned int *p = (unsigned int*) (0x22);	/*pointer for the DDRA*/
						/*test if is a + if it is activate direct pwm, if isn't activates reverse pwm*/
						(signal_character == '+') ? ((*p) |= (1 << 0)) : ((*p) &= ~(1 << 0));
						pwm_red = temp_pwm_value;
						//pwm_red = (int)(((0xFF * temp_pwm_value)) / 100);	/*actualizate the duty cycle*/
						OCR1A = pwm_red;
						serial_write(OCR1A);
					}
					else	serial_error();
			}
				
				/*Duty cycle for the RGB LED*/
				else if( (signal_character == 'R' || 'G' || 'B') ) 
					duty_cycle_rgb(signal_character);
				
				else	serial_error();
				
			} /*end main if*/
			else	serial_error();
	
		break;


		/*request for start the pwm*/
		case 'r':
		case 'R':
			temp = str_read();
			if( (temp == 'u') || (temp =='U') )	
			{
				temp = str_read();
				if(temp == '\0') DDRB |= (1<<PB5);		/*Change 13 to output*/
				else if(temp == 0x20) run_color_rgb();		/*RGB PWM*/
				else serial_error();
			}
			
			else if((temp == 'p') || (temp == 'P'))	ramp_rgb();
			else serial_error();
		break;
		
		//request for start the pwm
		case 's':
		case 'S':
			temp = str_read();
			if( (temp == 't' || 'T'))
			{	
				temp = str_read();
				if(temp == '\0') DDRB &= ~(1<<PB5);		/*Change 13 to input*/
				else if(temp == 0x20) stop_color_rgb();		/*RGB PWM*/
				else serial_error();
			}
			else	serial_error();
		break;
		
		default:
			serial_error();
	} /*end main switch*/
	
} /*end function*/
