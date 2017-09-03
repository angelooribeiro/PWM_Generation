/*
 * number_conv.h
 *
 * Created: 27/02/2016 20:53:52
 *  Author: angel
 */ 


#ifndef NUMBER_CONV_H_
#define NUMBER_CONV_H_

#include "header.h"

extern long int parse_number(void);			//read the base and return the translated number
extern long int parse_number_digital(void);		//to oprtation of one base comand
long int conv_dec_dec(void);					//convert from decimal(ascii) to decimal
long int conv_bin_dec(void);					//convert from binary(ascii) to decimal
long int conv_hexa_dec(void);				//convert from hexa(ascii) to decimal
void conv_hexa_ascii(unsigned char* conv, char nbytes);				//convert from hexa from ADC to ascii (conv = pointer to char array, nbytes = number of bytes)
void conv_adc_ascii();					//convert from ADC to ascii(in uV)

#endif /* NUMBER_CONV_H_ */