/*
 * spi.c
 *
 * Created: 27.10.2023 16:16:42
 *  Author: jovm
 */ 

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "uart.h"
#include "mcp2515.h"
#include "spi.h"



// Setter SS høy. Lar alt annet på PORTB være som det var.
void spi_set_ss() {
	PORTB |= (1 << DD_SS);
}
// Setter SS lav. Lar alt annet på PORTB være som det var.
void spi_clear_ss() {
	PORTB &= ~(1 << DD_SS);
}


void spi_master_init() {
	DDRB |= (1 << PB4) | (1 << PB5) | (1 << PB7);
	DDRB &= ~(1 << PB1) & ~(1 << PB2) & ~(1 << PB3) & ~(1 << PB6);

	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

	
	PORTB |= (1 << PB4);

}

void spi_write(char cData) {
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF))) {
	}
}


uint8_t spi_read() {
	SPDR = 0xff;
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF))) {
	}
	/* Return data register */
	return SPDR;
}
