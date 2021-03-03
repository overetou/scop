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

void	parse_face_point(const char *file_content, size_t *i, GLfloat **final_vertices,
const GLfloat *vertices, const GLfloat *vts, const size_t file_size, size_t *final_vert_size,
size_t *text_coord_nb)
{
	int index;
	int vt_index;

	(*final_vertices) = (GLfloat*)realloc((*final_vertices), ((*final_vert_size) + 5) * sizeof(GLfloat));
	error_check(sscanf(file_content + (*i), "%i", &index) == 1, "Wrong obj file format.");
	printf("v pos: %d.\n", index);
	(*final_vertices)[*final_vert_size] = vertices[(index - 1) * 3];
	(*final_vertices)[(*final_vert_size) + 1] = vertices[(index - 1) * 3 + 1];
	(*final_vertices)[(*final_vert_size) + 2] = vertices[(index - 1) * 3 + 2];
	while (*i < file_size && file_content[*i] != ' ' && file_content[*i] != '/' && file_content[*i] != '\n')
		(*i)++;
	(*i)++;
	if (*i < file_size && file_content[(*i) - 1] == '/')
	{
		error_check(sscanf(file_content + (*i),"%i", &vt_index) == 1, "/ in face declaration of obj file is not folowed by a value.");
		printf("vt pos: %d.\n", vt_index);
		(*final_vertices)[(*final_vert_size) + 3] = vts[(vt_index - 1) * 2];
		(*final_vertices)[(*final_vert_size) + 4] = vts[(vt_index - 1) * 2 + 1];
		(*text_coord_nb)++;
		while(*i != file_size && file_content[*i] != ' ' && file_content[*i] != '\n')
			(*i)++;
		(*i)++;
		printf("cursor is now on '%c'.\n", file_content[*i]);
	}
	if (*i < file_size && file_content[(*i) - 1] == '\n')
		(*i)--;
	*final_vert_size += 5;
}

void	handle_fourth_face(const char *file_content, size_t *i, GLfloat **final_vertices,
const GLfloat *vertices, const GLfloat *vts, const size_t file_size, size_t *final_vert_size,
size_t *text_coord_nb)
{
	*final_vertices = (GLfloat*)realloc(*final_vertices, ((*final_vert_size) + 10) * sizeof(GLfloat));
	(*final_vertices)[*final_vert_size] = (*final_vertices)[(*final_vert_size) - 9];
	(*final_vertices)[(*final_vert_size) + 1] = (*final_vertices)[(*final_vert_size) - 8];
	(*final_vertices)[(*final_vert_size) + 2] = (*final_vertices)[(*final_vert_size) - 7];
	printf("just copied this as the first vertex in four face scenario: %f, %f, %f.\n", (*final_vertices)[(*final_vert_size) - 9], (*final_vertices)[(*final_vert_size) - 8], (*final_vertices)[(*final_vert_size) - 7]);
	(*final_vertices)[(*final_vert_size) + 3] = (*final_vertices)[(*final_vert_size) - 3];
	(*final_vertices)[(*final_vert_size) + 4] = (*final_vertices)[(*final_vert_size) - 2];
	(*final_vertices)[(*final_vert_size) + 5] = (*final_vertices)[(*final_vert_size) - 1];
	printf("just copied this as the second vertex in four face scenario: %f, %f, %f.\n", (*final_vertices)[(*final_vert_size) - 3], final_vertices[(*final_vert_size) - 2], final_vertices[(*final_vert_size) - 1]);
	*final_vert_size += 10;
	puts("The next given vertex info will be the fourth point of the face.");
	parse_face_point(file_content, i, final_vertices, vertices, vts, file_size, final_vert_size, text_coord_nb);
	*final_vert_size += 5;
}

GLfloat	*load_vertices(const char *file_name, size_t *vertices_len)
{
	size_t	file_size, vert_size = 0, final_vert_size = 0;
	int fd = open(file_name, O_RDONLY);
	size_t	i = 0;
	char	*file_content;
	GLfloat *vertices = NULL;
	size_t	text_coord_nb = 0;//Keeps count of the number of point that gave their texture coordinates.

	check_open(fd);
	file_size = get_file_size(fd);
	//printf("File size = %lu.\n", file_size);
	file_content = (char*)malloc(file_size + 1);
	file_content[file_size] = '\0';
	if (read(fd, file_content, file_size) != file_size)
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
	i = 0;
	GLfloat	*final_vertices = NULL;
	int index, vt_index;
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
			//if (i < file_size && file_content[i] == ' ')
			//	handle_fourth_face(file_content, &i, final_vertices, vertices, vts, file_size,
			//&final_vert_size, &text_coord_nb);
		}
		while(i != file_size && file_content[i++] != '\n');
	}
	i = 0;
	while(i != final_vert_size)
	{
		//printf("%f %f %f %f %f\n", final_vertices[i], final_vertices[i + 1], final_vertices[i + 2], final_vertices[i + 3], final_vertices[i + 4]);
		i += 5;
	}
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
size_t	allocate_graphic_side_objects(UINT *handles)
{
	size_t	vertices_len;
	GLfloat *home_vertices = load_vertices("cube2.obj", &vertices_len);
	glGenVertexArrays(1, handles + 4);
	glGenBuffers(1, handles);
	//glGenBuffers(1, handles + 5);
	glBindVertexArray(handles[4]);
	glBindBuffer(GL_ARRAY_BUFFER, *handles);
	glBufferData(GL_ARRAY_BUFFER, vertices_len * sizeof(GLfloat), home_vertices, GL_STATIC_DRAW);
	free(home_vertices);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles[5]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	compile_shader(GL_VERTEX_SHADER, handles + 1, VERTEX_SHADER_SOURCE);
	compile_shader(GL_FRAGMENT_SHADER, handles + 2, FRAGMENT_SHADER_SOURCE);
	handles[3] = glCreateProgram();
	glAttachShader(handles[3], handles[1]);
	glAttachShader(handles[3], handles[2]);
	glLinkProgram(handles[3]);
	check_compilation_step_success(handles[3], glGetProgramiv, GL_LINK_STATUS);
	glDeleteShader(handles[1]);
	glDeleteShader(handles[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glUseProgram(handles[3]);
	return (vertices_len / 5);
}

void	desallocate_graphic_side_objects(UINT *handles)
{
	glDeleteVertexArrays(1, handles + 4);
	glDeleteBuffers(1, handles);
	//glDeleteBuffers(1, handles + 5);
	glDeleteProgram(handles[3]);
}
