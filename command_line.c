#include <string.h>

#define command_buffer_size 256
char command_buffer[command_buffer_size];

void init_command_line()
{
	memset(command_buffer, 0, command_buffer_size);
}

void enter_command()
{

}