//serial port comuncication declaration

#ifndef SERIAL_H
#define SERIAL_H

#include "header.h"

//output
#define OLEN 1024							//size of serial transmission buffer, power of 2
#define output_mask (OLEN - 1)				//mask for calculate index's
static volatile unsigned int ostart;		//transmission buffer start index
static volatile unsigned int oend;			//transmission buffer end index
static volatile char outbuf[OLEN];			//storage for transmission buffer

//input
#define ILEN 128								//size of serial receiving buffer, power of 2
#define input_mask (ILEN -1)				//mask for calculate index's
static volatile unsigned char istart;		//receiving buffer start index
static volatile unsigned char iend;			//receiving buffer end index
static volatile char inbuf[ILEN];			//storage for receiving buffer


void serial_configuration(void);				//initialization of serial port
void serial_receive(void);						//deals with the interrupt response
void serial_write(char byte);					//write for serial port
void serial_printstr( void );					//prints a buffer/string
void serial_writestr( char* data );				//to transmit a string
void serial_error(void);						//returns an error message 

#endif

/*copyright RIBEIRO_ANGELO*/