#ifndef FF_STB_VECTOR2F
#define FF_STB_VECTOR2F

#include "math.h"
#include "float.h"
#include "stdlib.h"

#if __has_include("ff_point2.h")
#include "ff_point2.h"
#endif

#define vector2f(x, y) (Vector2f){x, y}

#define ZERO_VECTOR2F (Vector2f){0., 0.}

typedef struct Vector2f_
{
	float x;
	float y;
} Vector2f;

#ifdef FF_STB_POINT2
static inline Vector2f p2v2(const Point2 point)
{
	return vector2f((float)point.x, (float)point.y);
}
#endif

static inline Vector2f sum_v2(const Vector2f vector_a, const Vector2f vector_b)
{
	return vector2f(vector_a.x + vector_b.x, vector_a.y + vector_b.y);
}

static inline Vector2f neg_v2(const Vector2f vector)
{
	return vector2f(-vector.x, -vector.y);
}

static inline Vector2f sub_v2(const Vector2f vector_a, const Vector2f vector_b)
{
	return vector2f(vector_a.x - vector_b.x, vector_a.y - vector_b.y);
}

static inline Vector2f scale_v2(const Vector2f vector, float scalar)
{
	return vector2f(vector.x * scalar, vector.y * scalar);
}

static inline float dot_v2(const Vector2f vector_a, const Vector2f vector_b)
{
	return (vector_a.x * vector_b.x) + (vector_a.y * vector_b.y);
}

static inline float norm_v2(const Vector2f vector)
{
	return sqrt(dot_v2(vector, vector));
}

static inline float cross_v2(const Vector2f vector_a, const Vector2f vector_b)
{
	return (vector_a.x * vector_b.y) - (vector_a.y * vector_b.x);
}

static inline Vector2f rot_v2(const Vector2f vector, float ang_rad)
{
	return vector2f(vector.x * cos(ang_rad) - vector.y * sin(ang_rad),
					vector.x * sin(ang_rad) + vector.y * cos(ang_rad));
}

static inline float distance_v2(const Vector2f vector_to, const Vector2f vector_from)
{
	return norm_v2(sub_v2(vector_to, vector_from));
}

static inline float distance_to_segment_v2(	const Vector2f point, 
											const Vector2f seg_start, 
											const Vector2f seg_end, 
											Vector2f * closest_point)
{
	float distance;

	Vector2f v;
	Vector2f w;

	float c1, c2;

	v = sub_v2 (seg_end, seg_start);
	w = sub_v2 (point, seg_start);

	c1 = dot_v2(v, w);

	if(c1 <= 0)
	{
		if(closest_point != NULL)
			*closest_point = seg_start;
		return norm_v2(sub_v2(point, seg_start));
	}

	c2 = dot_v2(v, v);

	if(c2 <= c1)
	{
		if(closest_point != NULL)
			*closest_point = seg_end;
		return norm_v2(sub_v2(point, seg_end));
	}

	float p = c1/c2;

	Vector2f projection;

	projection = sum_v2(seg_start, scale_v2(v, p));

	if(closest_point != NULL)
		*closest_point = projection;

	return norm_v2(sub_v2(projection, point));			
}

#define VECTOR2F_INTERSECTION_COLLINEAR -1
#define VECTOR2F_NO_INTERSECTION 0
#define VECTOR2F_INTERSECTION 1

static inline int intersect_v2(const Vector2f v1, const Vector2f v2, const Vector2f v3, const Vector2f v4, Vector2f * intersect_vector)
{
	double denominator = ((v4.x - v3.x) * (v1.y - v2.y) - (v1.x - v2.x) * (v4.y - v3.y));

	if(denominator <= DBL_EPSILON && denominator >= -DBL_EPSILON) return VECTOR2F_INTERSECTION_COLLINEAR;

	Vector2f len_segment_1;

	double t_a = 	((v3.y - v4.y) * (v1.x - v3.x) + (v4.x - v3.x) * (v1.y - v3.y)) / denominator;
	double t_b = 	((v1.y - v2.y) * (v1.x - v3.x) + (v2.x - v1.x) * (v1.y - v3.y)) / denominator;

	if(t_a >= 0. && t_a <= 1. && t_b >= 0. && t_b <= 1.)
	{
		if(intersect_vector != NULL)
		{
			len_segment_1 = sub_v2(v2, v1);

			*intersect_vector = sum_v2(v1, scale_v2(len_segment_1, (float)t_a));
		}

		return VECTOR2F_INTERSECTION;
	}
	else return VECTOR2F_NO_INTERSECTION;
}

#define VECTOR2_RIGHT_SIDE 1
#define VECTOR2_LEFT_SIDE -1
#define VECTOR2_ON_LINE 0

static inline int point_side_v2(const Vector2f point, const Vector2f v0, const Vector2f v1)
{
	float cross = cross_v2(sub_v2(v1, v0), sub_v2(point, v0));

	if(cross > FLT_EPSILON)
		return VECTOR2_RIGHT_SIDE;
	else if(cross < -FLT_EPSILON)
		return VECTOR2_LEFT_SIDE;

	return VECTOR2_ON_LINE;
}
/*
int intersect_box_v2(VECTOR2 box_a_start, VECTOR2 box_a_end, VECTOR2 box_b_start, VECTOR2 box_b_end)
{
	if(	(min_float(box_a_start.x, box_a_end.x) <= max_float(box_b_start.x, box_b_end.x) && min_float(box_b_start.x, box_b_end.x) <= max_float(box_a_start.x, box_a_end.x)) &&
		(min_float(box_a_start.y, box_a_end.y) <= max_float(box_b_start.y, box_b_end.y) && min_float(box_b_start.y, box_b_end.y) <= max_float(box_a_start.y, box_a_end.y)) )
		return 1;
	else
		return 0;
}
*/

static inline void swap_v2(Vector2f* a, Vector2f* b)
{
	Vector2f temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static inline int inside_rect_v2(const Vector2f pos, const Vector2f box_top_left, const Vector2f box_bottom_right)
{
	if(	pos.x > box_top_left.x && pos.x < box_bottom_right.x &&
		pos.y < box_top_left.y && pos.y > box_bottom_right.y)
	{
		return 1;
	}
	return 0;
}

#endif