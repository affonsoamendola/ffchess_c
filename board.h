#ifndef BOARD_H
#define BOARD_H

#include "chess.h"
#include "ff_point2.h"

void init_board();
void destroy_board();

int get_square_size();
Point2 get_board_size();

Point2 get_background_start();
Point2 get_board_start();

Piece* get_piece_at(Point2 board_coord);
Point2 get_square_at(Point2 pixel_coordinates);

void get_valid_moves(Piece* piece_, ff_List* out_initted_list);

void show_valid_moves();
void hide_valid_moves();

void create_board_piece(int x, int y, PIECE_TYPE type, TEAM team);
#endif