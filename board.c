#include <stdlib.h>
#include <string.h>

#include "chess.h"
#include "display.h"

#include "sprite.h"

#include "graphics.h"

#include "board.h"

int board_start_x = 0;
int board_start_y = 0;

int board_size_x = 8;
int board_size_y = 8;

int square_size = 20;
int get_square_size(){return square_size;}

ff_List game_pieces;
ff_List dead_pieces;
ff_List valid_moves_sprites;

void init_board()
{
	ff_initialize_list(&game_pieces);
	ff_initialize_list(&dead_pieces);
	ff_initialize_list(&valid_moves_sprites);

	create_board_piece(0, 0, TOWER, BLACK);
	create_board_piece(1, 0, KNIGHT, BLACK);
	create_board_piece(2, 0, BISHOP, BLACK);
	create_board_piece(3, 0, QUEEN, BLACK);
	create_board_piece(4, 0, KING, BLACK);
	create_board_piece(5, 0, BISHOP, BLACK);
	create_board_piece(6, 0, KNIGHT, BLACK);
	create_board_piece(7, 0, TOWER, BLACK);
	//create_board_piece(0, 1, PAWN, BLACK);
	//create_board_piece(1, 1, PAWN, BLACK);
	//create_board_piece(2, 1, PAWN, BLACK);
	//create_board_piece(3, 1, PAWN, BLACK);
	//create_board_piece(4, 1, PAWN, BLACK);
	//create_board_piece(5, 1, PAWN, BLACK);
	//create_board_piece(6, 1, PAWN, BLACK);
	//create_board_piece(7, 1, PAWN, BLACK);

	create_board_piece(0, 7, TOWER, WHITE);
	create_board_piece(1, 7, KNIGHT, WHITE);
	create_board_piece(2, 7, BISHOP, WHITE);
	create_board_piece(3, 7, QUEEN, WHITE);
	create_board_piece(4, 7, KING, WHITE);
	create_board_piece(5, 7, BISHOP, WHITE);
	create_board_piece(6, 7, KNIGHT, WHITE);
	create_board_piece(7, 7, TOWER, WHITE);
	create_board_piece(0, 6, PAWN, WHITE);
	create_board_piece(1, 6, PAWN, WHITE);
	create_board_piece(2, 6, PAWN, WHITE);
	create_board_piece(3, 6, PAWN, WHITE);
	create_board_piece(4, 6, PAWN, WHITE);
	create_board_piece(5, 6, PAWN, WHITE);
	create_board_piece(6, 6, PAWN, WHITE);
	create_board_piece(7, 6, PAWN, WHITE);


	for(int i = 0; i < game_pieces.size; i++)
	{
		Piece* piece = ff_get_first_list(&game_pieces);
		
		update_valid_moves(piece);

		ff_move_carrousel(&game_pieces, 1);
	}
}

void destroy_board()
{
	ff_destroy_list(&game_pieces);
	ff_destroy_list(&dead_pieces);
	ff_destroy_list(&valid_moves_sprites);
}

Point2 get_square_pixel_pos(Point2 board_coord)
{
	return sum_p2(	get_board_start_local_coords(), 
					scale_i_p2(board_coord, get_square_size()));
}

Point2 get_board_size()
{
	return point2(board_size_x, board_size_y);
}

int is_inside_board(Point2 pos)
{
	if(	pos.x >= 0 && pos.x < board_size_x &&
		pos.y >= 0 && pos.y < board_size_y)
	{
		return true;
	}	
	return false;
}

Point2 get_board_start_local_coords()
{
	Point2 start;

	start.x = get_current_res().x/2 - (square_size*board_size_x)/2;
	start.y = get_current_res().y/2 - (square_size*board_size_y)/2;

	return start;
}

Point2 get_background_start()
{
	Point2 start;

	Point2 viewport = get_viewport();

	start.x = get_current_res().x/2 - viewport.x - get_texture_w(0)/2;
	start.y = get_current_res().y/2 - viewport.y - get_texture_h(0)/2;

	return start;
}

Point2 get_board_start()
{
	Point2 start;

	Point2 viewport = get_viewport();

	start.x = get_current_res().x/2 - viewport.x - (square_size*board_size_x)/2;
	start.y = get_current_res().y/2 - viewport.y - (square_size*board_size_y)/2;

	return start;
}

//Transforms from mouse pos into world pos basically, turns pixel into square coordinates
Point2 get_square_at(Point2 pixel_coordinates)
{
	Point2 reference = get_board_start();

	Point2 delta = sub_p2(pixel_coordinates, reference);

	//This fixes some weirdness when you are checking the first negative line/column
	//Remember that when doing whole division, something that would return 0.5 on a float returns 0
	//and so does -0.5

	//This means that you get 2 squares with index 0, which is weird, this "skips" one of them;
	if(delta.x < 0) delta.x -= square_size;
	if(delta.y < 0) delta.y -= square_size;

	delta.x = delta.x / square_size;
	delta.y = delta.y / square_size;

	return delta;
}

Piece* get_piece_at(Point2 board_coord)
{
	Piece* found_piece = NULL;
	int found = false;

	for(int i = 0; i < game_pieces.size; i++)
	{
		Piece* piece = ff_get_first_list(&game_pieces);
		
		if(	equals_p2(board_coord, piece->pos))
		{
			found_piece = piece;
			found = true;
			//DO NOT BREAK OUT OF THIS
			//Since im using move carrousel to shift the list around, it needs to end at
			//the same place it started.
			//
			//Bad implementation? Yes.
			//But I'm not opening that mess and fixing it, it works remarkably well
			//Next time I'll make a better way of doing this,
			//I still have no idea how, but yeah, not a problem to fix right now
		}

		ff_move_carrousel(&game_pieces, 1);
	}

	return found_piece;
}

Piece* create_board_piece(int x, int y, PIECE_TYPE type, TEAM team)
{
	Piece new_piece;

	new_piece.team = team;
	new_piece.type = type;

	new_piece.pos = point2(x, y);

	Point2 pixel_pos = get_square_pixel_pos(point2(x, y));

	Sprite* sprite = create_sprite(	1, 
									rect(	(type-1) * get_square_size(), 
											team * get_square_size(), 
											get_square_size(), 
											get_square_size()), 
									pixel_pos.x, 
									pixel_pos.y, 
									1);

	new_piece.sprite = sprite;

	ff_initialize_list(&(new_piece.valid_moves));

	//TODO
	//This might not work properly, test it out
		

	ff_pushback_list(&game_pieces, &new_piece);

	return(ff_get_last_list(&game_pieces));
}

void move_piece(Piece* piece, Point2 new_board_coords)
{
	piece->pos = new_board_coords;

	Point2 sprite_pos = get_square_pixel_pos(new_board_coords);

	piece->sprite->pos_x = sprite_pos.x;
	piece->sprite->pos_y = sprite_pos.y;
}

void destroy_board_piece(Piece* piece)
{
	int index = -1;

	ff_find_list(&game_pieces, &index, piece);

	destroy_sprite(piece->sprite);

	ff_remove_at_list(&game_pieces, index);

	ff_destroy_list(&(piece->valid_moves));
}

void show_valid_moves(Piece* piece)
{
	for(int i = 0; i < piece->valid_moves.size; i++)
	{
		Point2 board_pos = *(Point2*)ff_get_first_list(&(piece->valid_moves));
		Point2 pixel_pos = get_square_pixel_pos(board_pos);

		Sprite* new_sprite = create_sprite(	3, 
											rect(0, 0, 20, 20), 
											pixel_pos.x, 
											pixel_pos.y, 
											1);

		ff_pushback_list(&valid_moves_sprites, new_sprite);

		ff_move_carrousel(&(piece->valid_moves), 1);
	}
}

void hide_valid_moves()
{
	while(valid_moves_sprites.size > 0)
	{
		Sprite* sprite = ff_get_first_list(&valid_moves_sprites);

		destroy_sprite(sprite);

		ff_remove_at_list(	&valid_moves_sprites,
							0);

	}
}

void update_valid_moves(Piece* piece)
{
	ff_destroy_list(&(piece->valid_moves));
	get_valid_moves(piece, &(piece->valid_moves));
}

void get_valid_moves(Piece* piece_, ff_List* out_initted_list)
{	
	Piece piece = *piece_;
	Point2 valid_spot;

	if(piece.type == PAWN)
	{
		//Basic Movement
		if(piece.team == WHITE)
		{
			valid_spot = sum_p2(piece.pos, point2(0, -1));
		}
		else if(piece.team == BLACK)
		{
			valid_spot = sum_p2(piece.pos, point2(0, 1));
		}

		if(	is_inside_board(valid_spot) &&
			get_piece_at(valid_spot) == NULL) ff_pushback_list(out_initted_list, &valid_spot);

		//If havent moved yet, pawns move 2 pieces
		if(piece.pos.y == 6 && piece.team == WHITE)
		{
			valid_spot = sum_p2(piece.pos, point2(0, -2));

			if(	is_inside_board(valid_spot) &&
				get_piece_at(valid_spot) == NULL)  ff_pushback_list(out_initted_list, &valid_spot);
		}
		
		if(piece.pos.y == 1 && piece.team == BLACK)
		{
			valid_spot = sum_p2(piece.pos, point2(0, 2));

			if(	is_inside_board(valid_spot) &&
				get_piece_at(valid_spot) == NULL) ff_pushback_list(out_initted_list, &valid_spot);
		}

		Piece* left = NULL;
		Piece* right = NULL;
		//Eating moves/ diagonal
		if(piece.team == WHITE)
		{
			left  = get_piece_at(sum_p2(piece.pos, point2(-1, -1)));
			right = get_piece_at(sum_p2(piece.pos, point2( 1, -1)));

			if(left != NULL)
			{
				if(left->team == BLACK)
				{
					valid_spot = sum_p2(piece.pos, point2(-1, -1));
					if(is_inside_board(valid_spot)) ff_pushback_list(out_initted_list, &valid_spot);
				}
			}

			if(right != NULL)
			{	
				if(right->team == BLACK)
				{
					valid_spot = sum_p2(piece.pos, point2( 1, -1));
					if(is_inside_board(valid_spot)) ff_pushback_list(out_initted_list, &valid_spot);
				}
			}
		}
		else if(piece.team == BLACK)
		{
			left  = get_piece_at(sum_p2(piece.pos, point2(-1, 1)));
			right = get_piece_at(sum_p2(piece.pos, point2( 1, 1)));

			if(left != NULL)
			{
				if(left->team == WHITE)
				{
					valid_spot = sum_p2(piece.pos, point2(-1, 1));
					if(is_inside_board(valid_spot)) ff_pushback_list(out_initted_list, &valid_spot);
				}
			}

			if(right != NULL)
			{
				if(right->team == WHITE)
				{
					valid_spot = sum_p2(piece.pos, point2( 1, 1));
					if(is_inside_board(valid_spot)) ff_pushback_list(out_initted_list, &valid_spot);
				}
			}
		}
	}

	
	Point2 cardinal_directions[4] = {{0, 1}, {0, -1}, { 1, 0}, {-1, 0}}; 
	Point2 diagonal_directions[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1,-1}};

	
	if(piece.type == TOWER || piece.type == BISHOP || piece.type == QUEEN)
	{
		//This types of movement thing allows me to reuse tower and bishop code
		//for the Queen, since her movements are basically the both of them combined


		//If this is not a queen, then the outermost for only runs once, and the
		//type of movement is determined at the first if after the rotation for-loop
		//
		//Else, there are two types of movement, cardinal and diagonal, determined by
		//the "or" (||) of that same if;

		int types_of_movement = 1;

		if(piece.type == QUEEN)
		{
			types_of_movement = 2;
		}

		for(int type_of_movement = 0; type_of_movement < types_of_movement; type_of_movement ++)
		{

			for(int rotation = 0; rotation < 4; rotation ++)
			{
				Point2 direction;
			
				if(piece.type == TOWER || (piece.type == QUEEN && type_of_movement == 0))	
				{
					direction = cardinal_directions[rotation];
				}
				else if(piece.type == BISHOP || (piece.type == QUEEN && type_of_movement == 1))
				{
					direction = diagonal_directions[rotation];
				}

				int clear_path = true;
				int length = 1;
				
				while(clear_path == true)
				{

					valid_spot = sum_p2(piece.pos, scale_i_p2(direction, length));
					
					if(!is_inside_board(valid_spot))
					{
						break;
					}

					Piece* piece_at_spot = NULL;
	 				piece_at_spot = get_piece_at(valid_spot);
	 				
	 				if(piece_at_spot != NULL)
	 				{
	 					clear_path = false;
	 					if(piece.team == WHITE)
		 				{
		 					if(piece_at_spot->team == BLACK)
		 					{
		 						ff_pushback_list(out_initted_list, &valid_spot);
		 					}
		 				}
		 				else if(piece.team == BLACK)
		 				{
		 					if(piece_at_spot->team == WHITE)
		 					{
		 						ff_pushback_list(out_initted_list, &valid_spot);
		 					}
		 				}
	 				}
	 				else
	 				{
		 				ff_pushback_list(out_initted_list, &valid_spot);		
	 				}
						
					length ++;
				}
			}
		}
	}

	Point2 knight_positions[8] = {{1, 2}, {2, 1}, {1, -2}, {2, -1}, {-1, 2}, {-2, 1}, {-1, -2}, {-2, -1}};

	if(piece.type == KNIGHT)
	{
		for(int i = 0; i < 8; i++)
		{
			valid_spot = sum_p2(piece.pos, knight_positions[i]);

			if(!is_inside_board(valid_spot))
			{
				continue;
			}


			Piece* piece_at_spot = NULL;
 			piece_at_spot = get_piece_at(valid_spot);

 			if(piece_at_spot == NULL)
 			{
 				ff_pushback_list(out_initted_list, &valid_spot);
 			}
 			else
 			{
 				if(piece.team == WHITE)
 				{
 					if(piece_at_spot->team == BLACK)
 					{
 						ff_pushback_list(out_initted_list, &valid_spot);
 					}
 				}
 				else if(piece.team == BLACK)
 				{
 					if(piece_at_spot->team == WHITE)
 					{
 						ff_pushback_list(out_initted_list, &valid_spot);
 					}
 				}
 			}
		}
	}
}	 