#ifndef SPRITE_H
#define SPRITE_H

#include "ff_rect.h"
#include "ff_linked_list.h"

typedef struct _Sprite
{
	int visible;

	int texture_id;
	Rect slice;

	int pos_x;
	int pos_y;

	//Integer scaling
	int scale;
}Sprite;

extern ff_List global_sprite_list;

Sprite* create_sprite(	const int texture_id, 
						const Rect slice, 
						const int pixel_pos_x, 
						const int pixel_pos_y, 
						const int scale);

void destroy_sprite(const Sprite* sprite);

#endif