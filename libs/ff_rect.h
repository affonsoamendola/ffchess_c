#ifndef FF_STB_RECT
#define FF_STB_RECT

#define rect(x, y, w, h) (Rect){x, y, w, h}
#define rect_wh(w, h) (Rect){0, 0, w, h}

#define ZERO_RECT (Rect){0, 0, 0, 0}
#define ONE_RECT (Rect){1, 1, 1, 1}

typedef struct _Rect
{
	int x;
	int y;
	unsigned int w;
	unsigned int h;
} Rect;

static inline Rect offset_i_rect(const Rect rect, unsigned int off_x, unsigned int off_y)
{
	return rect(rect.x + off_x, rect.y + off_y, rect.w, rect.h);
}
static inline Rect scale_i_rect(const Rect rect, unsigned int scale_x, unsigned int scale_y)
{
	return rect(rect.x*scale_x, rect.y*scale_y, rect.w*scale_x, rect.h*scale_y);
}

static inline int equals_rect(const Rect a, const Rect b)
{
	if(a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h) return 1;
	else return 0;
}

#endif