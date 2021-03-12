/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_buffers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:48:22 by overetou          #+#    #+#             */
/*   Updated: 2021/03/11 17:01:02 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <fcntl.h>

void	parse_faces(t_parsing_storage *st)
{
	while ((st->i) + 1 < st->file_size)
	{
		if (st->file_content[st->i] == 'f' &&
		st->file_content[(st->i) + 1] == ' ')
		{
			st->i += 2;
			parse_face_point(st);
			parse_face_point(st);
			parse_face_point(st);
			if (st->i < st->file_size && is_digit(st->file_content[st->i]))
				handle_fourth_face(st);
		}
		while (st->i != st->file_size && st->file_content[st->i] != '\n')
			(st->i)++;
		(st->i)++;
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
	close(fd);
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

void	allocate_gso_part_one(GLuint *handles, GLfloat *home_vertices,
size_t vertices_len)
{
	glGenVertexArrays(1, handles + 4);
	gl_check_errors("glGenVertexArrays");
	glGenBuffers(1, handles);
	gl_check_errors("glGenBuffers");
	glBindVertexArray(handles[4]);
	gl_check_errors("glBindVertexArray");
	glBindBuffer(GL_ARRAY_BUFFER, *handles);
	gl_check_errors("glBindBuffer");
	glBufferData(GL_ARRAY_BUFFER, vertices_len * sizeof(GLfloat),
	home_vertices, GL_STATIC_DRAW);
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

size_t	allocate_graphic_side_objects(GLuint *handles, t_master *m)
{
	m->home_vertices = load_vertices(m->obj_file_path, &(m->vertices_len));
	allocate_gso_part_one(handles, m->home_vertices, m->vertices_len);
	glDeleteShader(handles[1]);
	gl_check_errors("glDeleteShader");
	glDeleteShader(handles[2]);
	gl_check_errors("glDeleteShader");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
	(void*)0);
	gl_check_errors("glVertexAttribPointer");
	glEnableVertexAttribArray(0);
	gl_check_errors("glEnableVertexAttribArray");
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
	(void*)(3 * sizeof(float)));
	gl_check_errors("glVertexAttribPointer");
	glEnableVertexAttribArray(1);
	gl_check_errors("glEnableVertexAttribArray");
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
	(void*)(5 * sizeof(float)));
	gl_check_errors("glVertexAttribPointer");
	glEnableVertexAttribArray(2);
	gl_check_errors("glEnableVertexAttribArray");
	glUseProgram(handles[3]);
	gl_check_errors("glUseProgram");
	return (m->vertices_len / 8);
}

void	desallocate_graphic_side_objects(GLuint *handles)
{
	glDeleteVertexArrays(1, handles + 4);
	gl_check_errors("glDeleteVertexArrays");
	glDeleteBuffers(1, handles);
	gl_check_errors("glDeleteBuffers");
	glDeleteProgram(handles[3]);
	gl_check_errors("glDeleteProgram");
}
