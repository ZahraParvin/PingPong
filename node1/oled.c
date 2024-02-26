/*
 * oled.c
 *
 * Created: 27.10.2023 16:10:01
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
#include "fonts.h"
#include "oled.h"


void write_command_to_oled(uint8_t command){
	volatile char *oled_commmand_addr = (char*) 0x1000; //address to oled commands
	oled_commmand_addr[0] = command;
}

void write_data_to_oled(uint8_t data){
	volatile char *oled_data_addr = (char*) 0x1200; //address to oled data
	oled_data_addr[0] = data;
}

void oled_init(){
	write_command_to_oled(0xae); // display off
	write_command_to_oled(0xa1); //segment remap
	write_command_to_oled(0xda); //common pads hardware: alternative
	write_command_to_oled(0x12);
	write_command_to_oled(0xc8); //common output scan direction:com63~com0
	write_command_to_oled(0xa8); //multiplex ration mode:63
	write_command_to_oled(0x3f);
	write_command_to_oled(0xd5); //display divide ratio/osc. freq. mode
	write_command_to_oled(0x80);
	write_command_to_oled(0x81); //contrast control
	write_command_to_oled(0x50);
	write_command_to_oled(0xd9); //set pre-charge period
	write_command_to_oled(0x21);
	write_command_to_oled(0x20); //Set Memory Addressing Mode
	write_command_to_oled(0x02);
	write_command_to_oled(0xdb); //VCOM deselect level mode
	write_command_to_oled(0x30);
	write_command_to_oled(0xad); //master configuration
	write_command_to_oled(0x00);
	write_command_to_oled(0xa4); //out follows RAM content
	write_command_to_oled(0xa6); //set normal display
	write_command_to_oled(0xaf); // display on
	
	clear_oled_display();
}

void page_addressing_mode_init(){
	write_command_to_oled(0x20);
	write_command_to_oled(0b10);
}

void vertical_adressing_mode_init(){
	write_command_to_oled(0x20);
	write_command_to_oled(0b01);
}

void horizontal_adressing_mode_init(){
	write_command_to_oled(0x20);
	write_command_to_oled(0b00);
}

void enter_column(uint8_t column) {
// 	if (0 <= column && column <= 127) {
// 		// Set lower 4 bits
// 		write_command_to_oled(0x00 + (column & 0x0F));
// 
// 		// Set upper 4 bits
// 		write_command_to_oled(0x10 + ((column & 0xF0) >> 4));
// 	}

	write_command_to_oled(0b00000000 | (column & 0b1111));
	write_command_to_oled(0b00010000 | (column >> 4));

}

void enter_line(uint8_t line){
// 	if (0 <= line && line <=7) {
// 		write_command_to_oled(0xB0 + line);
// 	}

	write_command_to_oled(0b10110000 | line);
	
}


void oled_set_pos(int line, int col) {
	enter_line(line);
	enter_column(col);
}


void clear_oled_display() {
	for (int page = 0; page < 8; page++) {
		oled_set_pos(page, 0);
		for (int col = 0; col < 128; col++) {
			write_data_to_oled(0x00);
		}
	}
}
// Function to clear the OLED display
// void clear_oled_display() {
// 	for (int line = 0; line < 8; line++) {
// 		enter_line(line);
// 		enter_column(0);
// 		for (int i = 0; i < 128; i++) {
// 			write_data_to_oled(0x00);
// 		}
// 	}
// }

void write_one_to_every_pixel() {
	for (int line = 0; line < 8; line++) {
		enter_line(line);
		enter_column(0);
		//write_data_to_oled(0x01);
		for (int i = 0; i < 128; i++) {
			write_data_to_oled(0xff);
		}
	}
}

void white_screen() {
	write_command_to_oled(0xa5);
}



void draw_snake(){
	write_data_to_oled(0b11000000);
	write_data_to_oled(0b01100000);
	write_data_to_oled(0b00110000);
	write_data_to_oled(0b00011000);
	write_data_to_oled(0b00001100);
	write_data_to_oled(0b00000110);
	write_data_to_oled(0b00000011);
	write_data_to_oled(0b00000011);
	write_data_to_oled(0b00000110);
	write_data_to_oled(0b00001100);
	write_data_to_oled(0b00011000);
	write_data_to_oled(0b00110000);
	write_data_to_oled(0b01100000);
	write_data_to_oled(0b11000000);
	write_data_to_oled(0b11000000);
	write_data_to_oled(0b01100000);
	write_data_to_oled(0b00110000);
	write_data_to_oled(0b00011000);
	write_data_to_oled(0b00001100);
	write_data_to_oled(0b00000110);
	write_data_to_oled(0b00000011);
	write_data_to_oled(0b00000011);
	write_data_to_oled(0b00000110);
	write_data_to_oled(0b00001100);
	write_data_to_oled(0b00011000);
	write_data_to_oled(0b00110000);
	write_data_to_oled(0b01100000);
	write_data_to_oled(0b11000000);
	write_data_to_oled(0b11000000);
	write_data_to_oled(0b01100000);
	write_data_to_oled(0b00110000);
	write_data_to_oled(0b00011000);
	write_data_to_oled(0b00001100);
	write_data_to_oled(0b00000110);
	write_data_to_oled(0b00000011);
	write_data_to_oled(0b00000011);
	write_data_to_oled(0b00000110);
	write_data_to_oled(0b00001100);
	write_data_to_oled(0b00011000);
	write_data_to_oled(0b00110000);
	write_data_to_oled(0b01100000);
	write_data_to_oled(0b11000000);

	
}

void moving_snake(int line){
	for (int i=0;i<(128-42);i++){

		enter_line(line);
		enter_column(i);
		draw_snake();
		_delay_ms(500);
		clear_oled_display();
	}
}

void oled_print_char_big(char c) {
	c -=32;
	for (int i = 0; i < 5; i++) {
		int byte = pgm_read_byte(&font5[c][i]);
		//printf('%i', (int)c);
		write_data_to_oled(byte);
	}
}

void oled_print_char(char c) {
	c -=32;
	for (int i = 0; i < 4; i++) {
		int byte = pgm_read_byte(&font4[c][i]);
		//printf('%i', (int)c);
		write_data_to_oled(byte);
	}
}

void oled_print_string(char c[]) {
	for (int i = 0; i < strlen(c); i++) {
		oled_print_char(c[i]);
	}
}