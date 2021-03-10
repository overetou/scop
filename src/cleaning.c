/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:46:21 by overetou          #+#    #+#             */
/*   Updated: 2021/03/10 13:58:19 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	normalize_obj(GLfloat *vertices, size_t vert_size)
{
	GLfloat			x[3];
	GLfloat			y[3];
	GLfloat			z[3];
	size_t			i;

	if (vert_size < 3 || vert_size % 3 != 0)
		return ;
	assign_max_min_initial_values(x, vertices[0]);
	assign_max_min_initial_values(y, vertices[1]);
	assign_max_min_initial_values(z, vertices[2]);
	i = 3;
	while (i != vert_size)
	{
		store_if_min_or_max(x, vertices[i]);
		store_if_min_or_max(y, vertices[i + 1]);
		store_if_min_or_max(z, vertices[i + 2]);
		i += 3;
	}
	do_for_each(x, y, z, determine_tab_diff);
	i = 0;
	while (i != vert_size)
	{
		affect_diffs(vertices + i, x[2], y[2], z[2]);
		i += 3;
	}
}

void	gl_check_errors(const char *msg)
{
	GLenum	code;

	code = glGetError();
	if (code != GL_NO_ERROR)
	{
		printf("Error code: %x at step: %s.\n", code, msg);
		exit(0);
	}
}
