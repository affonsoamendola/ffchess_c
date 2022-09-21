#ifndef FF_STB_COLOR
#define FF_STB_COLOR

#define color(r, g, b, a) (Color){r, g, b, a}
#define COLOR_WHITE color(255,255,255,255)

typedef struct Color_
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} Color;

static inline Color scale_f_color(const Color color, const float factor)
{
	return color(	color.r * factor, 
					color.g * factor,
					color.b * factor,
					color.a * factor);
}

static inline Color lerp_f_color(const Color min, const Color max, const float parameter)
{
	return color( 	(max.r - min.r) * parameter + min.r,
					(max.g - min.g) * parameter + min.g,
					(max.b - min.b) * parameter + min.b,
					(max.a - min.a) * parameter + min.a );
}

#endif