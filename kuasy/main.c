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

#define LATCH               PC6  //линия синхронизации,защелка
#define MOSI                PB5  //линия передачи
#define SCK                 PB7  //линия тактирования
#define MISO                PB6 
#define SS                  PB4  

#define set_bit_up(x,y)        x|=(1<<y)   //установка бита лог1
#define set_bit_down(x,y)      x&= ~(1<<y) //установка бита лог0


#define DDR_STATUS_LED		DDRD
#define PORT_STATUS_LED		PORTD
#define STATUS_LED			PIND7 //led
#define SPI_PORT            PORTB
#define SPI_DDR             DDRB

#define COUNT_HC595	4

#define LED_4	4
#define LED_3	3
#define LED_2	2
#define LED_1	1
#define LED_0	0

volatile unsigned char sendData = 0; 

uint8_t dataOutput[COUNT_HC595];

void port_selection(int num_led, int status);
void init_PORT();
void init();
void initUART();
int send(char c, FILE *stream);
void init_SPI();
void send_SPI(uint8_t data);
void setOutput(uint8_t *data);
void init_SPI_HC595();
void clearOutputData();

static FILE uart = FDEV_SETUP_STREAM(send, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	stdout = &uart;
   
	initUART();
	printf("Start program\n");
	
	init_PORT();
	printf("Led status ON\n");
	
	init_SPI_HC595();
	printf("SPI_master\n");
	
	port_selection(29 ,1);
	setOutput(dataOutput);
	
	while(1)
	{
			
	}	
}

int send(char c, FILE *stream) 
{
	if (c == '\n')
		send('\r', stream);
	
	while(!(UCSRA&(1<<UDRE))){	
	}
	UDR = c;
	return 0;
}

void init_PORT()
{
	set_bit_up(PORT_STATUS_LED,STATUS_LED);
	set_bit_up(DDR_STATUS_LED,STATUS_LED);
// 	DDR_STATUS_LED |= (1<<STATUS_LED);
// 	PORT_STATUS_LED |= (1<<STATUS_LED);
//  DDRB|=(1<<PB4);  //SS на выход	
}

void initUART()
{
	DDRD  |= (1<<PIND1);  //RX 
	PORTD |= (1<<PIND0);  //TX
    //настройка скорости
	UBRRH = 0x00;//старший бит
	UBRRL = 0x67;//младший бит
	UCSRB |=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);//настройка регистров
	UCSRC |=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);	
}

void init_SPI_HC595()
{
	SPCR |=(1<<SPE)|(1<<MSTR);   //on SPI; master;
	SPI_DDR|=(1<<MOSI)|(1<<SS)|(1<<SCK);//выход
	SPI_DDR &= ~(1<<MISO);
	SPI_PORT |=(1<<SS); //1 на выходе
	DDRC|=(1<<PC5)|(1<<PC7)|(1<<PC6);//настройка входа для НС595
	PORTC &= ~(1<<PC5);     //OE
}

void send_SPI(uint8_t data)     //uint8_t-u - unsigned - беззнаковое,int - integer - целое,8 - кол-во битов
{
    SPDR = data;                  // регистр чтения,записи
    while(!(SPSR & (1<<SPIF)));
}

void setOutput(uint8_t *data) 
{
	    set_bit_up(PORTC,PC7);     //MR
		for (int i = 3; i >= 0; i--)
		{
			send_SPI(*(data + i));
		}
		set_bit_up(PORTC,PC6);     //защелка
		_delay_ms(1);
		set_bit_down(PORTC,PC6);
		
		set_bit_down(PORTC,PC7);     //MR	
}
void port_selection(int num_led, int status)//выбор микросхем и пин
{ 
	  num_led--;
	  uint8_t q = num_led /8; 
	  uint8_t w = num_led %8; 

	 if(status==1)
	 {
		dataOutput[q] |=(1<<w);
	 }
	  if(status==0)
	  {
		dataOutput[q] &= ~(1<<w);
	  }	  
}

void clearOutputData() 
{
	for (int i = 0; i < COUNT_HC595; i++) 
	{
		dataOutput[i] = 0x00;
	}
}


