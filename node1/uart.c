/*
 * uart.c
 *
 * Created: 27.10.2023 16:02:33
 *  Author: jovm
 */ 
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "uart.h"



int uart_transmit(char data, FILE * file) {
	while (! (UCSR0A & (1 << UDRE0)) );
	UDR0 = data;
	return 0;
}

int uart_receive(FILE * file){
	while ( !(UCSR0A & (1 << RXC0)) );
	return UDR0;
}

void uart_init(unsigned int ubrr) {
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
	
	fdevopen(uart_transmit, uart_receive);
}
