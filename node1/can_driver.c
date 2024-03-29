/*
 * can_driver.c
 *
 * Created: 31.10.2023 15:17:19
 *  Author: jovm
 */ 

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "mcp2515.h"
#include "can_driver.h"

void can_init() {
	mcp_init();
}




void can_send(message_ptr message) {
	mcp_write(MCP_TXB0SIDH, message->id / 8);
	mcp_write(MCP_TXB0SIDL, (message->id % 8) << 5);
	mcp_write(MCP_TXB0DLC, message->length);

	
	for (int i = 0; i < message->length; i++) {
		mcp_write(MCP_TXB0D0 + i, message->data[i]);
	}

	mcp_request_to_send(0);

}


message_t can_receive() {
	
	message_t message = {};

	
	uint8_t id_low = mcp_read(MCP_RXB0SIDL)/0b100000;
	uint8_t id_high = mcp_read(MCP_RXB0SIDH);
	message.id = id_high * 0b1000 + id_low;

	
	message.length = mcp_read(MCP_RXB0DLC);

	
	for (int i = 0; i < message.length; i++) {
		message.data[i] = mcp_read(MCP_RXB0D0 + i);
	}

	return message;
}
