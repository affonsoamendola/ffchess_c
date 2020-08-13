#include <stdlib.h>
#include <string.h>

#include "chess.h"
#include "board.h"
#include "gfx.h"

#include "ff_rect.h"

int square_size = 20;
int get_square_size(){return square_size;}

void set_board_piece(int x, int y, PIECE_TYPE type, TEAM team)
{
	board_pieces[x + y * get_board_size().x] = (PIECE){team, type};
}

int is_square_occupied(int x, int y)
{
	return get_board_piece(x, y).type == NO_PIECE;
}

PIECE get_board_piece(int x, int y)
{
	return board_pieces[x + y * get_board_size().y];
}


Rect get_square_bounds(int x, int y)
{
	Rect rect;

	rect.x = get_current_res().x/2 - (square_size * get_board_size().x)/2 + x * square_size;
	rect.y = get_current_res().x/2 - (square_size * get_board_size().x)/2 + x * square_size;
	rect.w = square_size;
	rect.h = square_size;

	return rect;
}
