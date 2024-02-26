/*
 * oled.h
 *
 * Created: 27.10.2023 16:10:18
 *  Author: jovm
 */ 


#ifndef OLED_H_
#define OLED_H_


void write_command_to_oled(uint8_t command);

void write_data_to_oled(uint8_t data);

void oled_init();

void page_addressing_mode_init();

void vertical_adressing_mode_init();

void horizontal_adressing_mode_init();

void enter_column(uint8_t column);
void enter_line(uint8_t line);
void oled_set_pos(int line, int col);

// Function to clear the OLED display
void clear_oled_display();

void write_one_to_every_pixel();
void white_screen();



void draw_snake();
void moving_snake(int line);
void oled_print_char_big(char c);
void oled_print_char(char c);
void oled_print_string(char c[]);



#endif /* OLED_H_ */