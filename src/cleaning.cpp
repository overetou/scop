#include "scop.h"

void	free_three(void *adr1, void *adr2, void *adr3)
{
	free(adr1);
	free(adr2);
	free(adr3);
}

void	store_if_min_or_max(GLfloat *tab, GLfloat val)
{
	if (val < tab[0])
		tab[0] = val;
	else if (val > tab[1])
		tab[1] = val;
}

void	normalize_obj(t_parsing_storage *storage)
{
	GLfloat			x[3];
	GLfloat			y[3];
	GLfloat			z[3];
	unsigned int	i;

	check_vertices_number(storage);
	assign_max_min_initial_values(x, storage->vert[0]);
	assign_max_min_initial_values(y, storage->vert[1]);
	assign_max_min_initial_values(z, storage->vert[2]);
	i = 3;
	while (i != storage->v_nb)
	{
		store_if_min_or_max(x, storage->vert[i]);
		store_if_min_or_max(y, storage->vert[i + 1]);
		store_if_min_or_max(z, storage->vert[i + 2]);
		i += 3;
	}
	do_for_each(x, y, z, determine_tab_diff);
	i = 0;
	while (i != storage->v_nb)
	{
		affect_diffs(storage->vert + i, x[2], y[2], z[2]);
		i += 3;
	}
}
