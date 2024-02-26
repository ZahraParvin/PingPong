/*
 * uart.h
 *
 * Created: 27.10.2023 16:02:22
 *  Author: jovm
 */ 


#ifndef UART_H_
#define UART_H_

#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1



int uart_transmit(char data, FILE * file);

int uart_receive(FILE * file);

void uart_init(unsigned int ubrr);


#endif /* UART_H_ */
