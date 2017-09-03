/*
 * GccApplication1.c
 *
 * Created: 20/02/2016 09:23:40
 * Author : angel
 */ 
#include "header.h"

char version[60] = "Version 1.0 Atmega2560 - Grupo 4 LPI-II DEI UM2016\n";			//program version
volatile char received_command[30];							//command to be executed 
volatile char last_command[30];								//stores the last command executed with success 
volatile char parse_index;
void save_command();										//save the last command executed with success 
char str_read_last(void);

//used to read the received_command array
char str_read(void)
{
	char temp_parse = '\0';
	temp_parse = received_command[parse_index];
	if(received_command[parse_index]!='\0')
	parse_index ++;
	
	return temp_parse;
}//end function

//to read a character that as been redden 
char str_read_last(void)
{
	char temp_parse = '\0';
	temp_parse = received_command[parse_index-1];
	return temp_parse;
} //end function

//saves the last comand executed with success
void save_command()
{
	int i = 0;
	while(received_command[i] != '\0')				//while is not the terminator character
	{
		last_command[i] = received_command[i];		//copies character by character
		i++;
	}
	last_command[i] = '\0';							//inserts the terminator character
	
}//end function 

int main(void)
{
	serial_configuration();		//configuration of serial port
	sei();
	config_pwm();
	config_pwm_rgb();
	config_timer5();
	while (1)
	{
		//nao faço nada
	}
		
}


/*copyright RIBEIRO_ANGELO*/

