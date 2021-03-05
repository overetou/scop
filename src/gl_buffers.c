#include "scop.h"
#include <fcntl.h>

static void check_compilation_step_success(UINT handle,
void(*getter)(UINT, GLenum, int*), GLenum status)
{
	int success;
	char info_log[512];

	getter(handle, status, &success);
	if (success)
		return;
	glGetShaderInfoLog(handle, 512, NULL, info_log);
	printf("Shader compilation failed: %s\n", info_log);
	exit(0);
}

static void compile_shader(GLenum shader_type, UINT *shader_handle,
const char *shader_source)
{
	*shader_handle = glCreateShader(shader_type);
	glShaderSource(*shader_handle, 1, &shader_source, NULL);
	glCompileShader(*shader_handle);
	check_compilation_step_success(*shader_handle,	glGetShaderiv,
	GL_COMPILE_STATUS);
}

void	glfloat_cpy_n(GLfloat *dest, const GLfloat *src, size_t n)
{
	size_t	i;

	i = 0;
	while(i != n)
	{
		dest[i] = src[i];
		i++;
	}
}

void	parse_face_point(const char *file_content, size_t *i, GLfloat **final_vertices,
const GLfloat *vertices, const GLfloat *vts, const size_t file_size, size_t *final_vert_size,
size_t *text_coord_nb)
{
	int index;
	int vt_index;
	GLfloat shades[] = {0.2, 0.2, 0.2, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8};

	(*final_vertices) = (GLfloat*)realloc((*final_vertices), ((*final_vert_size) + 8) * sizeof(GLfloat));
	error_check(sscanf(file_content + (*i), "%i", &index) == 1, "Wrong obj file format.");
	//printf("v pos: %d read at index %lu.\n", index, *i);
	(*final_vertices)[*final_vert_size] = vertices[(index - 1) * 3];
	(*final_vertices)[(*final_vert_size) + 1] = vertices[(index - 1) * 3 + 1];
	(*final_vertices)[(*final_vert_size) + 2] = vertices[(index - 1) * 3 + 2];
	while (*i < file_size && file_content[*i] != ' ' && file_content[*i] != '/' && file_content[*i] != '\n')
		(*i)++;
	(*i)++;
	if (*i < file_size && file_content[(*i) - 1] == '/')
	{
		error_check(sscanf(file_content + (*i),"%i", &vt_index) == 1, "/ in face declaration of obj file is not folowed by a value.");
		//printf("vt pos: %d.\n", vt_index);
		(*final_vertices)[(*final_vert_size) + 3] = vts[(vt_index - 1) * 2];
		(*final_vertices)[(*final_vert_size) + 4] = vts[(vt_index - 1) * 2 + 1];
		(*text_coord_nb)++;
		while(*i != file_size && file_content[*i] != ' ' && file_content[*i] != '\n')
			(*i)++;
		(*i)++;
	}
	else
	{
		(*final_vertices)[(*final_vert_size) + 3] = (*final_vertices)[*final_vert_size] + (*final_vertices)[(*final_vert_size) + 2];
		(*final_vertices)[(*final_vert_size) + 4] = (*final_vertices)[(*final_vert_size) + 1] + (*final_vertices)[(*final_vert_size) + 2];
	}
	glfloat_cpy_n((*final_vertices) + (*final_vert_size) + 5, shades + (*i) % (3 * 3), 3);
	//printf("cursor is now on '%c'.\n", file_content[*i]);
	if (*i < file_size && file_content[(*i) - 1] == '\n')
		(*i)--;
	*final_vert_size += 8;
}

void	handle_fourth_face(const char *file_content, size_t *i, GLfloat **final_vertices,
const GLfloat *vertices, const GLfloat *vts, const size_t file_size, size_t *final_vert_size,
size_t *text_coord_nb)
{
	*final_vertices = (GLfloat*)realloc(*final_vertices, ((*final_vert_size) + 16) * sizeof(GLfloat));
	glfloat_cpy_n((*final_vertices) + (*final_vert_size), (*final_vertices) + (*final_vert_size) - 24, 8);
	//printf("just copied this as the first vertex in four face scenario: %f, %f, %f.\n",
	glfloat_cpy_n((*final_vertices) + (*final_vert_size) + 8, (*final_vertices) + (*final_vert_size) - 8, 8);
	//printf("just copied this as the second vertex in four face scenario: %f, %f, %f.\n", (*final_vertices)[(*final_vert_size) + 5], (*final_vertices)[(*final_vert_size) + 6], (*final_vertices)[(*final_vert_size) + 7]);
	*final_vert_size += 16;
	//puts("The next given vertex info will be the fourth point of the face.");
	parse_face_point(file_content, i, final_vertices, vertices, vts, file_size, final_vert_size, text_coord_nb);
}

char	is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

GLfloat	*load_vertices(const char *file_name, size_t *vertices_len)
{
	size_t	file_size, vert_size = 0, final_vert_size = 0;
	int fd = open(file_name, O_RDONLY);
	size_t	i = 0;
	char	*file_content;
	GLfloat *vertices = NULL;
	size_t	text_coord_nb = 0;//Keeps count of the number of point that gave their texture coordinates.

	puts("load_vertices start");
	check_open(fd);
	file_size = get_file_size(fd);
	//printf("File size = %lu.\n", file_size);
	file_content = (char*)malloc(file_size + 1);
	file_content[file_size] = '\0';
	if (read(fd, file_content, file_size) != (ssize_t)file_size)
	{
		puts("Unable to read obj file correctly.");
		exit(0);
	}
	GLfloat	*vts = NULL;
	size_t	len_vts = 0;
	while (i + 1 < file_size)
	{
		if (file_content[i] == 'v' && file_content[i + 1] == ' ')
		{
			i += 2;
			vertices = (GLfloat*)realloc(vertices, (vert_size + 3) * sizeof(GLfloat));
			if (sscanf(file_content + i, "%f %f %f", vertices + vert_size, vertices + vert_size + 1, vertices + vert_size + 2) != 3)
			{
				puts("Unable to parse vertex.");
				exit(0);
			}
//			printf("At vert index: %lu:\n%f, %f, %f\n", vert_size, vertices[vert_size], vertices[vert_size + 1], vertices[vert_size + 2]);
			vert_size += 3;
		}
		else if (i + 2 < file_size && file_content[i] == 'v' && file_content[i + 1] == 't' && file_content[i + 2] == ' ')
		{
			i += 3;
			vts = (GLfloat*)realloc(vts, (len_vts + 2) * sizeof(GLfloat));
			sscanf(file_content + i, "%f %f", vts + len_vts, vts + len_vts + 1);
			len_vts += 2;
		}
		while (i != file_size && file_content[i++] != '\n');
	}
	normalize_obj(vertices, vert_size);
	i = 0;
	GLfloat	*final_vertices = NULL;
	while (i + 1 < file_size)
	{
		if (file_content[i] == 'f' && file_content[i + 1] == ' ')
		{
			i += 2;
			parse_face_point(file_content, &i, &final_vertices, vertices, vts, file_size,
			&final_vert_size, &text_coord_nb);
			parse_face_point(file_content, &i, &final_vertices, vertices, vts, file_size,
			&final_vert_size, &text_coord_nb);
			parse_face_point(file_content, &i, &final_vertices, vertices, vts, file_size,
			&final_vert_size, &text_coord_nb);
			if (i < file_size && is_digit(file_content[i]))
				handle_fourth_face(file_content, &i, &final_vertices, vertices, vts, file_size,
			&final_vert_size, &text_coord_nb);
		}
		while(i != file_size && file_content[i++] != '\n');
	}
	i = 0;
	//while(i != final_vert_size)
	//{
		//printf("%f %f %f %f %f\n", final_vertices[i], final_vertices[i + 1], final_vertices[i + 2], final_vertices[i + 3], final_vertices[i + 4]);
//		i += 8;
//	}
	if (text_coord_nb == final_vert_size / 8)
		puts("texture coordinates seem to have been correctly given.");
	else
		puts("Texture coordinates absents or incomplete.");
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
	glDeleteBuffers(1, handles);
	//glDeleteBuffers(1, handles + 5);
	glDeleteProgram(handles[3]);
}
