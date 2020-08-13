#ifndef BOARD_H
#define BOARD_H

#include "ff_point2.h"

void init_board();
void draw_board();
void free_board();

Point2 get_board_size();

extern PIECE* board_pieces;

#endif