/*
 * IncFile1.h
 *
 * Created: 31.10.2023 15:18:28
 *  Author: jovm
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_


void can_init();

typedef struct Message {
	unsigned int id;
	uint8_t length;
	char data[8];
} message_t, *message_ptr;

void can_send(message_ptr);

message_t can_receive();



#endif /* INCFILE1_H_ */
