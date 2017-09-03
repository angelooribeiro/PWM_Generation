/*
 * number_conv.c
 *
 * Created: 27/02/2016 20:54:05
 *  Author: angel
 */ 

#include "number_conv.h"

long int parse_number(void);			//read the base and "makes the number"
long int parse_number_digital(void);
long int conv_hexa_dec(void);		//convert from hexadecimal to decimal
long int conv_bin_dec(void);			//convert from binary to decimal
long int conv_dec_dec(void);			//needed because is this function that returns the number
unsigned char base = 0;

extern char str_read();
extern unsigned char digital_flag;

//read the base and "makes the number"
long int parse_number(void)
{
	char temp_base = str_read();			//read the base of the number
	
	if(digital_flag == 1)
		base = temp_base;					//if there is only a base
		
	switch(temp_base)
	{
		//binary
		case 'b':			
		case 'B':
			return conv_bin_dec();
			break;
			
		//hexadecimal
		case 'h':
		case 'H':
			return conv_hexa_dec();
			break;
			
		//decimal
		case 'd':
		case 'D':
			return conv_dec_dec();
			break;
			
		default:
			return -1;
			break;
	}
}	//end function

//read the base and "makes the number"
long int parse_number_digital(void)
{
	switch(base)
	{
		//binary
		case 'b':
		case 'B':
		return conv_bin_dec();
		break;
		
		//hexadecimal
		case 'h':
		case 'H':
		return conv_hexa_dec();
		break;
		
		//decimal
		case 'd':
		case 'D':
		return conv_dec_dec();
		break;
		
		default:
		return -1;
		break;
	}
}


//convert from hexadecimal to decimal
long int conv_hexa_dec(void)
{
	char temp_number = str_read();
	int dec_number = 0;

	while( (temp_number != '\0') && (temp_number != 0x20) && (temp_number != '.') )		//if not reache the end of the number in the array
	{
		if( (temp_number>='0') && (temp_number<='9') )				//if is a number
		dec_number = dec_number*16+(temp_number-'0');

		if( (temp_number >= 'A') && (temp_number <= 'F') )									//if is a capital letter
		dec_number = dec_number*16+(temp_number-55);

		if( (temp_number >= 'a') && (temp_number <= 'f') )										//if is a small letter
		dec_number = dec_number*16+(temp_number-87);

		if( !( ((temp_number>='0') && (temp_number<='9')) || ((temp_number >= 'A') &&
		(temp_number <= 'F')) || ((temp_number >= 'a') && (temp_number <= 'f')) ) )	/*if is strange character*/
		return -1;

		temp_number = str_read();
	}
	return dec_number;
} //end function

long int conv_bin_dec()			//convert from binary to decimal
{
	char temp_number = str_read();
	int dec_number = 0;

	while( (temp_number != '\0') && (temp_number != 0x20) && (temp_number != '.') )		/*not reach the end?*/
	{
		if( (temp_number != '0') && (temp_number != '1') )	/*if is not valid*/
		return -1;

		if( (temp_number == '0') || (temp_number == '1') )								/*is a valid number*/
			dec_number = (dec_number * 2) + (temp_number - '0');		/*actualize the number*/
			
		temp_number = str_read();							/*goes next*/
	}
	return dec_number;
}/*end function*/

//needed because is this function that returns the number
long int conv_dec_dec(void)	
{
	char temp_number = str_read();
	int dec_number = 0;

	while( (temp_number != '\0') && (temp_number != ' ') && (temp_number != '.') )		//if not reached the end of the number
	{
		if( (temp_number < '0') || (temp_number > '9') )		/*if is a strange character*/
		return -1;

		if( (temp_number >= '0') && (temp_number <= '9') )	/*if is a valid number*/
		{
			if(dec_number != 0)
			{
				int temp_dec = dec_number *10;					//get space for units
				dec_number = temp_dec + ( temp_number - 48);		//actualized number
			}

			if(dec_number == 0)							//first time units
			dec_number = ( temp_number - 48 );		//convert to int
		}
		temp_number = str_read();							/*get the next number*/
	}
	return dec_number;
}/*end function*/

//convert from hexa to ascii
void conv_hexa_ascii(unsigned char* conv, char nbytes){
	char hex [8];
	switch(nbytes){
		case 1:
		hex[0] = (conv[0]&0xF0) >> 4;
		hex[1] = conv[0]&0x0F;
		for(int i = 0; i<2; i++){
			if (hex[i] < 10) serial_write(hex[i]+48);
			else serial_write(hex[i]+55);
		}
		break;
		case 2:
		hex[2] = (conv[0]&0xF0) >> 4;
		hex[3] = conv[0]&0x0F;
		hex[0] = (conv[1]&0xF0) >> 4;
		hex[1] = conv[1]&0x0F;
		for(int i = 0; i<4; i++){
			if(i == 2)  serial_write(' ');
			if (hex[i] < 10) serial_write(hex[i]+48);
			else serial_write(hex[i]+55);
		}
		break;
	}
}/*end function*/

//convert from ADC to ascii(in uV)
void conv_adc_ascii(){
	unsigned char* string;
	unsigned long int micro_volt;
	micro_volt = (unsigned long int)(ADC) * 4883;
    int i = 0;

    do {       /* generate digits in reverse order */
		string[i++] = micro_volt % 10;  /* get next digit */
		micro_volt /= 10;			/* delete it */
	} while (micro_volt);

	while(i > 0){       /* send digits in reverse order */
		i--;
		serial_write(string[i]+48);
	}
	serial_writestr(" uV");
}