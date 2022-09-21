#ifndef CHESS_H
#define CHESS_H

#include "ff_point2.h"

#include "sprite.h"

typedef enum TEAM_
{
	WHITE = 0,
	BLACK = 1,
} TEAM;

typedef enum PIECE_TYPE_
{
	PAWN = 1,
	TOWER = 2,
	KNIGHT = 3,
	BISHOP = 4,
	QUEEN = 5,
	KING = 6,

} PIECE_TYPE;

typedef struct Piece_
{
	Sprite* sprite;

	TEAM team;
	PIECE_TYPE type;

	ff_List valid_moves

	Point2 pos;
} Piece;

#endif