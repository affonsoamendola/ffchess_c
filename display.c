#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "engine.h"
#include "resource_manager.h"

#include "ff_point2.h"
#include "ff_color.h"

#include "display.h"

uint32_t screen_res_x = 320;
uint32_t screen_res_y = 240;

float aspect_ratio = 0.0f;
int pixel_scale = 3;
int get_pixel_scale(){return pixel_scale;}

char* window_title = "Foffonso's Chess";

SDL_Window* window = NULL;
SDL_Texture* screen_surface = NULL;
Color* screen_pixels = NULL;

SDL_Renderer* renderer = NULL;

//Initializes graphics subsystem
void init_display()
{	
	aspect_ratio = screen_res_x / (float)screen_res_y;
	
	if((SDL_InitSubSystem(SDL_INIT_VIDEO)==-1))
	{
		printf("Could not initialize SDL Video: %s\n", SDL_GetError());
		exit(-1);
	}


	//Creates SDL window
	window = SDL_CreateWindow(	window_title,                // window title
						        SDL_WINDOWPOS_UNDEFINED,         // initial x position
						        SDL_WINDOWPOS_UNDEFINED,         // initial y position
						        screen_res_x * pixel_scale,  // width, in pixels
						        screen_res_y * pixel_scale,  // height, in pixels
						        SDL_WINDOW_OPENGL                // flags
						    );


	if(window == NULL)
	{
		printf("Could not set up SDL2 window: %s\n", SDL_GetError());
		exit(1);
	}

	//Creates SDL renderer context.
	renderer = SDL_CreateRenderer	( 	window,
										-1,
										SDL_RENDERER_ACCELERATED
									);

	if(renderer == NULL)
	{
		printf("Problem creating SDL Renderer: %s\n", SDL_GetError());
		exit(1);
	}

	//Allocates the area that will be storing all the pixel information
	//Each pixel is in the RGBA8888 format, so 4 bytes per pixel
	screen_pixels = malloc(	screen_res_x * 
							screen_res_y * sizeof(Color));

	//Creates the texture that will be receiving all pixels
	//And will be flushed to screen after integer scaling.
	screen_surface = SDL_CreateTexture(	renderer,
										SDL_PIXELFORMAT_ABGR8888,
										SDL_TEXTUREACCESS_STREAMING,
										screen_res_x, 
										screen_res_y
										);

	SDL_SetTextureBlendMode(screen_surface, SDL_BLENDMODE_BLEND);

	//Sets texture to have nearest neighbor scaling
	//Which is the same as integer scaling when dealing with integer scaling factors.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	SDL_ShowCursor(0);
	init_resource_manager();
}

void quit_display()
{
	quit_resource_manager();
	free(screen_pixels);
	SDL_DestroyTexture(screen_surface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

Point2 get_current_res()
{
	return point2(screen_res_x, screen_res_y);
}

Color get_pixel(const int x, const int y)
{

   	return *(Color *)(screen_pixels + y*screen_res_x + x);
}

void set_pixel(	const int x, const int y, 
				const Color color)
{
	if(x >= 0 && x < screen_res_x && y >= 0 && y < screen_res_y)
	{
		memcpy(screen_pixels + (y*screen_res_x + x), &color, 4);
	}
}

void draw_vertical_line(const int x, const int y_0, const int y_1, const Color color)
{
	for(int y = y_0; y <= y_1; y++)
	{
		set_pixel(x, y, color);
	}
}

void draw_horizontal_line(const int x_0, const int x_1, const int y, const Color color)
{
	for(int x = x_0; x <= x_1; x++)
	{
		set_pixel(x, y, color);
	}
}

void draw_line(Point2 p_0, Point2 p_1, const Color color)
{
	float slope;

	Point2 temp;

	float current_error = 0.0f;

	float x, y;

	if(p_0.x == p_1.x)
	{
		if(p_0.y > p_1.y)
		{
			draw_vertical_line(p_0.x, p_1.y, p_0.y, color);
		}
		else
		{
			draw_vertical_line(p_0.x, p_0.y, p_1.y, color);
		}
	}
	else if(p_0.y == p_1.y)
	{
		if(p_0.x > p_1.x)
		{
			draw_horizontal_line(p_1.x, p_0.x, p_0.y, color);
		}
		else
		{
			draw_horizontal_line(p_0.x, p_1.x, p_0.y, color);
		}
	}
	else if(abs(p_1.y - p_0.y) < abs(p_1.x - p_0.x))
	{
		if(p_0.x > p_1.x)
		{
			swap_p2(&p_0, &p_1);
		}

		y = p_0.y;
		
		slope = (float)(p_1.y - p_0.y)/(float)(p_1.x - p_0.x);

		for(int x = p_0.x; x <= p_1.x; x++)
		{
			set_pixel(x, y, color);

			current_error += slope;

			if(current_error >= 0.5f)
			{
				y = y + 1;
				current_error -= 1.0f;
			}

			if(current_error <= -0.5f)
			{
				y = y - 1;
				current_error += 1.0f;
			}
		}
	}
	else
	{
		if(p_0.y > p_1.y)
		{
			swap_p2(&p_0, &p_1);
		}

		x = p_0.x;
		
		slope = (float)(p_1.x - p_0.x)/(float)(p_1.y - p_0.y);

		for(int y = p_0.y; y <= p_1.y; y++)
		{
			set_pixel(x, y, color);

			current_error += slope;

			if(current_error >= 0.5f)
			{
				x = x + 1;
				current_error -= 1.0f;
			}
			else if(current_error <= -0.5f)
			{
				x = x - 1;
				current_error += 1.0f;
			}
		}
	}
}

void fill_rectangle(const Rect rect, const Color color)
{
	for(int y = rect.y; y < rect.y + rect.h; y++)
	{
		draw_horizontal_line(rect.x, rect.x + rect.w - 1, y, color);
	}
}

void fill_screen(const Color color)
{
	for(int x = 0; x < screen_res_x; x++)
	{
		for(int y = 0; y < screen_res_y; y++)
		{
			memcpy(screen_pixels + y*screen_res_x + x, &color, 4);
		}
	}
}

void clear_screen()
{

	memset(screen_pixels, 0, screen_res_x * screen_res_y*4);
}

void render_clear()
{
	//Clears everything
	//Renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	//Pixel Buffer.
	clear_screen();
}

void render_present()
{
	//Shows on screen
    SDL_RenderPresent(renderer);

    //printf("AVG FPS= %f FPS= %f DELTA_TIME= %f\n", engine_average_fps(), engine_fps(), engine_delta_time());
}

void flush_pixels()
{
	//Updates screen texture with pixel information.
	SDL_UpdateTexture(
	    screen_surface,
	    NULL,
	   	screen_pixels,
	    screen_res_x*4
	    );

    //Send texture to rendering context (Does scaling automagically.)
	SDL_RenderCopy(renderer, screen_surface, NULL, NULL);
}

//Sets the color modulation of a texture
void set_texture_mod(SDL_Texture* texture, Color color)
{
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(texture, color.a);
}

//Gets the color modulation of a texture
Color get_texture_mod(SDL_Texture* texture)
{
	Color color;

	SDL_GetTextureColorMod(texture, &color.r, &color.g, &color.b);
	SDL_GetTextureAlphaMod(texture, &color.a);

	return color;
}

//Copies image from src to dst, doesnt apply scaling.
//Src Rect is the rect of the image to be blitted, src_size needs to be set to the size
//Of the ENTIRE image (src_size.x is different than src_rect.w if the src image is a spritesheet for example)
void blit_software(	Color* src, Color* dst, 
					const Rect src_rect , const uint32_t src_pitch, 
					const Point2 dst_pos, const uint32_t dst_pitch)
{
	if(dst == NULL)
	{

		dst = screen_pixels;
	}

	for(int y = 0; y < src_rect.h; y++)
	{
		memcpy(	dst + dst_pos.x + (y + dst_pos.y)*dst_pitch, 
				src + src_rect.x + (y + src_rect.y)*src_pitch,
				src_rect.w*4);
	}
}

//Performs an alpha blending blit of the source pixel array to the destination
//Pixel array.
//VERY SLOW, USE TEXTURES AND THE RENDERER INSTEAD. (GFX_blit_alpha_texture)
void blit_software_alpha(	Color* p_src, Color* p_dst, 
							const Rect src_rect, const uint32_t src_pitch, 
							const Point2 dst_pos, const uint32_t dst_pitch)
{
	if(p_dst == NULL)
	{

		p_dst = screen_pixels;
	}

	for(int x = 0; x < src_rect.w; x++)
	{
		for(int y = 0; y < src_rect.h; y++)
		{
			Color result;
			Color src = *(p_src + x + src_rect.x + (y + src_rect.y)*src_pitch);
			Color* dst_address = (p_dst + x + dst_pos.x  + (y + dst_pos.y )*dst_pitch);
			Color dst = *dst_address;

			result.r = ((int)src.r*src.a)/0xff + ((int)dst.r*(0xff - src.a))/0xff;
			result.g = ((int)src.g*src.a)/0xff + ((int)dst.g*(0xff - src.a))/0xff;
			result.b = ((int)src.b*src.a)/0xff + ((int)dst.b*(0xff - src.a))/0xff;
			result.a = src.a + ((int)dst.a * (0xff - src.a))/0xff;
			
			*dst_address = result;
		}
	}
}

//Copies image from src to screen
//Applies integer scaling if scale is != (1,1)
void blit_hardware_texture(	SDL_Texture* texture,
							Rect src_rect, Point2 dst_offset,
							Point2 scale)
{
	if(texture != NULL)
	{

		SDL_Rect* src_rect_ptr = (SDL_Rect*)&src_rect;	

		//Creates the desination rect from the offset and texture sizes
		Rect dst_rect = rect(	dst_offset.x, dst_offset.y,
								src_rect.w, src_rect.h);

		//Scales the rect by the engine pixel scale and the non-square scale argument
		scale = scale_i_p2(scale, pixel_scale);
		dst_rect = scale_i_rect(dst_rect, scale.x, scale.y);
		
		SDL_RenderCopy(	renderer,
						texture,
						src_rect_ptr,
						(SDL_Rect*)&dst_rect);
	}
}

//Copies image from src to screen
//Applies integer scaling if scale is != (1,1)
void blit_hardware(	uint32_t texture_id_src,
					Rect src_rect, Point2 dst_offset,
					Point2 scale)
{
	ResourceTexture* texture_src;

	if(get_texture_resource(texture_id_src, &texture_src))
	{
		blit_hardware_texture(	texture_src->texture,
								src_rect, dst_offset,
								scale);
	}
}

void draw_char_font( 	const Point2 position, ResourceFont* font, Color color,
						char character)
{
	char char_index = character - 32;

	Rect char_rect;

	char_rect.x = (char_index%32) * font->char_w;
	char_rect.y = (char_index/32) * font->char_h;
	char_rect.w = font->char_w;
	char_rect.h = font->char_h;

	Color old_modulation;

	old_modulation = get_texture_mod(font->texture);
	set_texture_mod(font->texture, color);
	
	blit_hardware_texture(	font->texture,
							char_rect, position,
							point2(1, 1));

	set_texture_mod(font->texture, old_modulation);

}


//Draws a char on screen
void draw_char(	const Point2 position, uint32_t font_id, Color color,
				char character)
{
	ResourceFont* font;

	if(get_font_resource(font_id, &font))
	{
		draw_char_font(position, font, color, character);
	}	
}

void draw_string(	const Point2 position, uint32_t font_id, Color color,
					char* string)
{
	ResourceFont* font;

	if(get_font_resource(font_id, &font))
	{

		uint32_t i = 0;
		char current_char = *string;
		while(current_char != '\0')
		{

			draw_char_font(	point2(position.x + i*font->char_w, position.y), font, color,
							current_char);
			i++;
			current_char = *(string + i);
		}
	}	
}

void draw_string_f(	const Point2 position, uint32_t font_id, Color color, 
					char* format_string, ...)
{
	char buffer[256] = {0};
	

	va_list args;

	va_start(args, format_string);
	vsnprintf(buffer, 256, format_string, args);
	va_end(args);

	draw_string(position, font_id, color, buffer);
}
