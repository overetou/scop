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

void	parse_texture_coords(const char *file_content, size_t *i, GLfloat **final_vertices, size_t *final_vert_size, const GLfloat *vts, const size_t file_size)
{
	int vt_index;

	error_check(sscanf(file_content + (*i),"%i", &vt_index) == 1, "/ in face declaration of obj file is not folowed by a value.");
	glfloat_cpy_n((*final_vertices) + (*final_vert_size) + 3, vts + (vt_index - 1) * 2, 2);
	while(*i != file_size && file_content[*i] != ' ' && file_content[*i] != '\n')
		(*i)++;
	(*i)++;
}

void	parse_face_point(const char *file_content, size_t *i, GLfloat **final_vertices,
const GLfloat *vertices, const GLfloat *vts, const size_t file_size, size_t *final_vert_size, GLfloat *shaker)
{
	int index;

	(*final_vertices) = (GLfloat*)realloc((*final_vertices), ((*final_vert_size) + 8) * sizeof(GLfloat));
	error_check(sscanf(file_content + (*i), "%i", &index) == 1, "Wrong obj file format.");
	glfloat_cpy_n((*final_vertices) + (*final_vert_size), vertices + (index - 1) * 3, 3);
	while (*i < file_size && file_content[*i] != ' ' && file_content[*i] != '/' && file_content[*i] != '\n')
		(*i)++;
	(*i)++;
	if (*i < file_size && file_content[(*i) - 1] == '/')
		parse_texture_coords(file_content, i, final_vertices, final_vert_size, vts, file_size);
	else
	{
		(*final_vertices)[(*final_vert_size) + 3] = (*final_vertices)[*final_vert_size] + (*final_vertices)[(*final_vert_size) + 2];
		(*final_vertices)[(*final_vert_size) + 4] = (*final_vertices)[(*final_vert_size) + 1] + (*final_vertices)[(*final_vert_size) + 2];
	}
	fill_vec3((*final_vertices) + (*final_vert_size) + 5, *shaker, *shaker, *shaker);
	change_shaker(shaker);
	if (*i < file_size && file_content[(*i) - 1] == '\n')
		(*i)--;
	*final_vert_size += 8;
}

void	handle_fourth_face(const char *file_content, size_t *i, GLfloat **final_vertices,
const GLfloat *vertices, const GLfloat *vts, const size_t file_size, size_t *final_vert_size, GLfloat *shaker)
{
	*final_vertices = (GLfloat*)realloc(*final_vertices, ((*final_vert_size) + 16) * sizeof(GLfloat));
	glfloat_cpy_n((*final_vertices) + (*final_vert_size), (*final_vertices) + (*final_vert_size) - 24, 5);
	fill_vec3((*final_vertices) + (*final_vert_size) + 5, *shaker, *shaker, *shaker);
	change_shaker(shaker);
	glfloat_cpy_n((*final_vertices) + (*final_vert_size) + 8, (*final_vertices) + (*final_vert_size) - 8, 8);
	fill_vec3((*final_vertices) + (*final_vert_size) + 13, *shaker, *shaker, *shaker);
	change_shaker(shaker);
	*final_vert_size += 16;
	parse_face_point(file_content, i, final_vertices, vertices, vts, file_size, final_vert_size, shaker);
}

char	is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

void	parse_vertexs_and_text_coords(size_t	file_size, size_t *i, char	*file_content, GLfloat	**vertices, size_t (*vert_size), GLfloat	**vts, size_t	*len_vts)
{
	while ((*i) + 1 < file_size)
	{
		if (file_content[(*i)] == 'v' && file_content[(*i) + 1] == ' ')
		{
			(*i) += 2;
			(*vertices) = (GLfloat*)realloc((*vertices), ((*vert_size) + 3) * sizeof(GLfloat));
			if (sscanf(file_content + (*i), "%f %f %f", (*vertices) + (*vert_size), (*vertices) + (*vert_size) + 1, (*vertices) + (*vert_size) + 2) != 3)
			{
				puts("Unable to parse vertex.");
				exit(0);
			}
			(*vert_size) += 3;
		}
		else if ((*i) + 2 < file_size && file_content[(*i)] == 'v' && file_content[(*i) + 1] == 't' && file_content[(*i) + 2] == ' ')
		{
			(*i) += 3;
			(*vts) = (GLfloat*)realloc((*vts), ((*len_vts) + 2) * sizeof(GLfloat));
			sscanf(file_content + (*i), "%f %f", (*vts) + (*len_vts), (*vts) + (*len_vts) + 1);
			(*len_vts) += 2;
		}
		while ((*i) != file_size && file_content[(*i)++] != '\n');
	}
}

GLfloat	*load_vertices(const char *file_name, size_t *vertices_len)
{
	size_t	len_vts = 0;
	size_t	file_size, vert_size = 0, final_vert_size = 0;
	int		fd = open(file_name, O_RDONLY);
	size_t	i = 0;
	char	*file_content;
	GLfloat	*vertices = NULL;
	GLfloat	shaker = 0.25;
	GLfloat	*vts = NULL;

	error_check(fd > 0, "Could not read obj file.");
	file_size = get_file_size(fd);
	//printf("File size = %lu.\n", file_size);
	file_content = (char*)secure_malloc(file_size + 1);
	file_content[file_size] = '\0';
	if (read(fd, file_content, file_size) != (ssize_t)file_size)
	{
		puts("Unable to read obj file correctly.");
		exit(0);
	}
	parse_vertexs_and_text_coords(file_size, &i, file_content, &vertices, &vert_size, &vts, &len_vts);
	normalize_obj(vertices, vert_size);
	i = 0;
	GLfloat	*final_vertices = NULL;
	while (i + 1 < file_size)
	{
		if (file_content[i] == 'f' && file_content[i + 1] == ' ')
		{
			i += 2;
			parse_face_point(file_content, &i, &final_vertices, vertices, vts, file_size,
			&final_vert_size, &shaker);
			parse_face_point(file_content, &i, &final_vertices, vertices, vts, file_size,
			&final_vert_size, &shaker);
			parse_face_point(file_content, &i, &final_vertices, vertices, vts, file_size,
			&final_vert_size, &shaker);
			if (i < file_size && is_digit(file_content[i]))
				handle_fourth_face(file_content, &i, &final_vertices, vertices, vts, file_size,
			&final_vert_size, &shaker);
		}
		while(i != file_size && file_content[i++] != '\n');
	}
	i = 0;
	*vertices_len = final_vert_size;
	free(file_content);
	if (vertices)
		free(vertices);
	if (vts)
		free(vts);
	return (final_vertices);
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
	GLfloat *home_vertices = load_vertices(m->obj_file_path, &vertices_len);
	glGenVertexArrays(1, handles + 4);
	gl_check_errors("glGenVertexArrays");
	glGenBuffers(1, handles);
	gl_check_errors("glGenBuffers");
	//glGenBuffers(1, handles + 5);
	glBindVertexArray(handles[4]);
	gl_check_errors("glBindVertexArray");
	glBindBuffer(GL_ARRAY_BUFFER, *handles);
	gl_check_errors("glBindBuffer");
	glBufferData(GL_ARRAY_BUFFER, vertices_len * sizeof(GLfloat), home_vertices, GL_STATIC_DRAW);
	gl_check_errors("glBufferData");
	free(home_vertices);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles[5]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
