#include "main.h"
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>




int main(void)
{
	stdout = &uart;

	initUART();
	printf("Start program\n");

	init_PORT();
	printf("Led status ON\n");

	initSPI_for_HC595();
	printf("SPI_master\n");
	
	port_selection(1,1);
	setOutput(dataOutput);

	initSPI_for_HC165();
	read_status_port ();
	
	while(1)
	{

	}
}