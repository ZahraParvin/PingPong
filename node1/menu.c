/*
 * menu.c
 *
 * Created: 02.11.2023 09:42:33
 *  Author: zahrapa
 */ 

#include <string.h>
#include <stdio.h>
#include <stdbool.h>  
#include <util/delay.h>

#include "uart.h"
#include "sram.h"
#include "joystick.h"
#include "oled.h"
#include "menu.h"

volatile menu_t* current;
volatile int current_menu_size;
volatile int current_line = 3;
menu_t* new_menu(char* title, menu_t* parent) {
	
	menu_t* new_menu;
	
	new_menu->selection = title;
	new_menu->parent = parent;
	new_menu->first_child = NULL;
	new_menu->right_sibling = NULL;
		
	return new_menu;
}

void right_sibling(menu_t* menu, menu_t* right_sibling) {
	menu->right_sibling =right_sibling;
}

void first_child(menu_t* menu, menu_t* first_child) {
	menu->first_child =first_child;
}


menu_t* menu_init() {
	menu_t* main = new_menu("Main menu", NULL);
	
	menu_t* new_game = new_menu("New game", main);
    menu_t* calibration = new_menu("Calibration",main);

	menu_t* credits = new_menu("Credits",main);

    menu_t* byggern_credits = new_menu("Made by Z and J.", credits);
	
	first_child(main, new_game);
	right_sibling(new_game, calibration);
    right_sibling(calibration, credits);
	
 	current = main;
	return current;
	
}



void print_name(char* title){
    enter_column(0);
    enter_line(0);
	oled_print_string(title);
	enter_column(0);
    enter_line(1);
	for (int i = 0; i < 32; i++) {
		oled_print_char("-");
	}
	enter_column(0);
    enter_line(2);
}


void oled_print_title(char c[]) {
	enter_column(40);
    enter_line(0);
	for (int i = 0; i < strlen(c); i++) {
		oled_print_char_big(c[i]);
	}
}

void menu_print_test(){
	oled_print_title("Menu");
	
	oled_set_pos(3, 16);
	oled_print_string("New game");
	oled_set_pos(5, 16);
	oled_print_string("Calibration");
	oled_set_pos(7, 16);
	oled_print_string("Credits");
}

void menu_print(menu_t *menu) {

    
	clear_oled_display();
	oled_print_title(menu->selection);

    	
	int line = 2;
	
	menu = menu->first_child;
	
	while (menu != NULL && line < 7) {

		enter_line(line);
		enter_column(15);
		oled_print_string(menu->selection);
	
		line++;
		menu = menu->right_sibling;
	}
	
}
void draw_square(){
	write_data_to_oled(0b11111111);
	write_data_to_oled(0b11111111);
	write_data_to_oled(0b11000011);
	write_data_to_oled(0b11000011);
	write_data_to_oled(0b11000011);
	write_data_to_oled(0b11000011);
	write_data_to_oled(0b11000011);
	write_data_to_oled(0b11000011);
	write_data_to_oled(0b11111111);
	write_data_to_oled(0b11111111);
}

void draw_dot(){
	write_data_to_oled(0b00111100);
	write_data_to_oled(0b00111100);
	write_data_to_oled(0b00111100);
	write_data_to_oled(0b00111100);
}
void undraw_dot(){
	write_data_to_oled(0b00000000);
	write_data_to_oled(0b00000000);
	write_data_to_oled(0b00000000);
	write_data_to_oled(0b00000000);
}

void oled_menu_selection() {
	
	joy_direction direction = pos_direction(pos_joy);
	
	if (direction == UP) {
		current_line--;
		if (current_line < 3) {
			current_line = 3;
		}
		enter_line(current_line);
		enter_column(1);
		draw_square();
	}
	else if (direction == DOWN) {
		current_line++;
		if (current_line >= 7) {
			current_line = 7;
		}
		
		enter_line(current_line);
		enter_column(1);
		draw_square();
	}
	
}

bool select_menu (int button, int current_line) {
	bool game_on = false;
	if (button == 1) {
		if (current_line==3) {
			game_on = 1;
			clear_oled_display();
			enter_line(4);
			enter_column(50);
			oled_print_string("play!");
			
		}
		else if(current_line==5){
			clear_oled_display();
			enter_line(4);
			enter_column(10);
			oled_print_string("wait 5 sec!");
			pos_calibrate();
			_delay_ms(25000);
			clear_oled_display();
			menu_print_test();

		}
		else if (current_line==7){
			clear_oled_display();
			enter_line(4);
			enter_column(0);
			oled_print_string("Made by Z and J!");
			_delay_ms(25000);
			clear_oled_display();
			menu_print_test();
		}
        //menu_t* selected_menu = current;
	    // selected_menu = current->first_child;
	    // for (int i=0; i < current_line; i++) {
		//     selected_menu = selected_menu->right_sibling;
	    // }
	}
	return game_on;
}

// bool run_menu(){
// 		menu_t* current = menu_init();
// 		bool game_on = false;
// 		while(!game_on) {
// 	
// 			menu_print_test(current);
// 			if ( pos_direction(pos_joy)!= NEUTRAL) {
// 				oled_menu_selection();
// 			}			
//  			game_on = select_menu (read_right_button());
//  			_delay_ms(3000);
// 		}
// 		return game_on;
// }
