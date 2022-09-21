#include "engine.h"
#include "board.h"

#include "graphics.h"

#include "cursor.h"

#include "input.h"

void init_game()
{
	init_board();
}

int is_piece_selected = false;
Piece* piece_selected;

void game_loop()
{
	if(get_mouse(MOUSE_RIGHT))
	{
		move_viewport(neg_p2(get_mouse_delta()));
	}

	Point2 square = get_square_at(get_mouse_pos());

	move_cursor_to(square);

	if(get_mouse_down(MOUSE_LEFT) && is_piece_selected == false)
	{
		piece_selected = get_piece_at(square);

		if(piece_selected != NULL)
		{
			update_valid_moves(piece_selected);

			if(piece_selected->valid_moves.size > 0)
			{
				show_valid_moves(piece_selected);
				is_piece_selected = true;
			}
		}
	}

	if(get_mouse_down(MOUSE_LEFT) && is_piece_selected == true)
	{
		for(int i = 0; i < piece_selected->valid_moves.size; i ++)
		{
			Point2 valid_move = *(Point2*)ff_get_first_list(&(piece_selected->valid_moves));

			if(equals_p2(square, valid_move))
			{
				move_piece(piece_selected, valid_move);
				hide_valid_moves();
				is_piece_selected = false;
			}

			ff_move_carrousel(&(piece_selected->valid_moves), 1);
		}
	}

	if(get_mouse_down(MOUSE_RIGHT) && is_piece_selected == true)
	{
		hide_valid_moves();
		is_piece_selected = false;
	}
}

void quit_game()
{
	destroy_board();
}
