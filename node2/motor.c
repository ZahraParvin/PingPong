/*
 * motor.c
 *
 * Created: 14.11.2023 15:29:53
 *  Author: jovm
 */ 
#include "motor.h"
#include "sam.h"
#include "can_interrupt.h"
#include <stdint.h>
#include <stdio.h>

static sum_error = 0;
#define ENCODER_DATA_MASK 0b111111110
#define DO0_IDX 1


void delay_20us(void) {
	for(uint16_t i = 0; i < 1680; i++) {
		asm volatile ("nop");
	}
}

void motor_init(void){
	
	PIOD->PIO_PER |= dir; //Motor direction pin
	PIOD->PIO_PER |= en; //Motor enable
	PIOD->PIO_PER |= sel; //Motor select
	PIOD->PIO_PER |= not_rst; //!Reset
	PIOD->PIO_PER |= not_OE; //not output
	
	PIOD->PIO_OER |= dir; //Motor direction pin
	PIOD->PIO_OER |= en; //Motor enable
	PIOD->PIO_OER |= sel; //Motor select
	PIOD->PIO_OER |= not_rst; //!Reset
	PIOD->PIO_OER |= not_OE; //!Output enable
	

	PIOC->PIO_PER |= PIO_PC1;
	PIOC->PIO_PER |= PIO_PC2;
	PIOC->PIO_PER |= PIO_PC3;
	PIOC->PIO_PER |= PIO_PC4;
	PIOC->PIO_PER |= PIO_PC5;
	PIOC->PIO_PER |= PIO_PC6;
	PIOC->PIO_PER |= PIO_PC7;
	PIOC->PIO_PER |= PIO_PC8;
	
	
	PIOC->PIO_OER |= PIO_PC1;
	PIOC->PIO_OER |= PIO_PC2;
	PIOC->PIO_OER |= PIO_PC3;
	PIOC->PIO_OER |= PIO_PC4;
	PIOC->PIO_OER |= PIO_PC5;
	PIOC->PIO_OER |= PIO_PC6;
	PIOC->PIO_OER |= PIO_PC7;
	PIOC->PIO_OER |= PIO_PC8;
	
	PIOD->PIO_CODR |= not_rst;
	PIOD->PIO_SODR |= not_rst;
	
}



int encoder_read_from_motor() {
	
	PIOD->PIO_CODR |= not_OE;

	
	PIOD->PIO_CODR |= sel;
	delay_20us();
	uint8_t msb = (PIOC->PIO_PDSR & ENCODER_DATA_MASK) >> DO0_IDX;

	PIOD->PIO_SODR |= sel;
	delay_20us();
	uint8_t lsb = (PIOC->PIO_PDSR & ENCODER_DATA_MASK) >> DO0_IDX;

	PIOD->PIO_CODR |= not_rst;
	PIOD->PIO_SODR |= not_rst;

	
	PIOD->PIO_SODR |= not_OE;

	
	uint16_t encoder_data = ((msb << 8) | lsb);
	if (encoder_data & (1 << 15)) {
		return ((uint16_t) (~encoder_data) + 1);
	}
	
	printf("%d\n\r", encoder_data);
	return encoder_data;
}

void pid_controller(uint32_t position) {
	position = 125;
	motor_direction direction;
	float current_position = motor_read_position();
	//printf("current position: %u \n\r", current_position);
	float e = position - current_position;
	sum_error += e;
	
	static float KP = 30;
	static float KI = 0;
	uint32_t speed = (uint32_t)(e*KP+sum_error*KI);
	//printf("speed: %u \n\r", speed);
	if (speed<0){
		direction=LEFT;
	}
	if (speed>0){
		direction=RIGHT;
	}
	//motor_set_direction(RIGHT);
	//motor_set_speed(speed, RIGHT);
}


void set_position_open_loop(uint32_t slider) {

	motor_direction direction;
	if (slider<50){
		direction=LEFT;
	}
	if (slider>50){
		direction=RIGHT;
	}
	motor_set_direction(direction);
	motor_set_speed(2000, direction);	
}


void motor_set_speed(int v, motor_direction d) {
	if (d==LEFT) {
		dac_write(-v);
	}
	if (d==RIGHT) {
		dac_write(+v);
	}
}

static void motor_set_direction(motor_direction d) {
	if (d==LEFT) {
		PIOD->PIO_SODR |= PIO_SODR_P10;
	}
	if (d==RIGHT) {
		PIOD->PIO_SODR |= PIO_CODR_P10;
	}	
}

void motor_enable(void){
	PIOD->PIO_SODR |= en;
}

void solenoid_init(void){
	PIOC->PIO_OER |= PIO_OER_P12; // Pin direction
}

void solenoid_enable(void){
	PIOC->PIO_SODR |= PIO_SODR_P12; // Pin 51 on ATSAM
}

void solenoid_disable(void){
	PIOC->PIO_CODR |= PIO_CODR_P12;
}
