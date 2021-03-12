/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_buffer_part_one.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 16:52:09 by overetou          #+#    #+#             */
/*   Updated: 2021/03/11 16:56:27 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	parse_face_point(t_parsing_storage *st)
{
	int index;

	(st->final_vertices) = (GLfloat*)realloc((st->final_vertices),
	((st->final_vert_size) + 8) * sizeof(GLfloat));
	error_check(sscanf(st->file_content + (st->i), "%i", &index) == 1,
	"Wrong obj file format.");
	glfloat_cpy_n((st->final_vertices) + (st->final_vert_size),
	st->vertices + (index - 1) * 3, 3);
	while (st->i < st->file_size && st->file_content[st->i] != ' ' &&
	st->file_content[st->i] != '/' && st->file_content[st->i] != '\n')
		(st->i)++;
	(st->i)++;
	if (st->i < st->file_size && st->file_content[(st->i) - 1] == '/')
		parse_texture_coords(st);
	else
		generate_tex_coord_for_vertex(st);
	fill_vec3((st->final_vertices) + (st->final_vert_size) + 5, st->shaker,
	st->shaker, st->shaker);
	change_shaker(st);
	if (st->i < st->file_size && st->file_content[(st->i) - 1] == '\n')
		(st->i)--;
	st->final_vert_size += 8;
}

void	handle_fourth_face(t_parsing_storage *st)
{
	st->final_vertices = (GLfloat*)realloc(st->final_vertices,
	((st->final_vert_size) + 16) * sizeof(GLfloat));
	glfloat_cpy_n((st->final_vertices) + (st->final_vert_size),
	(st->final_vertices) + (st->final_vert_size) - 24, 5);
	fill_vec3((st->final_vertices) + (st->final_vert_size) + 5,
	st->shaker, st->shaker, st->shaker);
	change_shaker(st);
	glfloat_cpy_n((st->final_vertices) + (st->final_vert_size) + 8,
	(st->final_vertices) + (st->final_vert_size) - 8, 8);
	fill_vec3((st->final_vertices) + (st->final_vert_size) + 13, st->shaker,
	st->shaker, st->shaker);
	change_shaker(st);
	st->final_vert_size += 16;
	parse_face_point(st);
}

char	is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

void	parse_vertex_declaration(t_parsing_storage *st)
{
	st->i += 2;
	st->vertices = (GLfloat*)realloc(st->vertices,
	((st->vert_size) + 3) * sizeof(GLfloat));
	if (sscanf(st->file_content + st->i, "%f %f %f",
	(st->vertices) + (st->vert_size),
	(st->vertices) + (st->vert_size) + 1,
	(st->vertices) + (st->vert_size) + 2) != 3)
	{
		puts("Unable to parse vertex.");
		exit(0);
	}
	st->vert_size += 3;
}

void	parse_vertexs_and_text_coords(t_parsing_storage *st)
{
	while (st->i + 1 < st->file_size)
	{
		if (st->file_content[st->i] == 'v' &&
		st->file_content[(st->i) + 1] == ' ')
			parse_vertex_declaration(st);
		else if ((st->i) + 2 < st->file_size && st->file_content[st->i] ==
		'v' && st->file_content[(st->i) + 1] == 't' &&
		st->file_content[(st->i) + 2] == ' ')
		{
			st->i += 3;
			(st->vts) = (GLfloat*)realloc(st->vts,
			((st->len_vts) + 2) * sizeof(GLfloat));
			error_check(sscanf(st->file_content + st->i, "%f %f", (st->vts) +
			(st->len_vts), (st->vts) + (st->len_vts) + 1) == 2,
			"Obj parsing failed.");
			st->len_vts += 2;
		}
		while (st->i != st->file_size && st->file_content[st->i] != '\n')
			(st->i)++;
		(st->i)++;
	}
}
