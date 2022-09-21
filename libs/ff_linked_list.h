#ifndef FF_STB_LIST
#define FF_STB_LIST
//Part of Foffonso's STB-like header only library
//Be Excellent to Each Other
//
//Distributed under GPLv3

#include "string.h"
#include "stdint.h"

#include "stdlib.h"

typedef struct ff_List_Element_ ff_List_Element;

typedef struct ff_List_Element_
{
	void* content;
	ff_List_Element* previous_element;
	ff_List_Element* next_element;
}ff_List_Element;

typedef struct ff_List_Head_
{
	uint32_t size;
	ff_List_Element* first_element;
	ff_List_Element* last_element;
} ff_List_Head;



typedef ff_List_Head ff_List;

/*
	Linked list architecture.

			h-----------\
			v			v
		    0<->1<->2<->3
		    ^-----------^	
*/

//HEADER SECTION
static inline void ff_remove_at_list(ff_List* list, const int32_t index);


//

//Initializes a linked list
static inline void ff_initialize_list(ff_List* uninitted_list)
{
	uninitted_list->size = 0;
	uninitted_list->first_element = NULL;
	uninitted_list->last_element = NULL;
}
//Destroys list.
static inline void ff_destroy_list(ff_List* list)
{
	while(list->size > 0)
	{
		ff_remove_at_list(	list,
							0);
	}
}

//Allocates and copies content to it.
static inline ff_List_Element* ff_allocate_list_element(	const void* content, 
															const uint32_t content_size)
{
	ff_List_Element* new_element = malloc(sizeof(ff_List_Element));

	new_element->content = malloc(content_size);
	memcpy(new_element->content, content, content_size);

	return new_element;
}

//Deallocates both the list element and the content.
static inline void ff_deallocate_list_element(ff_List_Element* element)
{
	free(element->content);
	free(element);
}

//Creates the first element in the case of the list being empty.
static inline void ff_create_first_element(	ff_List* list,
											const void* content, 
											const uint32_t content_size)
{
	ff_List_Element* current_element = ff_allocate_list_element(content, content_size);
	list->first_element = current_element;
	list->last_element = current_element;

	current_element->next_element = current_element;
	current_element->previous_element = current_element;
	list->size++;
}


//Puts a new element behind the element in the arguments
static inline void ff_put_behind_element(	ff_List_Element* next,
											const void* content, 
											const uint32_t content_size)
{
	ff_List_Element* new_element = ff_allocate_list_element(	content, 
																content_size);

	new_element->next_element = next;
	new_element->previous_element = next->previous_element;

	next->previous_element->next_element = new_element;
	next->previous_element = new_element;
}

//Inserts a value behind the element at index. (Index wraps)
static inline void ff_insert_behind_list_(	ff_List* list,
											const int32_t index, 
											const void* content, 
											const uint32_t content_size)
{
	ff_List_Element* current_element;

	if(list->size == 0)
	{
		ff_create_first_element(list, content, content_size);

		return;
	}

	for(uint32_t i = 0; i < index; i++)
	{
		current_element = current_element->next_element;
	}

	ff_put_behind_element(	current_element,
							content,
							content_size);

	list->last_element = list->first_element->previous_element;
	list->size++;
}
//Just a macro so we dont havet o keep putting the size of the object as a parameter
#define ff_insert_behind_list(list, index, object) ff_insert_behind_list(list, index, object, sizeof(*object))

//Gets element at index (index wraps)
static inline void* ff_get_at_list(	ff_List* list,
									const int32_t index)
{
	if(list->size == 0) return NULL;

	ff_List_Element* current_element = list->first_element;

	for(uint32_t i = 0; i < index; i++)
	{
		current_element = current_element->next_element;
	}

	return current_element->content;
}

static inline void* ff_get_last_list(ff_List* list)
{
	return list->last_element->content;
}

static inline void* ff_get_fist_list(ff_List* list)
{
	return list->first_element->content;
}

//Removes element at index (index wraps)
static inline void ff_remove_at_list(	ff_List* list,
										const int32_t index)
{
	ff_List_Element* current_element = list->first_element;

	for(uint32_t i = 0; i < index; i++)
	{
		current_element = current_element->next_element;
	}

	current_element->previous_element->next_element = current_element->next_element;
	current_element->next_element->previous_element = current_element->previous_element;
	
	if(list->first_element == current_element) list->first_element = current_element->next_element;
	if(list->last_element == current_element) list->last_element = current_element->previous_element;

	ff_deallocate_list_element(current_element);

	if(list->size > 0) list->size--;	
}

//Puts content at the end of the list.
static inline void ff_pushback_list_(	ff_List* list, 
										const void* content, 
										const uint32_t content_size)
{
	if(list->size == 0)
	{
		ff_create_first_element(list, content, content_size);
		return;
	}

	ff_put_behind_element(	list->first_element,
							content,
							content_size);

	list->last_element = list->first_element->previous_element;
	list->size++;
}
//Macro to not have to put content size everytime.
#define ff_pushback_list(list, object) ff_pushback_list_(list, object, sizeof(*object))

//Returns 1, and writes the index at index_out if found an element with the same content 
//Else returns 0
static inline int ff_find_list_(ff_List* list, uint32_t* index_out, const void* content, const uint32_t content_size)
{
	ff_List_Element* current_element = list->first_element;

	int i = 0;
	do
	{
		if(memcmp(content, current_element->content, content_size) == 0) 
		{
			*index_out = i;
			return 1;
		}

		current_element = current_element->next_element;
		i++;
	}
	while(i < list->size);

	return 0;
}
#define ff_find_list(list, index_out, content) ff_find_list_(list, index_out, content, sizeof(*content))

//So I had this weird Idea to make accessing members not waste time iterating through already checked ones.
//This implementation of a linked list can be used as a Carrousel
//By changing the pointer to the first and last elements. (Well, last elements dont really matter if this is used as a carrousell)
//However the Individual positions in the list cannot be trusted anymore as indexes.
//(Unless the user guarantees every carrousel movement ends in the same place as it started.)
static inline void ff_move_carrousel(ff_List* list, int32_t amount)
{
	if(amount >= 0)
	{
		for(int i=0; i < amount; i++)
		{
			list->first_element = list->first_element->next_element;
			list->last_element = list->last_element->next_element;
		}
	}
	else
	{
		for(int i=0; i < abs(amount); i++)
		{
			list->first_element = list->first_element->previous_element;
			list->last_element = list->last_element->previous_element;
		}
	}
}

#endif