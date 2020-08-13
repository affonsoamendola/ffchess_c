#include "chess.h"
#include "board.h"

int selected_square_x = 0;
int selected_square_y = 0;

typedef struct OVER_SQUARE_RETURN_
{
	PIECE_TYPE piece;
	TEAM team;
	int square_x;
	int square_y;

} OVER_SQUARE_RETURN;

OVER_SQUARE_RETURN is_over_square(int x, int y)
{

}