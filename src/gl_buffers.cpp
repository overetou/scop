#include "scop.h"
#include "fcntl.h"
#include "unistd.h"

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

void			load_obj(const char *path, t_parsing_storage *storage)
{
	int			fd;

	fd = open(path, O_RDONLY);
	if (fd != -1)
		do_obj_parsing(fd, storage, 0);
	else
	{
		puts("could not open the given obj file.");
		exit(0);
	}
}

/*
**We bind our raw buffers to targets to further describe
**how they should be used.
**glBufferData copies the target data to the graphic card's memory.
**Additionaly, it gives a hint: GL_STATIC_DRAW means that that buffer data
**will be set only one time, but used many times.
**glVertexAttribPointer: a function used to configure the vertex shader argse.
*/
void	allocate_graphic_side_objects(UINT *handles, const char *path)
{
	t_parsing_storage parsing_info;
	GLfloat	*obj_final_data[3];

	load_obj(path, &parsing_info);
	normalize_obj(&parsing_info);
	obj_final_data[0] = load_vertex_data_from_storage(&parsing_info,
	&(obj_final_data[1]), &(obj_final_data[2]));
	int i = 0;
	while (i < parsing_info.v_nb)
	{
		printf("%f, %f, %f\n", parsing_info.vert[i], parsing_info.vert[i + 1], parsing_info.vert[i+ 2]);
		i += 3;
	}
	glGenVertexArrays(1, handles + 4);
	glGenBuffers(1, handles);
	//glGenBuffers(1, handles + 5);
	glBindVertexArray(handles[4]);
	glBindBuffer(GL_ARRAY_BUFFER, *handles);
	glBufferData(GL_ARRAY_BUFFER, parsing_info.v_nb, parsing_info.vert, GL_STATIC_DRAW);
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
}

void	desallocate_graphic_side_objects(UINT *handles)
{
	glDeleteVertexArrays(1, handles + 4);
	glDeleteBuffers(1, handles);
	//glDeleteBuffers(1, handles + 5);
	glDeleteProgram(handles[3]);
}