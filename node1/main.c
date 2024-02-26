/*
 * bobthebuilder.c
 *
 * Created: 30.10.2023 14:43:12
 * Author : jovm
 */ 

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>  

#include "uart.h"
#include "sram.h"
#include "joystick.h"
#include "oled.h"
#include "mcp2515.h"
#include "spi.h"
#include "can_driver.h"
#include "menu.h"



int main(void){
	uart_init(MYUBRR);
	printf("\r\n");
	printf("Connected!\n\r");
	adc_init();
	SRAM_init();
	
	oled_init();
	/*	bool game = run_menu();*/

// 	 	bool game_on = false;
// 	 	while(!game_on) {
// 	
// 	 		menu_print_test();
// 	 		if ( pos_direction(pos_joy)!= NEUTRAL) {
// 	 			oled_menu_selection();
// 	 		}
// 	 	}
	 	int sel_line = 3;
	 	menu_print_test();
	 	oled_set_pos(sel_line, 4);
	 	draw_dot();
	
	can_init(); 
	mcp_set_mode(MODE_NORMAL);
	pos_calibrate();
	
	while (1)
	{

 		int joystick_x = pos_joy.per_x + 100;
		uint8_t right_button = read_right_button();
		control_refresh();
		if( pos_joy.per_y > 80) {

			oled_set_pos(sel_line, 4);
			undraw_dot();
			sel_line--;
			if (sel_line <= 3) sel_line = 3;

			oled_set_pos(sel_line, 4);
			draw_dot();
		}
		if(pos_joy.per_y < -80) {
			oled_set_pos(sel_line, 4);
			undraw_dot();
			sel_line++;
			if (sel_line >= 7) sel_line = 7;
			oled_set_pos(sel_line, 4);
			draw_dot();
		}
		bool game = select_menu (right_button,sel_line);
		printf("pos: %d \n\r", slider.right);
		_delay_ms(100);
		message_t joy_pos = {
			1, // Id
			2, // Length of data
			{joystick_x,slider.right}
		}; 
		can_send(&joy_pos); 

	
// 		message_t slider_pos = {
// 			0, // Id
// 			1, // Length of data
// 			slider.right
// 		};
// 		can_send(&slider_pos);	
		
// 		message_t button_can = {
// 			3, // Id
// 			1, // Length of data
// 			right_button
// 		};
// 		can_send(&button_can);
	}
	return 0;
}	



	
	

	// 		printf("Joystick: \n\r %d \n\r %d \n\r", pos_joy.per_x, pos_joy.per_y);
	// 		//printf("Joystick: \n\r %d \n\r %d \n\r", slider.left, slider.right);
	// 		//printf("direction: %d\n\r",direction);