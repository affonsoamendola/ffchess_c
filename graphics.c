#include "engine.h"

#include "display.h"
#include "chess.h"
#include "board.h"
#include "effect.h"

#include "mouse.h"

Effect rainbow_effect;
Effect breathe_effect;

void init_graphics()
{
	breathe_effect = create_effect_breathe(0.6, 1.0, 0.6);
	rainbow_effect = create_effect_rainbow(0.25, 200);
}

void draw_cursor()
{
	float intensity = 0.0;
	int intensity_int = 0;

	Color intensity_color;

	update_effect(&breathe_effect, &intensity);

	intensity_int = (int)(intensity * 255.0f);

	intensity_color = color(intensity_int, intensity_int, intensity_int, 255);

	draw_vertical_line(get_mouse_x(), get_mouse_y() + 2, get_mouse_y() + 5, intensity_color);
	draw_vertical_line(get_mouse_x(), get_mouse_y() - 5, get_mouse_y() - 2, intensity_color);
	draw_horizontal_line(get_mouse_x() + 2, get_mouse_x() + 5, get_mouse_y(), intensity_color);
	draw_horizontal_line(get_mouse_x() - 5, get_mouse_x() - 2, get_mouse_y(), intensity_color);
}

void draw_board()
{
	Point2 board_start = get_background_start();

	blit_hardware(0, rect(0, 0, get_texture_w(0), get_texture_h(0)), point2(board_start.x, board_start.y), point2(1,1));
}

void draw_header()
{
	update_effect(&rainbow_effect, NULL);

	fill_rectangle(rect(0,0, get_current_res().x, 18), color(0,0,0,220));

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
	clear_screen();

	draw_string_f(point2(0,0), 0, color(255,255,255,255), "Foffonso's Chess");
	draw_string_f(point2(0,10), 2, color(255,255,255,255), "Enjoy your stay :)");

	draw_string_f(point2(290,10), 2, color(255,255,255,255), "ver %i.%i", VER_MAJOR, VER_MINOR);
}

void draw_piece(Point2 pixel_pos, PIECE piece)
{

	blit_hardware(1, rect((piece.type-1) * get_square_size(), piece.team * get_square_size(), get_square_size(), get_square_size()), pixel_pos, point2(1,1));
}

void draw_pieces()
{
	Point2 board_start = get_board_start();

	for(int x = 0; x < get_board_size().x; x++)
	{
		for(int y = 0; y < get_board_size().y; y++)
		{
			PIECE current_piece = get_board_piece(x, y);

			if(current_piece.type != NO_PIECE)
			{
				draw_piece(	sum_p2(	board_start, 
									point2(x * get_square_size(), y * get_square_size())
								  ), 
							current_piece
				          );
			}
		}
	}
}

void draw_game()
{
	draw_board();
	draw_pieces();
	draw_header();
	draw_cursor();

	flush_pixels();
}