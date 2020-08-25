#include "chess.h"
#include "board.h"


#include "stdio.h"
#include "ff_point2.h"

Point2 cursor_pos = {0, 0};

int show_cursor = 1;

void move_cursor_to(Point2 location)
{
	if(	location.x >= 0 && location.x < get_board_size().x &&
		location.y >= 0 && location.y < get_board_size().y)
	{
		cursor_pos = location;
	}
}

void move_cursor(int d_x, int d_y)
{
	move_cursor_to(sum_p2(cursor_pos, point2(d_x, d_y)));	
	printf("%d %d\n", cursor_pos.x, cursor_pos.y);
}