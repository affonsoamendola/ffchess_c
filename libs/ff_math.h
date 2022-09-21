#ifndef FF_STB_MATH
#define FF_STB_MATH

#include <math.h>
   
static inline float ff_sin_lim_f(float x, float min, float max)
{
	return (sin(x)*(max - min) + (max + min))/2.0f;
}

static inline double ff_sin_lim_d(double x, double min, double max)
{
	return (sin(x)*(max - min) + (max + min))/2.0f;;
}

static inline int ff_max(int a, int b)
{
	if(a > b) return a;
	else return b;
}

static inline int ff_min(int a, int b)
{
	if(a < b) return a;
	else return b;
}

static inline float ff_lerp_f(float min_x, float max_x, float parameter)
{
	return  (max_x - min_x) * parameter + min_x;
}

static inline float ff_param_f(float min_x, float max_x, float x)
{
	return  (x - min_x) / (max_x - min_x);
}


#endif