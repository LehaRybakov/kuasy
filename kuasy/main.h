/*
 * kuasy.c
 *
 * Created: 12.10.2022 13:24:59
 * Author : a.rybakov
 */ 
#include <avr/io.h>
#define  F_CPU 16000000UL                                                                                     
#include <util/delay.h>
#include <stdio.h>
//HC165
#define CP                  PB7  //тактовый вход
#define CE                  PB0  //вход тактирования
#define PL                  PB1  //защелка
#define MISO                PB6  //выход 165

//HC595
#define EN_OUT              PC5
#define RST_OUT             PC7
#define LATCH               PC6  //линия синхронизации,защелка
#define MOSI                PB5  //линия передачи
#define SCK                 PB7  //линия тактирования
#define SS                  PB4  

#define set_bit_up(x,y)        x|=(1<<y)   //установка бита лог1
#define set_bit_down(x,y)      x&= ~(1<<y) //установка бита лог0

#define DDR_STATUS_LED		DDRD
#define PORT_STATUS_LED		PORTD
#define STATUS_LED			PIND7
//led
#define SPI_PORT            PORTB
#define SPI_DDR             DDRB

#define COUNT_HC595	4
#define COUNT_HC165 8

#define LED_4	4
#define LED_3	3
#define LED_2	2
#define LED_1	1
#define LED_0	0

unsigned char data[8];      //храним данные 74HC165 здесь
char X;
volatile unsigned char sendData = 0; 

uint8_t dataOutput[COUNT_HC595];

void port_selection(int num_led, int status);
void init_PORT();
void init();
void initUART();
int  send(char c, FILE *stream);
void init_SPI();
void send_SPI(uint8_t data);
void setOutput(uint8_t *data);
void initSPI_for_HC595();
void clearOutputData();

void initSPI_for_HC165();
void read_status_port();
int SPI_Receive();

void read_HC165();
void read165transl595 ();

static FILE uart = FDEV_SETUP_STREAM(send, NULL, _FDEV_SETUP_WRITE);


