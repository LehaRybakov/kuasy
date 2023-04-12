/*
 * kuasy.c
 *
 * Created: 12.10.2022 13:24:59
 * Author : a.rybakov
 */ 

#include "main.h"

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
	//read_status_port ();
	
	
	read_HC165();
	read165transl595 ();
	
	
	
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
//on led
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
	UBRRH = 0x00;    //старший бит
	UBRRL = 0x67;    //младший бит
	
	UCSRB |=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);   //настройка регистров
	UCSRC |=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

void initSPI_for_HC595()
{
	SPCR |=(1<<SPE)|(1<<MSTR)|(1<<SPR0);   //on SPI; master;
	SPI_DDR|=(1<<MOSI)|(1<<SS)|(1<<SCK);   //выход
	SPI_DDR &= ~(1<<MISO);
	SPI_PORT |=(1<<SS);                    //1 на выходе
	DDRC|=(1<<PC5)|(1<<PC7)|(1<<PC6);      //настройка входа для НС595
	//PORTC &= ~(1<<PC5);                  //OE
}

void send_SPI(uint8_t data)     //uint8_t-u - unsigned - беззнаковое,int - integer - целое,8 - кол-во битов
{
	SPDR = data;                // регистр чтения,записи
	while(!(SPSR & (1<<SPIF)));
}

void setOutput(uint8_t *data)
{
	set_bit_up(PORTC,PC7);       //MR
	for (int i = 3; i >= 0; i--)
	{
		send_SPI(*(data + i));
	}
	set_bit_up(PORTC,PC6);      //защелка
	_delay_ms(1);
	set_bit_down(PORTC,PC6);
	
	set_bit_down(PORTC,PC7);    //MR
}
void port_selection(int num_led, int status) //выбор микросхем и пин
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
void initSPI_for_HC165()
{
	set_bit_up(DDRB,PL);      //защелка
	set_bit_up(DDRB,CP);      //тактовый вход
	set_bit_down(DDRB,MISO);  //выход микросхемы
	  
	set_bit_up(PORTB,PL);
	  
}
void read_HC165()             //чтение из 74HC165
{
	set_bit_down (PORTB,PL);
	set_bit_up(PORTB,PL);
	
	for (int i=0; i < 8; i++)
	{
		data[i] = SPI_Receive();
	}	
}

void read_status_port ()
{
	uint8_t j;
	while(1)
	{
		set_bit_down(DDRB,PL);
		_delay_ms(1);
		set_bit_up(DDRB,PL);
		_delay_ms(1);
		j=SPI_Receive();
		printf("on is",j);
		_delay_ms(100);
	}
	//return 0;
}
int SPI_Receive()
{
	SPDR = 0x00;
	 while(!(SPSR&(1<<SPIF)));
	return SPDR;
}

void enable_ports (int num_mic)
{
	if (num_mic==4)
	{
		port_selection(32 ,1);
		port_selection(31 ,1);
		port_selection(30 ,1);
		port_selection(29 ,1);
		port_selection(28 ,1);
		port_selection(27 ,1);
		port_selection(26 ,1);
		port_selection(25 ,1);
	}
	if (num_mic==3)
	{
		port_selection(24 ,1);
		port_selection(23 ,1);
		port_selection(22 ,1);
		port_selection(21 ,1);
		port_selection(20 ,1);
		port_selection(19 ,1);
		port_selection(18 ,1);
		port_selection(17 ,1);
	}
	if (num_mic==2)
	{
		port_selection(16 ,1);
		port_selection(15 ,1);
		port_selection(14 ,1);
		port_selection(13 ,1);
		port_selection(12 ,1);
		port_selection(11 ,1);
		port_selection(10 ,1);
		port_selection(9 ,1);
	}
	if (num_mic==1)
	{
		port_selection(8 ,1);
		port_selection(7 ,1);
		port_selection(6 ,1);
		port_selection(5 ,1);
		port_selection(4 ,1);
		port_selection(3 ,1);
		port_selection(2 ,1);
		port_selection(1 ,1);
	}
}
void read165transl595 () //читаем из165 выводим 595
{
	char X;
	set_bit_down (PORTB,PL);
	set_bit_up(PORTB,PL);
	SPDR=0;
	while(!(SPSR & (1<<SPIF)));
	X=SPDR;
	_delay_ms(100);
}

