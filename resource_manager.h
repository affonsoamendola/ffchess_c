#ifndef FF_RESOURCE_H
#define FF_RESOURCE_H

#define MAX_TEXTURES 512
#define MAX_FONTS 64

#include "ff_stb.h"

typedef struct _ResourceFont
{
	bool loaded;
	uint32_t sheet_w;
	uint32_t sheet_h;
	uint32_t char_w;
	uint32_t char_h;
	SDL_Texture* texture;
}ResourceFont;

typedef struct _ResourceTexture
{
	bool loaded;
	uint32_t w;
	uint32_t h;
	SDL_Texture* texture;
} ResourceTexture;

typedef struct _ResourceAtlas
{
	ResourceTexture textures[MAX_TEXTURES];
	ResourceFont fonts[MAX_FONTS];
} ResourceAtlas;

void free_resource_atlas();

bool load_texture_resource_list(const char* location);
bool get_texture_resource(uint32_t index, ResourceTexture** texture_pointer_out);

uint32_t get_texture_w(uint32_t index);
uint32_t get_texture_h(uint32_t index);

bool load_font_resource_list(const char* location);
bool get_font_resource(uint32_t index, ResourceFont** font_pointer_out);

void init_resource_manager();
void quit_resource_manager();

#endif