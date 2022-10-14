/*
 * terminal.c
 *
 * Created: 14.10.2022 11:58:29
 *  Author: b.ulyashev
 *	In main.c add
 *	stdout = &mystdout;
 *	
 *	after that you can use printf*  from standart library
 *	printf("Hello world!!\n");
 *	printf("Your age is %d", 25);


 */ 
 #include "terminal.h"


 int uart_putchar(char c, FILE *stream)
 {
	 if (c == '\n')
	 uart_putchar('\r', stream);
	 while( !(UCSRA&(1 << UDRE)) );
		UDR = c;
	 return 0;
 }
