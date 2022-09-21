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
Point2 get_board_start_local_coords();

Piece* get_piece_at(Point2 board_coord);
Point2 get_square_at(Point2 pixel_coordinates);

void update_valid_moves(Piece* piece);
void get_valid_moves(Piece* piece_, ff_List* out_initted_list);
void move_piece(Piece* piece, Point2 new_board_coords);

void show_valid_moves();
void hide_valid_moves();

Piece* create_board_piece(int x, int y, PIECE_TYPE type, TEAM team);
#endif