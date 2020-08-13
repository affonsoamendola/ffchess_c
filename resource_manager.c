#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "engine.h"
#include "ff_stb.h"

#include "resource_manager.h"

ResourceAtlas resource_atlas = {0};

//Gets the renderer from the GFX System.
extern SDL_Renderer* renderer;

void init_resource_manager()
{
	//Initializes SDL_image
	if((IMG_Init(SDL_INIT_EVERYTHING)==-1))
	{
		printf("Could not initialize SDL_image: %s\n", SDL_GetError());
		exit(-1);
	}

	load_texture_resource_list("./graphix/default_texture_list.trl");
	load_font_resource_list("./graphix/default_font_list.frl");
}

void quit_resource_manager()
{

	IMG_Quit();
}

void free_texture_resource(uint32_t texture_id);

void free_resource_atlas()
{
	for(int i = 0; i < MAX_TEXTURES; i++)
	{
		free_texture_resource(i);
	}
}

ResourceTexture* load_texture_resource(char* location, uint32_t index)
{
	if(index >= MAX_TEXTURES || index < 0)
	{
		printf("Texture index %d out of bounds, limits are 0 and %d", index, MAX_TEXTURES-1);
		return NULL;
	}

	if(resource_atlas.textures[index].loaded == true)
	{
		printf("Texture %d already loaded.", index);
		return NULL;
	}

	SDL_Surface* sdl_surface;

	sdl_surface = IMG_Load(location);

	if(sdl_surface == NULL)
	{
		printf("Could not load image file, Error: %s\n", SDL_GetError());
		exit(-1);
	}

	ResourceTexture texture;

	texture.loaded = true;
	texture.w = sdl_surface->w;
	texture.h = sdl_surface->h;
	texture.texture = SDL_CreateTextureFromSurface(renderer, sdl_surface);

	resource_atlas.textures[index] = texture;

	SDL_FreeSurface(sdl_surface);

	return &resource_atlas.textures[index];
}

bool get_texture_resource(uint32_t index, ResourceTexture** texture_pointer_out)
{
	if(resource_atlas.textures[index].loaded == true)
	{
		*texture_pointer_out = &resource_atlas.textures[index];
		return true;
	}
	else
	{
		*texture_pointer_out = NULL;
		return false;
	}
}

void free_texture_resource(uint32_t texture_id)
{
	ResourceTexture* texture;

	if(get_texture_resource(texture_id, &texture))
	{
		resource_atlas.textures[texture_id] = (ResourceTexture){0};
		SDL_DestroyTexture(texture->texture);
	}
	else
	{
		printf("Attempted to free an unloaded texture_id.");
	}
}

uint32_t get_texture_w(uint32_t index)
{
	return resource_atlas.textures[index].w;
}

uint32_t get_texture_h(uint32_t index)
{
	return resource_atlas.textures[index].h;
}

//Reads a texture resource list file (.trl)
//Format is supposed to be easily editable, and consists in a ASCII readable text file
//first line is composed of the characters TRL followed by a blank space (' ') and an
//integer with the number of entries in the list.
//after which begins each entry, which is composed of an index unsigned int followed by a 
//two ints with the texture size and a string of the texture png file (relative to executable.) 
//(max of 128 characters on the string, can be easily increased by changing the buffer array size)
//EX:
//TRL 3
//0 graphix/tex0.png //index 0 at graphix/tex0.png
//1 graphix/tex1.png //index 1 at graphix/tex1.png
//2 tex1234.png
bool load_texture_resource_list(const char* location)
{
	FILE* texture_list_file;

	texture_list_file = fopen(location, "r");

	if(texture_list_file != NULL)
	{
		if(fgetc(texture_list_file) != 'T') return 1;
		if(fgetc(texture_list_file) != 'R') return 1;
		if(fgetc(texture_list_file) != 'L') return 1;
		if(fgetc(texture_list_file) != ' ') return 1;

		uint32_t texture_amount = 0;

		fscanf(texture_list_file, "%d", &texture_amount);	

		for(int i = 0; i < texture_amount; i++)
		{
			char buffer[128];
			
			uint32_t index;
			
			fscanf(texture_list_file, "%d %s", &index, buffer);
			
			load_texture_resource(buffer, index);
		}

		fclose(texture_list_file);

		return false;
	}
	else return true;
}

ResourceFont* load_font_resource(	char* location, uint32_t index, 
									uint32_t char_w, uint32_t char_h)
{
	if(index >= MAX_FONTS || index < 0)
	{
		printf("Font index %d out of bounds, limits are 0 and %d", index, MAX_FONTS-1);
		return NULL;
	}

	if(resource_atlas.fonts[index].loaded == true)
	{
		printf("Font %d already loaded.", index);
		return NULL;
	}

	SDL_Surface* sdl_surface;

	sdl_surface = IMG_Load(location);

	if(sdl_surface == NULL)
	{
		printf("Could not load image file, Error: %s\n", SDL_GetError());
		exit(-1);
	}

	ResourceFont font;

	font.loaded = true;
	font.sheet_w = sdl_surface->w;
	font.sheet_h = sdl_surface->h;
	font.char_w = char_w;
	font.char_h = char_h;
	font.texture = SDL_CreateTextureFromSurface(renderer, sdl_surface);

	resource_atlas.fonts[index] = font;

	SDL_FreeSurface(sdl_surface);

	return &resource_atlas.fonts[index];
}

bool get_font_resource(uint32_t index, ResourceFont** font_pointer_out)
{
	if(resource_atlas.fonts[index].loaded == true)
	{
		*font_pointer_out = &resource_atlas.fonts[index];
		return true;
	}
	else
	{
		*font_pointer_out = NULL;
		return false;
	}
}

void free_font_resource(uint32_t font_id)
{
	ResourceFont* font;

	if(get_font_resource(font_id, &font))
	{
		resource_atlas.fonts[font_id] = (ResourceFont){0};
		SDL_DestroyTexture(font->texture);
	}
	else
	{
		printf("Attempted to free an unloaded font_id.");
	}
}

//Reads a font resource list file (.frl)
//Format is supposed to be easily editable, and consists in a ASCII readable text file
//first line is composed of the characters FRL followed by a blank space (' ') and an
//integer with the number of entries in the list.
//after which begins each entry, which is composed of an index unsigned int followed by a 
//two ints with the font char size and a string of the font png file (relative to executable.) 
//(max of 128 characters on the string, can be easily increased by changing the buffer array size)
//EX:
//FRL 3
//0 8 8 graphix/font0.png //index 0 size 8x8 at graphix/font0.png
//1 4 6 graphix/font1.png //index 0 size 4x6 at graphix/font1.png
//2 16 16 font1234.png
bool load_font_resource_list(const char* location)
{
	FILE* font_list_file;

	font_list_file = fopen(location, "r");

	if(font_list_file != NULL)
	{
		if(fgetc(font_list_file) != 'F') return 1;
		if(fgetc(font_list_file) != 'R') return 1;
		if(fgetc(font_list_file) != 'L') return 1;
		if(fgetc(font_list_file) != ' ') return 1;

		uint32_t font_amount = 0;

		fscanf(font_list_file, "%d", &font_amount);	

		for(int i = 0; i < font_amount; i++)
		{
			char buffer[128];
			
			uint32_t index;
			uint32_t char_w;
			uint32_t char_h;
			
			fscanf(font_list_file, "%d %d %d %s", &index, &char_w, &char_h, buffer);
			
			load_font_resource(buffer, index, char_w, char_h);
		}

		fclose(font_list_file);

		return false;
	}
	else return true;
}
