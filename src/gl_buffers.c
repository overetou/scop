/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_buffers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:48:22 by overetou          #+#    #+#             */
/*   Updated: 2021/03/10 14:01:08 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <fcntl.h>

void	parse_texture_coords(t_parsing_storage *st)
{
	int vt_index;

	error_check(sscanf(st->file_content + (st->i),"%i", &vt_index) == 1, "/ in face declaration of obj file is not folowed by a value.");
	glfloat_cpy_n((st->final_vertices) + (st->final_vert_size) + 3, st->vts + (vt_index - 1) * 2, 2);
	while(st->i != st->file_size && st->file_content[st->i] != ' ' && st->file_content[st->i] != '\n')
		(st->i)++;
	(st->i)++;
}

void	parse_face_point(t_parsing_storage *st)
{
	int index;

	(st->final_vertices) = (GLfloat*)realloc((st->final_vertices), ((st->final_vert_size) + 8) * sizeof(GLfloat));
	error_check(sscanf(st->file_content + (st->i), "%i", &index) == 1, "Wrong obj file format.");
	glfloat_cpy_n((st->final_vertices) + (st->final_vert_size), st->vertices + (index - 1) * 3, 3);
	while (st->i < st->file_size && st->file_content[st->i] != ' ' && st->file_content[st->i] != '/' && st->file_content[st->i] != '\n')
		(st->i)++;
	(st->i)++;
	if (st->i < st->file_size && st->file_content[(st->i) - 1] == '/')
		parse_texture_coords(st);
	else
	{
		(st->final_vertices)[(st->final_vert_size) + 3] = (st->final_vertices)[st->final_vert_size] + (st->final_vertices)[(st->final_vert_size) + 2];
		(st->final_vertices)[(st->final_vert_size) + 4] = (st->final_vertices)[(st->final_vert_size) + 1] + (st->final_vertices)[(st->final_vert_size) + 2];
	}
	fill_vec3((st->final_vertices) + (st->final_vert_size) + 5, st->shaker, st->shaker, st->shaker);
	change_shaker(st);
	if (st->i < st->file_size && st->file_content[(st->i) - 1] == '\n')
		(st->i)--;
	st->final_vert_size += 8;
}

void	handle_fourth_face(t_parsing_storage *st)
{
	st->final_vertices = (GLfloat*)realloc(st->final_vertices, ((st->final_vert_size) + 16) * sizeof(GLfloat));
	glfloat_cpy_n((st->final_vertices) + (st->final_vert_size), (st->final_vertices) + (st->final_vert_size) - 24, 5);
	fill_vec3((st->final_vertices) + (st->final_vert_size) + 5, st->shaker, st->shaker, st->shaker);
	change_shaker(st);
	glfloat_cpy_n((st->final_vertices) + (st->final_vert_size) + 8, (st->final_vertices) + (st->final_vert_size) - 8, 8);
	fill_vec3((st->final_vertices) + (st->final_vert_size) + 13, st->shaker, st->shaker, st->shaker);
	change_shaker(st);
	st->final_vert_size += 16;
	parse_face_point(st);
}

char	is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

void	parse_vertexs_and_text_coords(t_parsing_storage *st)
{
	while (st->i + 1 < st->file_size)
	{
		if (st->file_content[st->i] == 'v' && st->file_content[(st->i) + 1] == ' ')
		{
			st->i += 2;
			st->vertices = (GLfloat*)realloc(st->vertices, ((st->vert_size) + 3) * sizeof(GLfloat));
			if (sscanf(st->file_content + st->i, "%f %f %f",
			(st->vertices) + (st->vert_size), (st->vertices) + (st->vert_size) + 1,
			(st->vertices) + (st->vert_size) + 2) != 3)
			{
				puts("Unable to parse vertex.");
				exit(0);
			}
			st->vert_size += 3;
		}
		else if ((st->i) + 2 < st->file_size && st->file_content[st->i] == 'v' && st->file_content[(st->i) + 1] == 't' && st->file_content[(st->i) + 2] == ' ')
		{
			st->i += 3;
			(st->vts) = (GLfloat*)realloc(st->vts, ((st->len_vts) + 2) * sizeof(GLfloat));
			sscanf(st->file_content + st->i, "%f %f", (st->vts) + (st->len_vts), (st->vts) + (st->len_vts) + 1);
			st->len_vts += 2;
		}
		while (st->i != st->file_size && st->file_content[(st->i)++] != '\n');
	}
}

void	parse_faces(t_parsing_storage *st)
{
	while ((st->i) + 1 < st->file_size)
	{
		if (st->file_content[st->i] == 'f' && st->file_content[(st->i) + 1] == ' ')
		{
			st->i += 2;
			parse_face_point(st);
			parse_face_point(st);
			parse_face_point(st);
			if (st->i < st->file_size && is_digit(st->file_content[st->i]))
				handle_fourth_face(st);
		}
		while(st->i != st->file_size && st->file_content[(st->i)++] != '\n');
	}
}

GLfloat	*load_vertices(const char *file_name, size_t *vertices_len)
{
	t_parsing_storage	st;
	int					fd;

	fd = open(file_name, O_RDONLY);
	error_check(fd > 0, "Could not read obj file.");
	st.shaker = 0.25;
	bzero(&st, sizeof(st));
	st.file_size = get_file_size(fd);
	st.file_content = (char*)secure_malloc(st.file_size + 1);
	(st.file_content)[st.file_size] = '\0';
	error_check(read(fd, st.file_content, st.file_size)
	== (ssize_t)(st.file_size), "Unable to read obj file correctly.");
	parse_vertexs_and_text_coords(&st);
	normalize_obj(st.vertices, st.vert_size);
	st.i = 0;
	parse_faces(&st);
	*vertices_len = st.final_vert_size;
	free(st.file_content);
	if (st.vertices)
		free(st.vertices);
	if (st.vts)
		free(st.vts);
	return (st.final_vertices);
}

void	allocate_gso_part_one(UINT *handles, GLfloat *home_vertices, size_t vertices_len)
{
	glGenVertexArrays(1, handles + 4);
	gl_check_errors("glGenVertexArrays");
	glGenBuffers(1, handles);
	gl_check_errors("glGenBuffers");
	glBindVertexArray(handles[4]);
	gl_check_errors("glBindVertexArray");
	glBindBuffer(GL_ARRAY_BUFFER, *handles);
	gl_check_errors("glBindBuffer");
	glBufferData(GL_ARRAY_BUFFER, vertices_len * sizeof(GLfloat), home_vertices, GL_STATIC_DRAW);
	gl_check_errors("glBufferData");
	free(home_vertices);
	compile_shader(GL_VERTEX_SHADER, handles + 1, VERTEX_SHADER_SOURCE);
	compile_shader(GL_FRAGMENT_SHADER, handles + 2, FRAGMENT_SHADER_SOURCE);
	handles[3] = glCreateProgram();
	gl_check_errors("glCreateProgram");
	glAttachShader(handles[3], handles[1]);
	gl_check_errors("glAttachShader 1");
	glAttachShader(handles[3], handles[2]);
	gl_check_errors("glAttachShader 2");
	glLinkProgram(handles[3]);
	gl_check_errors("glLinkProgram");
	check_compilation_step_success(handles[3], glGetProgramiv, GL_LINK_STATUS);
}

/*
**We bind our raw buffers to targets to further describe
**how they should be used.
**glBufferData copies the target data to the graphic card's memory.
**Additionaly, it gives a hint: GL_STATIC_DRAW means that that buffer data
**will be set only one time, but used many times.
**glVertexAttribPointer: a function used to configure the vertex shader argse.
*/
size_t	allocate_graphic_side_objects(UINT *handles, t_master *m)
{
	size_t	vertices_len;
	GLfloat *home_vertices;

	home_vertices = load_vertices(m->obj_file_path, &vertices_len);
	allocate_gso_part_one(handles, home_vertices, vertices_len);
	glDeleteShader(handles[1]);
	gl_check_errors("glDeleteShader");
	glDeleteShader(handles[2]);
	gl_check_errors("glDeleteShader");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	gl_check_errors("glVertexAttribPointer");
	glEnableVertexAttribArray(0);
	gl_check_errors("glEnableVertexAttribArray");
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	gl_check_errors("glVertexAttribPointer");
	glEnableVertexAttribArray(1);
	gl_check_errors("glEnableVertexAttribArray");
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	gl_check_errors("glVertexAttribPointer");
	glEnableVertexAttribArray(2);
	gl_check_errors("glEnableVertexAttribArray");
	glUseProgram(handles[3]);
	gl_check_errors("glUseProgram");
	return (vertices_len / 8);
}

void	desallocate_graphic_side_objects(UINT *handles)
{
	glDeleteVertexArrays(1, handles + 4);
	gl_check_errors("glDeleteVertexArrays");
	glDeleteBuffers(1, handles);
	gl_check_errors("glDeleteBuffers");
	glDeleteProgram(handles[3]);
	gl_check_errors("glDeleteProgram");
}
