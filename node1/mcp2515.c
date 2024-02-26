/*
 * mcp2515.c
 *
 * Created: 31.10.2023 15:13:16
 *  Author: jovm
 */ 

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "mcp2515.h"
#include "spi.h"


static void interrupt_can_recieve_init() {
	DDRD &= ~(1 << PD2);
	GICR |= (1 << INT0);
	MCUCR |= (1 << ISC01);
	MCUCR &= ~(1 << ISC00);
}



void mcp_reset() {
	spi_clear_ss();
	PORTB &= ~(1 << DD_SS);
	spi_write(MCP_RESET);
	PORTB |= (1 << DD_SS);
	spi_set_ss();
}

uint8_t mcp_read(uint8_t address) {
	spi_clear_ss();
	spi_write(MCP_READ);
	spi_write(address);
	uint8_t data = spi_read();
	spi_set_ss();

	return data;
}
void mcp_init() {
	interrupt_can_recieve_init();
	spi_master_init();
	//printf("Before reset: %x\r\n",mcp_read(MCP_CANSTAT));
	mcp_reset();
	//printf("After reset: %x\r\n",mcp_read(MCP_CANSTAT));
	_delay_ms(3);

	
	uint8_t value = mcp_read(MCP_CANSTAT);
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 is not in CONFIG-mode after reset. CANSTAT: %x \r\n", value);
	}
	
	uint8_t BRP = F_CPU / (2 * NUMBER_OF_TQ * BAUDRATE);

	mcp_write(MCP_CNF1, SJW4 | (BRP - 1));
	mcp_write(MCP_CNF2, BTLMODE | SAMPLE_3X | ((PS1 - 1) << 3) | (PROPAG - 1));
	mcp_write(MCP_CNF3, WAKFIL_DISABLE | (PS2 - 1));

}


void mcp_write(uint8_t address, uint8_t data) {
	spi_clear_ss();
	spi_write(MCP_WRITE);
	spi_write(address);
	spi_write(data);
	spi_set_ss();
}

char mcp_read_status() {
	spi_clear_ss();
	spi_write(MCP_READ_STATUS);
	char data = spi_read();
	spi_set_ss();

	return data;
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	spi_clear_ss();
	spi_write(MCP_BITMOD);
	spi_write(address);
	spi_write(mask);
	spi_write(data);
	spi_set_ss();
}


void mcp_request_to_send(int buffer_number) {
	spi_clear_ss();
	buffer_number = buffer_number % 3;
	char data = MCP_RTS_TX0;
	if (buffer_number == 0) {
		data = MCP_RTS_TX0;
		} else if (buffer_number == 1) {
		data = MCP_RTS_TX1;
		} else if (buffer_number == 2) {
		data = MCP_RTS_TX2;
	}
	spi_write(data);
	spi_set_ss();
}

void mcp_request_to_send_buffer0() {
	spi_clear_ss();
	char data = MCP_RTS_TX0;
	spi_write(data);
	spi_set_ss();
}


void mcp_set_mode(uint8_t mode) {
	mcp_bit_modify(MCP_CANCTRL, 0b11100000, mode);
}
