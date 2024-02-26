/*
 * menu.h
 *
 * Created: 02.11.2023 09:42:45
 *  Author: zahrapa
 */ 


#ifndef MENU_H_
#define MENU_H_

typedef struct{
	char* selection;
	struct menu_t* parent;
	struct menu_t* first_child;
	struct menu_t* right_sibling;
	} menu_t;
	



menu_t* new_menu(char* title, menu_t* parent);

void right_sibling(menu_t* menu, menu_t* right_sibling);
void first_child(menu_t* menu, menu_t* first_child);

menu_t* menu_init();



void print_name(char* title);

void oled_print_title(char c[]);
void menu_print_test();
void menu_print(menu_t *menu);
void draw_square();
void draw_dot();
void undraw_dot();
void oled_menu_selection();

bool select_menu (int button,int current_line);

bool run_menu();
#endif /* MENU_H_ */
