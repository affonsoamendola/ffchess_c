#ifndef INPUT_H
#define INPUT_H

#include "SDL2/SDL.h"
#include "ff_point2.h"

#define MOUSE_LEFT 		0
#define MOUSE_RIGHT 	1
#define MOUSE_MIDDLE 	2

#define KEY_LEFT	SDL_SCANCODE_LEFT
#define KEY_RIGHT	SDL_SCANCODE_RIGHT
#define KEY_UP		SDL_SCANCODE_UP
#define KEY_DOWN	SDL_SCANCODE_DOWN

#define KEY_W 		SDL_SCANCODE_W
#define KEY_A 		SDL_SCANCODE_A
#define KEY_S 		SDL_SCANCODE_S
#define KEY_D		SDL_SCANCODE_D

#define KEY_ENTER	SDL_SCANCODE_ENTER

void init_input();
void input_update();

char get_key_down(int key_def);
char get_key_up(int key_def);
char get_key(int key_def);

Point2 get_mouse_delta();
Point2 get_mouse_pos();

char get_mouse_down(int button_def);
char get_mouse_up(int button_def);
char get_mouse(int button_def);

#endif