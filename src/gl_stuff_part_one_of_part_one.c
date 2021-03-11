/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_stuff_part_one_of_part_one.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 16:57:00 by overetou          #+#    #+#             */
/*   Updated: 2021/03/11 16:57:01 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	parse_texture_coords(t_parsing_storage *st)
{
	int vt_index;

	error_check(sscanf(st->file_content + (st->i), "%i", &vt_index) == 1,
	"/ in face declaration of obj file is not folowed by a value.");
	glfloat_cpy_n((st->final_vertices) + (st->final_vert_size) + 3,
	st->vts + (vt_index - 1) * 2, 2);
	while (st->i != st->file_size && st->file_content[st->i] != ' ' &&
	st->file_content[st->i] != '\n')
		(st->i)++;
	(st->i)++;
}

void	generate_tex_coord_for_vertex(t_parsing_storage *st)
{
	(st->final_vertices)[(st->final_vert_size) + 3] = (st->final_vertices)
	[st->final_vert_size] + (st->final_vertices)[(st->final_vert_size) + 2];
	(st->final_vertices)[(st->final_vert_size) + 4] =
	(st->final_vertices)[(st->final_vert_size) + 1] +
	(st->final_vertices)[(st->final_vert_size) + 2];
}
