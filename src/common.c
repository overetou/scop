#include "scop.h"

//Returns the reallocated pointer or stop the program after a free if error.
void	*secure_realloc(void *to_realloc, size_t new_size)
{
	void	*temp;

	temp = to_realloc;
	to_realloc = realloc(to_realloc, new_size);
	if (to_realloc)
		return (to_realloc);
	free(temp);
	puts("A realloc failed.");
	exit(0);
}