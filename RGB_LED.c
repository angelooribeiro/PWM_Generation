/*
 * RGB_LED.c
 *
 * Created: 10/05/2016 00:03:40
 *  Author: angel
 */ 
#include "RGB_LED.h"

volatile int pwm_red;
volatile int pwm_green;
volatile int pwm_blue;

volatile int temp_pwm_red;
volatile int temp_pwm_green;
volatile int temp_pwm_blue;

volatile int temp_pwm_value;

volatile float step;
unsigned char temp_TCCR5B;
volatile unsigned char flag_inc;
volatile unsigned char color;
volatile unsigned char temp_color;

/*receive the RGB character and actualize the duty cycle of witch color*/
void duty_cycle_rgb(unsigned char signal_character)
{
		unsigned char control_digit2 = str_read();		/*second space*/
		temp_pwm_value = parse_number();	/*takes the % of pwm in hex*/
		unsigned char control_digit3 = str_read();		/*end character*/
			
		if( (temp_pwm_value>=0) && (temp_pwm_value<=100) && (control_digit2==0x20) && (control_digit3=='\0') )
		{
			temp_pwm_value = (0xFF * temp_pwm_value) / 100;	/*convert for the recharge value*/
			switch(signal_character)
			{
				case 'R':
					(temp_pwm_value > pwm_red) ? (temp_pwm_red = temp_pwm_value - pwm_red) : (temp_pwm_red = pwm_red - temp_pwm_value); /*abs*/
					(temp_pwm_red > pwm_red) ? (flag_inc = 1) : (flag_inc = 2);
					temp_color = 'R';
				break;
							
				case 'G':
					(temp_pwm_value > pwm_green) ? (temp_pwm_green = temp_pwm_value - pwm_green) : (temp_pwm_green = pwm_green - temp_pwm_value); /*abs*/
					(temp_pwm_green > pwm_green) ? (flag_inc = 1) : (flag_inc = 2);
					temp_color = 'G';
				break;
							
				case 'B':
					(temp_pwm_value > pwm_blue) ? (temp_pwm_blue = temp_pwm_value - pwm_blue) : (temp_pwm_blue = pwm_blue - temp_pwm_value); /*abs*/
					(temp_pwm_blue > pwm_blue) ? (flag_inc = 1) : (flag_inc = 2);
					temp_color = 'B';
				break;
							
				default:
					serial_error();
				break;
			} /*end switch*/
		}
		else	serial_error();
} /*end function*/

/*run an pwm color*/
void run_color_rgb()
{
	unsigned char signal_character = str_read();
	unsigned char conntrol_digit1 = str_read();
	
	if( (conntrol_digit1 == '\0') && (signal_character == color) && (color == temp_color) )
	{
		switch(signal_character)
		{
			case 'R':
				TCCR1B |= (1<<CS10);
				TCCR5B |= temp_TCCR5B;
				temp_color = 0;
			break;
		
			case 'G':
				TCCR3B |= (1<<CS30);
				TCCR5B |= temp_TCCR5B;
				temp_color = 0;
			break;
		
			case 'B':
				TCCR4B |= (1<<CS40);
				TCCR5B |= temp_TCCR5B;
				temp_color = 0;
			break;
		
			default:
				serial_error();
				temp_color = 0;
			break;
		} /*end switch*/
	}
}

/*stop a pwm color*/
void stop_color_rgb()
{
	color = str_read();
	unsigned char control_digit1 = str_read();
	
	if( ((color == 'R') || (color =='G') || (color == 'B')) && (control_digit1 == '\0') )
	{	
		OCR5A = 0x01;
		TCCR5B |= 0x02; /*just to preform the operation in the timer*/ 
		flag_inc = 0;	/*takes more time but is easier to implement*/
	}
	else
		serial_error();
}

/*defines a start or stop ramp*/
void ramp_rgb(void)
{
	unsigned char control_digit1 = str_read();  /*space*/
	color = str_read();			/*rgb color*/
	unsigned char control_digit2 = str_read();	/*space*/
	step = parse_number();		/*% of inc dec*/
	unsigned char control_digit3 = str_read_last();	/*space*/
	unsigned char time_units = parse_number();	/*get 1,2,3 time units -> ys, ms, s*/
	unsigned char control_digit4 = str_read_last();	/*space*/
	int step_time = parse_number();	/*time of which step*/
	unsigned char control_digit5 = str_read();	/*\0*/
	
	
	if( (control_digit1 == 0x20) && (control_digit2==0x20) && (control_digit3==0x20) && (control_digit4 == 0x20) && (control_digit5=='\0') && ( (color == 'R') || 
	(color == 'G') || (color == 'B')) && (step>=0) && (step<=100) && ( (time_units == 1) || (time_units ==2) || (time_units==3)) )
	{	
		step = (step/100); /*to multiply */
		
		if(time_units == 1)   /*ys*/
		{
			OCR5A = step_time *2;
			temp_TCCR5B = 0x02;	/*fclock/8 = 0.5ys too make one secound (*2)*/
		}
		
		if(time_units == 2)    /*ms*/
		{
			OCR5A = (65535*step_time)/1000;		/*65535->1000ys; x->step_time*/
			temp_TCCR5B = 0x04;	/*fclock/256 = 16ys - too make one secound - 1 048 560*/
		}
		
		if(time_units == 3)	  /*s*/
		{
			OCR5A = (65535*step_time)/4;		/*65535->4s; x->step_time*/
			temp_TCCR5B = 0x05;	/*fclock/1024 = 64ys  - 4 194 240*/
		}
		
	}else serial_error();

}