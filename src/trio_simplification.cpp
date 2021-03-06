#include "scop.h"

void	do_for_each(GLfloat *tab1, GLfloat *tab2,
GLfloat *tab3, void (*func)(GLfloat *tab))
{
	func(tab1);
	func(tab2);
	func(tab3);
}

void	affect_diffs(GLfloat *vertices_sub_tab,
GLfloat diff1, GLfloat diff2, GLfloat diff3)
{
	vertices_sub_tab[0] += diff1;
	vertices_sub_tab[1] += diff2;
	vertices_sub_tab[2] += diff3;
}

void	determine_tab_diff(GLfloat *tab)
{
	if (tab[1] > (tab[1] - tab[0]) / 2)
		tab[2] = -(tab[1] - (tab[1] - tab[0]) / 2);
	else
		tab[2] = (tab[1] - tab[0]) / 2 - tab[1];
}

void	assign_max_min_initial_values(GLfloat *tab, GLfloat val)
{
	tab[1] = val;
	tab[0] = tab[1];
}

void	check_vertices_number(t_parsing_storage *storage)
{
	if (storage->v_nb < 3)
	{
		puts("Insufficient number of vertices counted. Stopping now.");
		exit(0);
	}
}