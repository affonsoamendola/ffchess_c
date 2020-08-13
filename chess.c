#include <stdlib.h>
#include <string.h>

#include "chess.h"
#include "board.h"

#include "ff_rect.h"

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
