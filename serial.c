//serial port comunication defenition
//USING USART0
#include "serial.h"

extern volatile char received_command[];
extern volatile char last_command[];
extern void parse();
extern volatile char parse_index;

static char help[] = "MR - Memory Read : MR <base><addr> <base><length> \nMW - Memory Write : MW <base><addr> <base><length> <base><byte>\nMC - Memory Copy : MC <base><org> <base><length> <base><dest>\nRI - Read INTeger : RI <base><addr>\nWI - Write INTeger : WI <base><addr> <base><value>\nRD - Read Digital Input : RD <base><addr8> <addr3>\nWD - Write Digital Output : WD <base><addr8> <addr3> <bitvalue>\nRA - Analog Read : RA <base><addr3>\nWE - E2PROM Write : WE <base><addr> <base><byte>\nRE - E2PROM Read : RE <base><addr> <base><length>\n<BCKSP> - Backspace\n<ESC> - Abort\n'$' - Limpa todos os caracteres recebidos e repete o ultimo comando\nVER - Version\n";


//serial port configuration
void serial_configuration()
{
	//Baud rate 9600, fosc=16M
	UBRR0H = (103 >> 8);
	UBRR0L = 103;
	//RESTAR ALL FLAGS
	UCSR0A = 0X00;
	//enable transmission and reception
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0);
	//USART asynchronous mode, 8 bits, 1 stop bit
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	
	//initiation of auxiliary variables
	istart = 0;
	iend  = 0;
	ostart = 0;
	oend = 0;
}
//end function

//process the received data
void serial_receive()
{
		unsigned char data = UDR0;					//save received char
		
		//<BCKSP>, clear last received
		if(data == 8)									//#########################done and tested#########################
		{
			if(iend != istart)						//if exist data in buffer								
				istart =  (istart-1);	//returns back one
		}
		
		//<ESC>, clear all received, istart goes to the same position as iend
		if(data == 27)									//#####################done and tested#########################
		{	
			if(iend != istart)						//if exist data in buffer								
				istart = (iend & input_mask);			//jumps over all received data
		}
		
		//clear all received, execute last command		#################done and tested################################
		if(data == '$')								
		{
			unsigned char i = 0;
			istart = (iend & input_mask);			//jumps over all received data
			while( last_command[i] != '\0')			//while there is not a enter
			{
				received_command[i] = last_command[i];		//copies the last command for the buffer used for processor
				i++;
			}
			received_command[i] = '\0';
			parse_index = 0;
			parse();	//goes to execute the command
		}
		
		//HELP not working for too big arrays
		if(data == '?')
		{	
			serial_writestr(help);
		}
		
		if(data == 13)		//execute command			//######################done and tested#############################
		{	
			unsigned char i = 0;
			while( istart != ( iend & input_mask) )
			{
				iend = (iend+1) & input_mask;
				received_command[i] = inbuf[iend];
				i++;
			}
			received_command[i] = '\0';				//end character
			iend++;									//jumps over 13
			parse_index = 0;
			parse();
		}
		
		//stores the received byte
		if( (data!=8) && (data!=27) && (data!='$') && (data!='?') )
		{
			istart = (istart+1)&input_mask;			//actualize istart index
			inbuf[istart] = data;					//put in buffer the received data
		}
}
//end function

//interrupt service routine on reading
ISR(USART0_RX_vect)
{	
	serial_receive();
}
//end function

//write for serial port					
void serial_write(char data)				
{
	while(!(1<<UDRIE0));	//waits for free space

	ostart = ( (ostart+1) & output_mask);			//actualization of the position
	outbuf[ostart] = data;		//stores data in the buffer						
		
	UCSR0B |= (1<<UDRIE0);						//enable sending
}
//end function

//interrupt service routine on sending
ISR(USART0_UDRE_vect)
{
	if( ostart == oend)						//if all is transmitted 
		UCSR0B &= ~(1<<UDRIE0);				// Disable UDRE interrupt
	
	if ( ostart != oend )						//if there something to transmit
	{
		oend = ( oend+ 1 ) & output_mask;	//actualization of index
		UDR0 = outbuf[oend];				//send data		
	}
}
//end function

//prints a buffer/string
void serial_writestr( char* data )
{
	while((*data) != '\0' )
	{
		serial_write(*data);
		data++;
	}
}//end function

//prints an error message
void serial_error()
{
	serial_writestr("Error\n");
}//end function



/*copyright RIBEIRO_ANGELO*/