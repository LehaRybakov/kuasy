/*
 * kuasy.c
 *
 * Created: 12.10.2022 13:24:59
 * Author : a.rybakov
 */ 



#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "driver/terminal.h"

#define DDR_STATUS_LED		DDRD
#define PORT_STATUS_LED		PORTD
#define STATUS_LED			PIND7

volatile unsigned char sendData = 0; 



void send(unsigned char data)
{
	while(!(UCSRA&(1<<UDRE)));
	UDR = data;
}

void sendString (unsigned char str[], int length) {
	for (int i = 0; i < length; i++)
	{	
		send(str[i]);
	}
}

void sendstr (unsigned char *s)
{
	while (*s != 0)
		send(*s++);	
}

void hello(uint8_t number);
void initUART();
void init();


uint8_t globalValue = 0;

int main(void)
{
	stdout = &mystdout;
		init();
		initUART();
	
	while(1)
	{
		hello(globalValue++);
		_delay_ms(10);
		globalValue = (globalValue==256)?0:globalValue;
	}
}

void hello(uint8_t number) {
	//printf("Hello!!!!\n");
	printf("Value = %d\n", number);
}

void initUART()
{
	DDRD |= (1<<PIND1);
	PORTD |= (1<<PIND0);
	
	UBRRH = 0x00;
	UBRRL = 0x67;
	UCSRB |=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	UCSRC |=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

void init()
{
	DDR_STATUS_LED |= (1<<STATUS_LED);
	PORT_STATUS_LED |= (1<<STATUS_LED);
}


