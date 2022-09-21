#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "engine.h"
#include "resource_manager.h"

#include "ff_linked_list.h"

#include "sprite.h"

#include <string.h>

ff_List global_sprite_list;

void init_sprite_list()
{
	ff_initialize_list(&global_sprite_list);
}

Sprite* create_sprite(	const int texture_id, 
						const Rect slice, 
						const int pixel_pos_x, 
						const int pixel_pos_y, 
						const int scale)
{
	Sprite sprite;

	sprite.visible = true;
	sprite.texture_id = texture_id;

	sprite.slice = slice;

	sprite.pos_x = pixel_pos_x;
	sprite.pos_y = pixel_pos_y;

	sprite.scale = scale;

	ff_pushback_list(&global_sprite_list, &sprite);

	return ff_get_last_list(&global_sprite_list);
}

void destroy_sprite(const Sprite* sprite)
{
	int index = -1;

	ff_find_list(&global_sprite_list, &index, sprite);

	ff_remove_at_list(&global_sprite_list, index);
}

void destroy_sprite_list()
{
	ff_destroy_list(&global_sprite_list);
}