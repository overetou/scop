#define STB_IMAGE_IMPLEMENTATION
#include "scop.h"
#include <unistd.h>

unsigned int	get_file_size(int fd)
{
	int	result;

	result = lseek(fd, 0L, SEEK_END);
	if (result != -1)
	{
		lseek(fd, 0, SEEK_SET);
		return (result);
	}
	puts("Error while trying to reach obj file end.");
	exit(0);
}
