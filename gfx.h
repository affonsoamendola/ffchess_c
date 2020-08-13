#ifndef GFX_H
#define GFX_H

#include "ff_stb.h"
#include "ff_point2.h"
#include "ff_rect.h"
#include "ff_vector2.h"
#include "ff_color.h"
#include "SDL2/SDL.h"

void init_gfx();
void render_clear();
void render_present();
void quit_gfx();


Color get_pixel(const int x, const int y);
void set_pixel(	const int x, const int y, 
				const Color color);

void fill_rectangle(const Rect rect, const Color color);

void draw_vertical_line(const int x, const int y_0, const int y_1, const Color color);
void draw_horizontal_line(const int x_0, const int x_1, const int y, const Color color);
void draw_line(Point2 p_0, Point2 p_1, const Color color);

void clear_screen();
void flush_pixels();

void blit_hardware(	uint32_t texture_id_src,
					Rect src_rect, Point2 dst_offset,
					Point2 scale);
void draw_char(	const Point2 position, uint32_t font_id, Color color,
				char character);
void draw_string(	const Point2 position, uint32_t font_id, Color color,
					char* string);
void draw_string_f(	const Point2 position, uint32_t font_id, Color color, 
					char* format_string, ...);

Point2 get_current_res();

static inline float get_aspect_ratio() 
{
	Point2 res = get_current_res();
	return res.x/(float)res.y;
}

static inline Point2 get_screen_coordinates(const Vector2f point) 
{
	return sum_p2(scale_p2(get_current_res(), 0.5f), point2(point.x, -point.y));
}

#endif