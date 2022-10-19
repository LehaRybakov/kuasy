/*
 * kuasy.c
 *
 * Created: 12.10.2022 13:24:59
 * Author : a.rybakov
 */ 





#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#include <stdio.h>


#define DDR_STATUS_LED		DDRD
#define PORT_STATUS_LED		PORTD
#define STATUS_LED			PIND7

volatile unsigned char sendData = 0; 



void init();
void initUART();
static int send(char c, FILE *stream);

static FILE uart = FDEV_SETUP_STREAM(send, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	stdout = &uart;

	initUART();
	printf("Start program\n");
	
	init();
	printf("Led status ON\n");
	
	while(1)
	{
		
		
	}	
}

static int send(char c, FILE *stream) {
	if (c == '\n')
		send('\r', stream);
	
	while(!(UCSRA&(1<<UDRE))){
		
	}
	UDR = c;
	return 0;
}

void init()
{
	DDR_STATUS_LED |= (1<<STATUS_LED);
	PORT_STATUS_LED |= (1<<STATUS_LED);
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
