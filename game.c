#include "engine.h"
#include "board.h"

void init_game()
{
	init_board();
}

void quit_game()
{
	free_board();
}
