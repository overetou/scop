#include "scop.h"

static void	error_check_gl(GLenum error)
{
	if (error == GL_NO_ERROR)
		return;
	if (error == GL_INVALID_ENUM)
		puts("An unacceptable value is specified for an enumerated argument.");
	else if (error == GL_INVALID_VALUE)
		puts("A numeric argument is out of range.");
	else if (error == GL_INVALID_OPERATION)
		puts("The specified operation is not allowed in the current state.");
	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
		puts("The framebuffer object is not complete.");
	else if (error == GL_OUT_OF_MEMORY)
		puts("There is not enough memory left to execute the command.");
	else if (error == GL_STACK_UNDERFLOW)
		puts("An attempt has been made to perform an operation that would cause an internal stack to underflow.");
	else if (error == GL_STACK_OVERFLOW)
		puts("An attempt has been made to perform an operation that would cause an internal stack to overflow.");
	else
		puts("Unknown error.");
	exit(0);
}

void	create_vbo(UINT *vao_id, UINT *vbo_id, UINT *color_buffer_id)
{	GLfloat Vertices[] = {
	-0.8f, -0.8f, 0.0f, 1.0f,
	 0.0f,	0.8f, 0.0f, 1.0f,
	 0.8f, -0.8f, 0.0f, 1.0f
	};

	GLfloat Colors[] = {
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, vao_id);
	glBindVertexArray(*vao_id);
	glGenBuffers(1, vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, color_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, *color_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	error_check_gl(glGetError());
}

void	destroy_vbo(UINT *vao_id, UINT *vbo_id, UINT *color_buffer_id)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, color_buffer_id);
	glDeleteBuffers(1, vbo_id);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, vao_id);
	error_check_gl(glGetError());
}

void	create_shaders(UINT *vert_shad_id, UINT *frag_shad_id, UINT *prog_id)
{
	const GLchar *src;

	*vert_shad_id = glCreateShader(GL_VERTEX_SHADER);
	src = VERTEX_SHADER_SOURCE;
	glShaderSource(*vert_shad_id, 1, &src, NULL);
	glCompileShader(*vert_shad_id);
	*frag_shad_id = glCreateShader(GL_FRAGMENT_SHADER);
	src = FRAGMENT_SHADER_SOURCE;
	glShaderSource(*frag_shad_id, 1, &src, NULL);
	glCompileShader(*frag_shad_id);
	*prog_id = glCreateProgram();
	glAttachShader(*prog_id, *vert_shad_id);
	glAttachShader(*prog_id, *frag_shad_id);
	glLinkProgram(*prog_id);
	glUseProgram(*prog_id);
	error_check_gl(glGetError());
}

void	destroy_shaders(UINT *vert_shad_id, UINT *frag_shad_id, UINT *prog_id)
{
	glUseProgram(0);
	glDetachShader(*prog_id, *vert_shad_id);
	glDetachShader(*prog_id, *frag_shad_id);
	glDeleteShader(*frag_shad_id);
	glDeleteShader(*vert_shad_id);
	glDeleteProgram(*prog_id);
	error_check_gl(glGetError());
}