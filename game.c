#include "engine.h"
#include "gfx.h"
#include "effect.h"
#include "resource_manager.h"
#include "chess.h"
#include "board.h"

Effect rainbow_effect;

void init_game()
{
	rainbow_effect = create_effect_rainbow(0.25, 200);

	init_board();
}

void quit_game()
{
	free_board();
}

void draw_header()
{
	update_effect(&rainbow_effect, NULL);

	draw_string_f(point2(0,0), 0, color(255,255,255,255), "Foffonso's Chess");
	draw_string_f(point2(0,10), 2, color(255,255,255,255), "Enjoy your stay :)");

	draw_string_f(point2(290,10), 2, color(255,255,255,255), "ver %i.%i", VER_MAJOR, VER_MINOR);

	//Draws the rainbow strip at the bottom
	Color rainbow_strip_color;
	for(int i = 0; i < get_current_res().x; i++)
	{
		//temp is the angle offset of the effect, 
		float temp = (i*2.0*M_PI)/get_current_res().x; 
		//gets the current state of the effect offset by temp and puts it in rainbow_strip_color
		get_effect(&rainbow_effect, &temp, &rainbow_strip_color);
		//Draw current pixel of the rainbow strip
		set_pixel(i, 18, rainbow_strip_color);
	}

	flush_pixels();
}

void draw_piece(Point2 pixel_pos, PIECE piece)
{
	blit_hardware(1, rect((piece.type-1) * get_square_size(), piece.team * get_square_size(), get_square_size(), get_square_size()), pixel_pos, point2(1,1));
}

void draw_pieces()
{
	for(int x = 0; x < get_board_size().x; x++)
	{
		for(int y = 0; y < get_board_size().y; y++)
		{
			PIECE current_piece = get_board_piece(x, y);

			if(current_piece.type != NO_PIECE)
			{
				draw_piece(	point2( get_current_res().x/2 - (get_square_size() * get_board_size().x)/2 + x * get_square_size() ,
								get_current_res().y/2 - (get_square_size() * get_board_size().y)/2 + y * get_square_size()),
							current_piece);
			}
		}
	}
}

void draw_game()
{
	draw_board();
	draw_pieces();
	draw_header();
}