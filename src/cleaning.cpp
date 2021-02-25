#include "scop.h"

void	free_three(void *adr1, void *adr2, void *adr3)
{
	free(adr1);
	free(adr2);
	free(adr3);
}
