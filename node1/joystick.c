/*
 * joystick.c
 *
 * Created: 27.10.2023 16:06:46
 *  Author: jovm
 */ 
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "uart.h"
#include "sram.h"
#include "joystick.h"

uint8_t max_v_joy = 255 ;
uint8_t min_v_joy = 0;

uint8_t max_v_slider = 255 ;
uint8_t min_v_slider = 109;

void adc_init() {
	MCUCR = (1 << SRE);
	SFIOR = (1 << XMM2);
}


int joy_percent(int value, int zero) {
	int percent_value;
	if(value > zero) {
		percent_value = (( value - zero ) * 100) / (max_v_joy - zero);
	}
	else if (value < zero) {
		percent_value = (( value - zero - 50 ) * 100) / (zero - min_v_joy);
	}
	else {
		percent_value = 0;
	}
	return percent_value;
}

uint8_t read_right_button(void) {
	return (PINB & (1<<PINB1)) >> PINB1;
}

void control_refresh () {
	DDRE &= ~(1 << PE0);
	volatile char *adc = (char *) 0x1400;
	adc[0] = 0x11110000; 
	 _delay_us(20);
	 
	uint8_t value_x = adc[0];
	uint8_t value_y = adc[1];
	
	pos_joy.x = value_x;
	pos_joy.y = value_y;
	
	pos_joy.per_x = joy_percent(pos_joy.x,pos_joy.x_zero);
	pos_joy.per_y = joy_percent(pos_joy.y,pos_joy.y_zero);
	
	uint8_t value_left = (( adc[2] -min_v_slider) *100) / (max_v_slider - min_v_slider);
	uint8_t value_right = (( adc[3] - min_v_slider) *100) / (max_v_slider - min_v_slider);
	slider.left = value_left;
	slider.right = value_right;
}



void pos_calibrate() {
	DDRE &= ~(1 << PE0);
	volatile char *adc = (char *) 0x1400;
	adc[0] = 0x11000000;
	uint8_t value_x;
	uint8_t value_y;
	_delay_us(20);
	printf("Starting calibration...\n\r");
	_delay_ms(5000);
	printf("Don't touch anything...\n\r");
	_delay_ms(2000);
	value_x = adc[0];
	value_y = adc[1];
	pos_joy.x_zero = value_x;
	pos_joy.y_zero = value_y;
// 	_delay_ms(1000);	
// 	printf("Now please push down the joy stick...\n\r");
// 	_delay_ms(10000);
// 	value_x = adc[0];
// 	pos_joy.min_x = value_x;
// 	_delay_ms(5000);
// 	
// 	printf("Now please push left the joy stick...\n\r");
// 	_delay_ms(10000);
// 	value_x = adc[1];
// 	pos_joy.min_y = value_y;
// 	_delay_ms(2000);
// 	
// 	printf("calibration complete...\n\r");	
}

pos_t pos_read() {
	pos_t position;
	position.x = pos_x;
	position.y = pos_y;
	return position;
}

joy_direction pos_direction (pos_t position) {
	int dead_zone = 50;
	
	if (abs(position.per_x) > abs(position.per_y)  && position.x < (position.x_zero + dead_zone) ) {
		return LEFT;
	}
	else if (abs(position.per_x) > abs(position.per_y) && position.x > (position.x_zero + dead_zone)) {
		return RIGHT;
	}
	else if (abs(position.per_x) < abs(position.per_y) && position.y > (position.y_zero + dead_zone) ) {
		return UP;
	}
	else if (abs(position.per_x) < abs(position.per_y) && position.y < (position.y_zero + dead_zone)) {
		return DOWN;
	}
	else {
		return NEUTRAL;
	}
}



void run_multiboard(void) {

	//Joystick

	pos_t position;
	joy_direction direction;
	slider_t slider;
	
	adc_init();
	pos_calibrate();
	while(1){

		control_refresh();
		_delay_ms(1000);
		//printf("Joystick: \n\r %d \n\r %d \n\r", pos_joy.x, pos_joy.y);
		
		uint8_t right_button = read_right_button();
		printf("Button: %d \n\r", right_button);
		
		//printf("slider  %d : %d \n\r", slider.left, slider.right);
		//printf("slider right: %d \n\r", pos_joy.y);

		//joy_d = pos_direction(pos_joy);

		//printf("\nDirection: %d\n\n\r", joy_d);

		//printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");

		//_delay_ms(1000);
	}
}