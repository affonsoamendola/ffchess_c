#ifndef FF_STB_POINT2
#define FF_STB_POINT2

#define point2(x, y) (Point2){x, y}

#define ZERO_POINT2 (Point2){0, 0}
#define ONE_POINT2 (Point2){1, 1}
#define MINUS_ONE_POINT2 (Point2){-1, -1}

typedef struct _Point2
{
	int x;
	int y;

} Point2;

static inline int equals_p2(const Point2 point_a, const Point2 point_b)
{
	if(point_a.x == point_b.x && point_a.y == point_b.y)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

static inline Point2 sum_p2(const Point2 point_a, const Point2 point_b)
{
	return point2(point_a.x + point_b.x, point_a.y + point_b.y);
}

static inline Point2 mul_p2(const Point2 point_a, const Point2 point_b)
{
	return point2(point_a.x * point_b.x, point_a.y * point_b.y);
}

static inline Point2 sub_p2(const Point2 point_a, const Point2 point_b)
{
	return point2(point_a.x - point_b.x, point_a.y - point_b.y);
}

static inline Point2 neg_p2(const Point2 point)
{
	return point2(-point.x, -point.y);
}

static inline Point2 scale_i_p2(const Point2 point, const int scalar)
{
	return point2(point.x * scalar, point.y * scalar);
}

static inline Point2 scale_f_p2(const Point2 point, const float scalar)
{
	return point2(point.x * scalar, point.y * scalar);
}

static inline Point2 scale_p2(const Point2 point, const float scalar)
{
	return scale_f_p2(point, scalar);
}

static inline Point2 clamp_p2(const Point2 in, const Point2 min, const Point2 max)
{
	Point2 out;

	if(in.x < min.x) { out.x = min.x;}
	else if(in.x > max.x) { out.x = max.x;}
	else { out.x = in.x; }

	if(in.y < min.y) { out.y = min.y;}
	else if(in.y > max.y) { out.y = max.y;}
	else { out.y = in.y; }

	return out;
}

static inline void swap_p2(Point2* a, Point2* b)
{
	Point2 temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

#endif