#ifndef FF_STB_STATIC_STACK
#define FF_STB_STATIC_STACK


typedef struct ff_Static_List_
{
	uint32_t edges_size;
	bool edges_loaded[];
} ff_Static_List;

Edge edges[MAX_EDGES];

#define ff_create_static_list(list, size) \
uint32_t list_size;\
bool list_loaded[size]\




#define






#endif