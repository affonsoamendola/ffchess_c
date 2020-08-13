#ifndef CHESS_H
#define CHESS_H

#include "ff_point2.h"

typedef enum TEAM_
{
	WHITE = 0,
	BLACK = 1,
} TEAM;

typedef enum PIECE_TYPE_
{
	NO_PIECE = 0,
	PAWN = 1,
	TOWER = 2,
	KNIGHT = 3,
	BISHOP = 4,
	QUEEN = 5,
	KING = 6,

} PIECE_TYPE;

typedef struct PIECE_
{
	TEAM team;
	PIECE_TYPE type;
} PIECE;

PIECE get_board_piece(int x, int y);

void set_board_piece(int x, int y, PIECE_TYPE type, TEAM team);

#endif