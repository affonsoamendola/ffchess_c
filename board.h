#ifndef BOARD_H
#define BOARD_H

#include "chess.h"
#include "ff_point2.h"

void init_board();
void free_board();

int get_square_size();
Point2 get_board_size();

Point2 get_background_start();
Point2 get_board_start();

extern PIECE* board_pieces;

#endif