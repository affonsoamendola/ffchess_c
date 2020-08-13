#ifndef MOUSE_H
#define MOUSE_H

#include "display.h"
#include "input.h"

extern int mouse_x_raw;
extern int mouse_y_raw;

int get_mouse_x(){ return mouse_x_raw/get_pixel_scale(); }
int get_mouse_y(){ return mouse_y_raw/get_pixel_scale(); }
#endif