#ifndef GRAPHICS_H
#define GRAPHICS_H

void draw_game();

void move_viewport(Point2 delta);
Point2 get_viewport();

void init_graphics();
void quit_graphics();

#endif