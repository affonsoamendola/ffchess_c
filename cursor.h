#ifndef CURSOR_H
#define CURSOR_H

#include "ff_point2.h"

void move_cursor_to(Point2 location);
void move_cursor(int d_x, int d_y);

Point2 get_cursor_pos();

int get_cursor_visible();

#endif 