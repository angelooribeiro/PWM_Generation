/*
 * RGB_LED.h
 *
 * Created: 10/05/2016 00:03:26
 *  Author: angel
 */ 


#ifndef RGB_LED_H_
#define RGB_LED_H_

#include "header.h"

/*receive the RGB character and actualize the duty cycle of witch color*/ 
void duty_cycle_rgb(unsigned char signal_character);
void run_color_rgb(void);		/*run an pwm color*/
void stop_color_rgb(void);		/*stop a pwm color*/
void ramp_rgb(void);			/*defines a start or stop ramp*/


#endif /* RGB_LED_H_ */