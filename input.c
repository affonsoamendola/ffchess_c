#include <stdlib.h>

#include "stdio.h"
#include "SDL2/SDL.h"

#include "engine.h"
#include "cursor.h"

#include "display.h"

SDL_Event input_event;

char key_down[284];
char key[284];
char key_up[284];

char mouse_down[3];
char mouse[3];
char mouse_up[3];

Point2 mouse_raw_pos_last = {0, 0};
Point2 mouse_raw_pos_current = {0, 0};

char get_key_down(int key_def)
{
	return key_down[key_def]; 
}

char get_key_up(int key_def)
{
	return key_up[key_def];
}

char get_key(int key_def)
{
	return key[key_def];
}

Point2 get_mouse_pos()
{
	Point2 current = {0, 0};

	current = mouse_raw_pos_current;

	current.x = current.x/get_pixel_scale();
	current.y = current.y/get_pixel_scale();

	return current;
}

Point2 get_mouse_delta()
{
	Point2 current = {0, 0};
	Point2 last = {0, 0};

	current = get_mouse_pos();

	last = mouse_raw_pos_last;
	last.x = last.x / get_pixel_scale();
	last.y = last.y / get_pixel_scale();

	Point2 delta = sub_p2(current, last);

	return delta;
}

char get_mouse_down(int button_def)
{
	return mouse_down[button_def];
}

char get_mouse_up(int button_def)
{
	return mouse_up[button_def];
}

char get_mouse(int button_def)
{
	return mouse[button_def];
}

void init_input()
{
	//Set everything to zero
	memset(key, 	 0, 284);
	memset(mouse, 	 	0, 3);

	memset(key_down, 0, 284);
	memset(key_up, 	 0, 284);

	memset(mouse_down, 	0, 3);
	memset(mouse_up, 	0, 3);

	//Set mouse to current position
	SDL_GetMouseState(&(mouse_raw_pos_current.x), &(mouse_raw_pos_current.y));
	
	//Set last to current position as well, so the first frame has mouse delta = 0
	mouse_raw_pos_last.x = mouse_raw_pos_current.x;
	mouse_raw_pos_last.y = mouse_raw_pos_current.y;
}

void input_update()
{
	memset(key_down, 0, 284);
	memset(key_up, 	 0, 284);

	memset(mouse_down, 	0, 3);
	memset(mouse_up, 	0, 3);

	mouse_raw_pos_last.x = mouse_raw_pos_current.x;
	mouse_raw_pos_last.y = mouse_raw_pos_current.y;
	SDL_GetMouseState(&(mouse_raw_pos_current.x), &(mouse_raw_pos_current.y));

	while(SDL_PollEvent(&input_event))
	{
		if(	input_event.type == SDL_KEYDOWN)
		{
			if(input_event.key.repeat == 0) 
			{
				key_down[input_event.key.keysym.scancode] = true;
				key[input_event.key.keysym.scancode] = true;
			}
		}

		if(	input_event.type == SDL_KEYUP)
		{
			if(input_event.key.repeat == 0) 
			{
				key_up[input_event.key.keysym.scancode] = true;
				key[input_event.key.keysym.scancode] = false;
			}
		}

		if( input_event.type == SDL_MOUSEBUTTONDOWN)
		{
			switch(input_event.button.button)
			{
				case SDL_BUTTON_LEFT:
					mouse_down[0] = true;
					mouse[0] = true;
					break;
				case SDL_BUTTON_RIGHT:
					mouse_down[1] = true;
					mouse[1] = true;
					break;
				case SDL_BUTTON_MIDDLE:
					mouse_down[2] = true;
					mouse[2] = true;
					break;
			}		
		}

		if( input_event.type == SDL_MOUSEBUTTONUP)
		{
			switch(input_event.button.button)
			{
				case SDL_BUTTON_LEFT:
					mouse_up[0] = true;
					mouse[0] = false;
					break;
				case SDL_BUTTON_RIGHT:
					mouse_up[1] = true;
					mouse[1] = false;
					break;
				case SDL_BUTTON_MIDDLE:
					mouse_up[2] = true;
					mouse[2] = false;
					break;
			}		
		}


		if( input_event.type == SDL_QUIT )
		{
			signal_quit();
			break;
		}
	}
}