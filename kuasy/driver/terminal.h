/*
 * terminal.h
 *
 * Created: 14.10.2022 11:58:41
 *  Author: b.ulyashev
 */ 


#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <stdio.h>
#include <avr/io.h>

extern int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

#endif /* TERMINAL_H_ */